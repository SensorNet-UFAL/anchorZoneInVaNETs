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

//
// Veins Mobility module for the INET Framework (i.e., implementing inet::IMobility)
// Based on inet::MobilityBase of INET Framework v3.4.0
//

#ifndef Veins_AnchorZoneMobility_h
#define Veins_AnchorZoneMobility_h

#include "inet/common/ModuleAccess.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/geometry/common/EulerAngles.h"
#include "inet/common/geometry/common/CanvasProjection.h"
#include "inet/mobility/contract/IMobility.h"
//#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "anchor_zone/AnchorZoneManager.h"



namespace Veins {

class INET_API AnchorZoneMobility : public cSimpleModule, public inet::IMobility {
//class INET_API AnchorZoneMobility : public TraCIMobility, public inet::IMobility {
	public:
		AnchorZoneMobility();

	public:
		virtual void preInitialize(std::string external_id, const inet::Coord& position, std::string road_id, double speed, double angle);
		virtual void nextPosition(const inet::Coord& position, std::string road_id, double speed, double angle);

	public:
		virtual double getMaxSpeed() const override;

		virtual inet::Coord getCurrentPosition() override;
		virtual inet::Coord getCurrentSpeed() override;
		virtual inet::EulerAngles getCurrentAngularPosition() override;
		virtual inet::EulerAngles getCurrentAngularSpeed() override { return inet::EulerAngles::ZERO; }

		virtual inet::Coord getConstraintAreaMax() const override { return constraintAreaMax; }
		virtual inet::Coord getConstraintAreaMin() const override { return constraintAreaMin; }

		virtual double getAzId() { return azId; }

		virtual void setAzId(double azId) {
		    this->azId = azId;
		}

        virtual void setExternalId(std::string external_id) {
            this->external_id = external_id;
        }

        virtual std::string getExternalId() const {
            if (external_id == "") throw cRuntimeError("TraCIMobility::getExternalId called with no external_id set yet");
            return external_id;
        }

        virtual AnchorZoneManager* getManager() const {
            if (!manager) manager = AnchorZoneManagerAccess().get();
            return manager;
        }

        virtual TraCICommandInterface* getCommandInterface() const {
            if (!commandInterface) commandInterface = getManager()->getCommandInterface();
            return commandInterface;
        }
        virtual TraCICommandInterface::Vehicle* getVehicleCommandInterface() const {
            if (!vehicleCommandInterface) vehicleCommandInterface = new TraCICommandInterface::Vehicle(getCommandInterface()->vehicle(getExternalId()));
            return vehicleCommandInterface;
        }



	protected:
		cModule *visualRepresentation;
		const inet::CanvasProjection *canvasProjection;

		inet::Coord constraintAreaMin, constraintAreaMax;

		inet::Coord lastPosition;
		inet::Coord lastSpeed;
		inet::EulerAngles lastOrientation;

		double azId;
		std::string external_id; /**< updated by setExternalId() */
		mutable AnchorZoneManager* manager;
        mutable TraCICommandInterface* commandInterface;
        mutable TraCICommandInterface::Vehicle* vehicleCommandInterface;


	protected:
		virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }

		virtual void initialize(int stage) override;

		virtual void handleMessage(cMessage *msg) override;

		virtual void updateVisualRepresentation();

		virtual void emitMobilityStateChangedSignal();

	    AnchorZoneMobility* mobility;
	    TraCICommandInterface* traci;
	    TraCICommandInterface::Vehicle* traciVehicle;

};

} // namespace Veins

namespace Veins {
class AnchorZoneMobilityAccess
{
    public:
        AnchorZoneMobility* get(cModule* host) {
            AnchorZoneMobility* traci = FindModule<AnchorZoneMobility*>::findSubModule(host);
            ASSERT(traci);
            return traci;
        };
};
}


#endif // ifndef Veins_AnchorZoneMobility_h


