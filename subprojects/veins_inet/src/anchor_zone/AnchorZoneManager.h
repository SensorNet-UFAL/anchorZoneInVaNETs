//
// Copyright (C) 2006-2017 Christoph Sommer <sommer@ccs-labs.org>
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

#ifndef Veins_AnchorZoneManager_h
#define Veins_AnchorZoneManager_h

#include <omnetpp.h>

#include "veins/modules/mobility/traci/TraCIScenarioManagerLaunchd.h"
//#include "veins/modules/mobility/traci/TraCIColor.h"
//#include "veins/modules/mobility/traci/TraCICommandInterface.h"
//#include "anchor_zone/AnchorZoneMobility.h"


namespace Veins {

/**
 * @brief
 * Creates and manages network nodes corresponding to cars.
 *
 * See the Veins website <a href="http://veins.car2x.org/"> for a tutorial, documentation, and publications </a>.
 *
 * @author Christoph Sommer
 *
 */
class AnchorZoneManager : public TraCIScenarioManagerLaunchd {
	public:

		virtual ~AnchorZoneManager();
		virtual void preInitializeModule(cModule* mod, const std::string& nodeId, const Coord& position, const std::string& road_id, double speed, double angle, VehicleSignal signals);
		virtual void updateModulePosition(cModule* mod, const Coord& p, const std::string& edge, double speed, double angle, VehicleSignal signals);

	protected:
	//	AnchorZoneMobility* mobility;
	//	TraCICommandInterface* traci;
	//	TraCICommandInterface::Vehicle* traciVehicle;



};
}


namespace Veins {
class AnchorZoneManagerAccess
{
	public:
		AnchorZoneManager* get() {
			return FindModule<AnchorZoneManager*>::findGlobalModule();
		};
};
}


#endif // ifndef Veins_AnchorZoneManager_h
