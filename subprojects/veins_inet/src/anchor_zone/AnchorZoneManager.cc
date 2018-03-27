#include "anchor_zone/AnchorZoneManager.h"
#include "veins/base/utils/Coord.h"
#include "anchor_zone/AnchorZoneMobility.h"

using Veins::AnchorZoneManager;

Define_Module(Veins::AnchorZoneManager);

AnchorZoneManager::~AnchorZoneManager() {
}

void AnchorZoneManager::preInitializeModule(cModule* mod, const std::string& nodeId, const Coord& position, const std::string& road_id, double speed, double angle, VehicleSignal signals) {
	// pre-initialize AnchorZoneMobility

	for (cModule::SubmoduleIterator iter(mod); !iter.end(); iter++) {
		cModule* submod = SUBMODULE_ITERATOR_TO_MODULE(iter);
		AnchorZoneMobility* inetmm = dynamic_cast<AnchorZoneMobility*>(submod);
		if (!inetmm) return;
		inetmm->preInitialize(nodeId, inet::Coord(position.x, position.y), road_id, speed, angle);


	}
	  //mobility = AnchorZoneManagerAccess().get(getParentModule());

	  //traciVehicle = mobility->getVehicleCommandInterface();
}

void AnchorZoneManager::updateModulePosition(cModule* mod, const Coord& p, const std::string& edge, double speed, double angle, VehicleSignal signals) {
	double teste;
    TraCICoord testeCoord; Coord teste2;
    // update position in AnchorZoneMobility
	for (cModule::SubmoduleIterator iter(mod); !iter.end(); iter++) {
		cModule* submod = SUBMODULE_ITERATOR_TO_MODULE(iter);
		AnchorZoneMobility *inetmm = dynamic_cast<AnchorZoneMobility*>(submod);
/*
		azId = inetmm->getAzId();
        if(azId>100){
            traciVehicle->setColor(TraCIColor::fromTkColor("DarkRed"));
        }
        else if (azId>50) {
            traciVehicle->setColor(TraCIColor::fromTkColor("DarkGreen"));
        }
        else{
            traciVehicle->setColor(TraCIColor::fromTkColor("DarkBlue"));
        }

*/
        // inetmm->setAzId(p.y);
        // teste = inetmm->getAzId();

		if (!inetmm) return;

		testeCoord.x = 100;
		testeCoord.y = 100;


		teste2 = connection->traci2omnet(testeCoord);

		inetmm->nextPosition(inet::Coord(p.x, p.y), edge, speed, angle);
		//inetmm->nextPosition(inet::Coord(teste2.x, teste2.y), edge, speed, angle);




	}







}



