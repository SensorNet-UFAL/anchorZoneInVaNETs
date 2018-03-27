#include "inet/common/INETMath.h"
#include "anchor_zone/AnchorZoneMobility.h"
#include "inet/visualizer/mobility/MobilityCanvasVisualizer.h"

namespace Veins {

using inet::Coord;

Register_Class(AnchorZoneMobility);


//
// Public, lifecycle
//

AnchorZoneMobility::AnchorZoneMobility() :
	visualRepresentation(nullptr),
	constraintAreaMin(Coord::ZERO),
	constraintAreaMax(Coord::ZERO),
	lastPosition(Coord::ZERO),
	lastSpeed(Coord::ZERO),
	azId(0),
	lastOrientation(inet::EulerAngles::ZERO) {
}

//
// Public, called from manager
//

void AnchorZoneMobility::preInitialize(std::string external_id, const inet::Coord& position, std::string road_id, double speed, double angle) {
	Enter_Method_Silent();
	lastPosition = position;
	lastSpeed = Coord(cos(angle), -sin(angle));
	lastOrientation.alpha = -angle;

    /*mobility = AnchorZoneMobilityAccess().get(getParentModule());
    traci = mobility->getCommandInterface();
    traciVehicle = mobility->getVehicleCommandInterface();*/

}

void AnchorZoneMobility::nextPosition(const inet::Coord& position, std::string road_id, double speed, double angle) {
	Enter_Method_Silent();
	lastPosition = position;
	lastSpeed = Coord(cos(angle), -sin(angle));
	lastOrientation.alpha = -angle;
	emitMobilityStateChangedSignal();
	updateVisualRepresentation();
	azId = position.y; //TESTE

    /*if(azId > 100){

        traciVehicle->setColor(TraCIColor::fromTkColor("DarkRed"));
    }
    else if (azId > 50) {
        traciVehicle->setColor(TraCIColor::fromTkColor("DarkGreen"));
    }
    else{
        traciVehicle->setColor(TraCIColor::fromTkColor("DarkBlue"));
    }*/




}

//
// Public, implementing IMobility interface
//

double AnchorZoneMobility::getMaxSpeed() const {
	return NaN;
}

Coord AnchorZoneMobility::getCurrentPosition() {
	return lastPosition;
}

Coord AnchorZoneMobility::getCurrentSpeed() {
	return lastSpeed;
}

inet::EulerAngles AnchorZoneMobility::getCurrentAngularPosition() {
	return lastOrientation;
}


//
// Protected
//

void AnchorZoneMobility::initialize(int stage) {
	cSimpleModule::initialize(stage);
	//EV_TRACE << "initializing AnchorZoneMobility stage " << stage << endl;
	if (stage == inet::INITSTAGE_LOCAL) {
		constraintAreaMin.x = par("constraintAreaMinX");
		constraintAreaMin.y = par("constraintAreaMinY");
		constraintAreaMin.z = par("constraintAreaMinZ");
		constraintAreaMax.x = par("constraintAreaMaxX");
		constraintAreaMax.y = par("constraintAreaMaxY");
		constraintAreaMax.z = par("constraintAreaMaxZ");
		bool visualizeMobility = par("visualizeMobility");
		if (visualizeMobility) {
			visualRepresentation = inet::getModuleFromPar<cModule>(par("visualRepresentation"), this);
		}
		WATCH(constraintAreaMin);
		WATCH(constraintAreaMax);
		WATCH(lastPosition);
		WATCH(lastSpeed);
		WATCH(lastOrientation);
	}
	else if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2) {
		if (visualRepresentation != nullptr) {
			auto visualizationTarget = visualRepresentation->getParentModule();
			canvasProjection = inet::CanvasProjection::getCanvasProjection(visualizationTarget->getCanvas());
		}
		emitMobilityStateChangedSignal();
		updateVisualRepresentation();
	}
}

void AnchorZoneMobility::handleMessage(cMessage *message) {
	throw cRuntimeError("This module does not handle messages");
}

void AnchorZoneMobility::updateVisualRepresentation() {
	EV_DEBUG << "current position = " << lastPosition << endl;
	if (hasGUI() && visualRepresentation != nullptr) {
		inet::visualizer::MobilityCanvasVisualizer::setPosition(visualRepresentation, canvasProjection->computeCanvasPoint(getCurrentPosition()));
	}
}

void AnchorZoneMobility::emitMobilityStateChangedSignal() {
	emit(mobilityStateChangedSignal, this); //ALTERADO
}


} // namespace veins
