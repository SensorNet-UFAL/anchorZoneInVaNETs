//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef Veins_TraCIAZApp_H_
#define Veins_TraCIAZApp_H_

#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "AnchorZoneMobility.h"
#include "veins/modules/mobility/traci/TraCIColor.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/base/utils/asserts.h"

namespace Veins {
class TraCIAZApp : public BaseApplLayer {
public:
    void initialize(int stage);
    void finish();
    int getAzId(Coord vehiclePos);
protected:
    static const simsignalwrap_t mobilityStateChangedSignal;

protected:
     // module parameters
     AnchorZoneMobility* mobility;
     TraCICommandInterface* traci;
     TraCICommandInterface::Vehicle* traciVehicle;
     //std::set<std::string> visitedEdges; /**< set of edges this vehicle visited */
     //bool hasStopped; /**< true if at some point in time this vehicle travelled at negligible speed */
     int primeiro;

protected:
     virtual void onBSM(BasicSafetyMessage* bsm);
     void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject* details);
     void handlePositionUpdate();
};
}

#endif /* SRC_VEINS_APPLICATIONS_TRACIAZAPP_H_ */
