//
// Copyright (C) 2006-2012 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include <algorithm>

#include "veins/base/utils/asserts.h"

#include "veins/modules/application/az/TraCIAzApp.h"
#include "veins/modules/mobility/traci/TraCIColor.h"



using Veins::TraCIMobility;
using Veins::TraCIMobilityAccess;

using Veins::TraCIAzApp;

Define_Module(Veins::TraCIAzApp);

void TraCIAzApp::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    lastDroveAt = simTime();
    currentAzSelected = 0;
    AzIdOfWsm = 0;

    if (stage==0) {

        //initialize pointers to other modules
        if (FindModule<TraCIMobility*>::findSubModule(getParentModule())) {
            mobility = TraCIMobilityAccess().get(getParentModule());
            traci = mobility->getCommandInterface();
            traciVehicle = mobility->getVehicleCommandInterface();
        }
        else {
            traci = NULL;
            mobility = NULL;
            traciVehicle = NULL;
        }

        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        mac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(
                getParentModule());
        assert(mac);

        myId = getParentModule()->getId();

        //read parameters
        headerLength = par("headerLength").longValue();
        sendBeacons = par("sendBeacons").boolValue();
        beaconLengthBits = par("beaconLengthBits").longValue();
        beaconUserPriority = par("beaconUserPriority").longValue();
        beaconInterval =  par("beaconInterval");

        dataLengthBits = par("dataLengthBits").longValue();
        dataOnSch = par("dataOnSch").boolValue();
        dataUserPriority = par("dataUserPriority").longValue();

        wsaInterval = par("wsaInterval").doubleValue();
        communicateWhileParked = par("communicateWhileParked").boolValue();
        currentOfferedServiceId = -1;

        isParked = false;


        findHost()->subscribe(mobilityStateChangedSignal, this);
        findHost()->subscribe(parkingStateChangedSignal, this);

        sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);
        sendWSAEvt = new cMessage("wsa evt", SEND_WSA_EVT);

        generatedBSMs = 0;
        generatedWSAs = 0;
        generatedWSMs = 0;
        receivedBSMs = 0;
        receivedWSAs = 0;
        receivedWSMs = 0;

        //quantityVehicles = 0;

    }
    else if (stage == 1) {
        //simulate asynchronous channel access

        if (dataOnSch == true && !mac->isChannelSwitchingActive()) {
            dataOnSch = false;
            std::cerr << "App wants to send data on SCH but MAC doesn't use any SCH. Sending all data on CCH" << std::endl;
        }
        simtime_t firstBeacon = simTime();

        if (par("avoidBeaconSynchronization").boolValue() == true) {

            simtime_t randomOffset = dblrand() * beaconInterval;
            firstBeacon = simTime() + randomOffset;

            if (mac->isChannelSwitchingActive() == true) {
                if ( beaconInterval.raw() % (mac->getSwitchingInterval().raw()*2)) {
                    std::cerr << "The beacon interval (" << beaconInterval << ") is smaller than or not a multiple of  one synchronization interval (" << 2*mac->getSwitchingInterval() << "). "
                            << "This means that beacons are generated during SCH intervals" << std::endl;
                }
                firstBeacon = computeAsynchronousSendingTime(beaconInterval, type_CCH);
            }

            if (sendBeacons) {
                scheduleAt(firstBeacon, sendBeaconEvt);
            }
        }
    }
}



void TraCIAzApp::handlePositionUpdate(cObject* obj) {

    curPosition = mobility->getCurrentPosition();
    curSpeed = mobility->getCurrentSpeed();

    manager = mobility->getManager();

    double azId =  this->getAzId(curPosition);

    traciVehicle->setAzId(azId);

    if(manager->isSelectedAz( azId )){
            traciVehicle->setColor(TraCIColor::fromTkColor("DarkRed"));
            //TESTE - Verifica se estou na AZ selecionada que estava no ultimo instante
            if (currentAzSelected != azId){
                currentAzSelected = azId;
                mobility->currentAnchorZone.recordWithTimestamp(simTime(), azId);
            }
            if (manager->getExistCFinAZ(azId) == false){
                //WaveShortMessage* wsm = new WaveShortMessage();
                wsmFC = new WaveShortMessage();
                populateWSM(wsmFC);
                wsmFC->setAz( azId );
                wsmFC->setWsmData(mobility->getRoadId().c_str());
                sendDown(wsmFC);
               // AzIdOfWsm = azId;
            }
            else{
                if (AzIdOfWsm == azId){
                    WaveShortMessage* forwardWsm = new WaveShortMessage(*wsmFC);
                    forwardWsm->setWsmVersion(wsmFC->getWsmVersion() + 1);
                    sendDelayedDown(forwardWsm, 3);
                    manager->currentFloatContent.recordWithTimestamp(simTime(), azId );
                    manager->currentFcForwarded.recordWithTimestamp(simTime(), forwardWsm->getWsmVersion() );
                }
            }
    }
    else{
            traciVehicle->setColor(TraCIColor::fromTkColor("DarkGreen"));
    }
}


void TraCIAzApp::onWSM(WaveShortMessage* wsm) {
    double azIdMessage, azIdVehicle;
    azIdMessage = wsm->getAz();
    azIdVehicle = traciVehicle->getAzId();

    if (azIdMessage == azIdVehicle){
        receivedWSMs++;
        if (azIdVehicle != AzIdOfWsm){
            manager->setExistCFinAZ(azIdMessage);
            wsmFC = new WaveShortMessage(*wsm);
            wsmFC->setSenderAddress(myId);
            wsmFC->setWsmVersion(wsm->getWsmVersion() + 1);
            WaveShortMessage* forwardWsm = new WaveShortMessage(*wsmFC);
            sendDelayedDown(forwardWsm, 3);
            manager->currentFloatContent.recordWithTimestamp(simTime(), azIdMessage );
            manager->currentFcForwarded.recordWithTimestamp(simTime(), forwardWsm->getWsmVersion() );
            AzIdOfWsm = azIdMessage;
       }
    }
};


double TraCIAzApp::getAzId(Coord currentPosition){

    std::pair<double,double> LonLat;

    LonLat= traci->getLonLat(currentPosition);

    int line = ( (LonLat.second - 37.52239) / 0.001796632 );  //0-74 lines
    int column = ( ( 122.1015 + LonLat.first ) / 0.002265394 ); //0-49 columns
    double az_id =  (line * 100) + column;
    return(az_id);

}

void TraCIAzApp::finish(){
    //recordScalar("quantityVehicles",quantityVehicles);

    recordScalar("generatedWSMs",generatedWSMs);
    recordScalar("receivedWSMs",receivedWSMs);

    recordScalar("generatedBSMs",generatedBSMs);
    recordScalar("receivedBSMs",receivedBSMs);



}
