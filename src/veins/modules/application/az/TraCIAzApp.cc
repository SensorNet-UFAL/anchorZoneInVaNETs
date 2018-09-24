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
    azIdOfWsm = 0;

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

    double azId =  this->getAzId(curPosition, manager->getLatLongIni(), manager->getfactorXY(), manager->getdistmLong(), manager->getdistmLat());
    double azIdSelected;

    traciVehicle->setAzId(azId);

    if(manager->isSelectedAz( azId )){

            azIdSelected = manager->returnAzSelected(azId); //Caso ForwardAZ habilitado, retorna AZ principal
            //traciVehicle->setColor(TraCIColor::fromTkColor("DarkRed"));
            //TESTE - Verifica se estou na AZ selecionada que estava no ultimo instante
            if (currentAzSelected != azIdSelected){
                currentAzSelected = azIdSelected;
                mobility->currentAnchorZone.recordWithTimestamp(simTime(), azIdSelected);
            }

            if (manager->existFCinAZ(azIdSelected) == false){
                WaveShortMessage* wsmFC = new WaveShortMessage();
                populateWSM(wsmFC);
                wsmFC->setAz( azIdSelected );
                wsmFC->setWsmData(mobility->getRoadId().c_str());
                //ID do CF: ID da AZ + sequencia a cada novo FC por AZ
                mobility->currentFloatContent.recordWithTimestamp(simTime(), azIdSelected);
                sendDown(wsmFC);
                azIdOfWsm = azIdSelected;
            }
            else{
                if (azIdOfWsm == azIdSelected){
                    WaveShortMessage* forwardWsm = new WaveShortMessage(*manager->getExistFCinAZ(azIdSelected) );
                    forwardWsm->setWsmVersion(forwardWsm->getWsmVersion() + 1);
                    sendDelayedDown(forwardWsm, 1);
                    //sendDown(forwardWsm);
                    manager->currentFloatContent.recordWithTimestamp(simTime(), azIdSelected );
                    manager->currentFcForwarded.recordWithTimestamp(simTime(), forwardWsm->getWsmVersion() );
                    mobility->currentFloatContent.recordWithTimestamp(simTime(), azIdSelected);
                }
		else{
                    azIdOfWsm = 0; //teste

                }
            }
    }
    else{
            //traciVehicle->setColor(TraCIColor::fromTkColor("DarkGreen"));
            currentAzSelected = 0;
            azIdOfWsm = 0;
    }
}


void TraCIAzApp::onWSM(WaveShortMessage* wsm) {

    double azIdMessage = wsm->getAz();
    double azIdVehicle =  manager->returnAzSelected( traciVehicle->getAzId() );

    if (azIdMessage == azIdVehicle ){
        receivedWSMs++;
        if (  azIdVehicle != azIdOfWsm){
            WaveShortMessage* msg = new WaveShortMessage(*wsm);
            msg->setSenderAddress(myId);
            msg->setWsmVersion(wsm->getWsmVersion() + 1);
            manager->setExistFCinAZ(azIdMessage, msg);
            sendDelayedDown(msg, 1);
            //sendDown(msg);
            manager->nodesReceivedCF++;
            manager->currentFloatContent.recordWithTimestamp(simTime(), azIdMessage );
            manager->currentFcForwarded.recordWithTimestamp(simTime(), msg->getWsmVersion() );
            mobility->currentFloatContent.recordWithTimestamp(simTime(), azIdMessage);
            azIdOfWsm = azIdMessage;
        }
        //else{
        //    azIdOfWsm = 0;
        //}
    }
};


double TraCIAzApp::getAzId(Coord currentPosition, Coord positionGridIni, int factorXY, double distmLong, double distmLat){

    std::pair<double,double> LonLat;
    int line, column;
    double latIni = positionGridIni.y;
    double longIni = positionGridIni.x;

    LonLat= traci->getLonLat(currentPosition);
    line = ( (LonLat.second - latIni) / distmLat );
    column = ( (LonLat.first - longIni) / distmLong );

    double az_id =  (line * factorXY) + column;

    return(az_id);

}

void TraCIAzApp::finish(){
    //recordScalar("quantityVehicles",quantityVehicles);
    recordScalar("generatedWSMs",generatedWSMs);
    recordScalar("receivedWSMs",receivedWSMs);
    recordScalar("generatedBSMs",generatedBSMs);
    recordScalar("receivedBSMs",receivedBSMs);

}
