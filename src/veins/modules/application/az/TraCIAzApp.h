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

#ifndef TraCIAzApp_H
#define TraCIAzApp_H

#include <set>
#include <list>

#include <omnetpp.h>

//#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIScenarioManagerLaunchd.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h" //TESTE

using Veins::TraCIMobility;
using Veins::TraCICommandInterface;

/**
 * FIXME
 */
namespace Veins {
class TraCIAzApp : public BaseWaveApplLayer  {
	public:

	protected:
        TraCIScenarioManager* manager;
        simtime_t lastDroveAt;
        //bool forwardCF;
        bool existCf; //variable to verify if exist CF when vehicle enter in a AZ selected
        bool enterAZSelected; //MUDAR PARA NR DA AZ PARA EVITAR PROBLEMA QUANDO AZs PROXIMAS
        double currentAzSelected;

	protected:
        virtual void initialize(int stage);
        virtual void handlePositionUpdate(cObject* obj);
        virtual void onWSM(WaveShortMessage* wsm);
        virtual double getAzId(Coord currentPosition);



};
}

#endif
