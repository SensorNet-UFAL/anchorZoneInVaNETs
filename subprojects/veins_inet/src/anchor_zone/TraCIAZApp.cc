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

#include <anchor_zone/TraCIAZApp.h>

using Veins::AnchorZoneMobility;
using Veins::TraCIAZApp;

Define_Module(Veins::TraCIAZApp);

const simsignalwrap_t TraCIAZApp::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);



void TraCIAZApp::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    if (stage == 0) {
        //debug = par("debug");
        //testNumber = par("testNumber");
        mobility = AnchorZoneMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();
        findHost()->subscribe(mobilityStateChangedSignal, this);
        primeiro = par("segundo");


        //visitedEdges.clear();
        //hasStopped = false;

        //if (debug) std::cout << "TraCITestApp initialized with testNumber=" << testNumber << std::endl;
    }
}

void TraCIAZApp::finish() {
}

void TraCIAZApp::onBSM(BasicSafetyMessage* bsm) {
    //Your application has received a beacon message from another car or RSU
    //code for handling the message goes here

}

void TraCIAZApp::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject* details) {
    if (signalID == mobilityStateChangedSignal) {
        handlePositionUpdate();
    }
}

void TraCIAZApp::handlePositionUpdate() {
    //const simtime_t t = simTime();
  //  const std::string roadId = mobility->getRoadId();
   // visitedEdges.insert(roadId);
/*
    traciVehicle->setAzId(mobility->getCurrentPosition().y);
    if(traciVehicle->getAzId()>100){
        traciVehicle->setColor(TraCIColor::fromTkColor("DarkRed"));
    }
    else if (traciVehicle->getAzId()>50) {
        traciVehicle->setColor(TraCIColor::fromTkColor("DarkGreen"));
    }
    else{
        traciVehicle->setColor(TraCIColor::fromTkColor("DarkBlue"));
    }
*/

    /*pegar Longitude e latitude
    std::pair<double, double> LonLat;
    LonLat = traci->getLonLat(mobility->getCurrentPosition());
    traciVehicle->setAzId(LonLat.first);
     */

  /*  int testCounter = 0;

    if (testNumber == testCounter++) {
        if (t == 9) {
            assertTrue("(Vehicle::getSpeed) vehicle is driving", mobility->getSpeed() > 25);
        }
        if (t == 10) {
            traciVehicle->setSpeedMode(0x00);
            traciVehicle->setSpeed(0);
        }
        if (t == 11) {
            assertClose("(Vehicle::setSpeed, Vehicle::setSpeedMode) vehicle has stopped", 0.0, mobility->getSpeed());
        }
        if (t == 12) {
            traciVehicle->setSpeedMode(0xff);
            traciVehicle->setSpeed(-1);
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            traciVehicle->changeRoute("42", 9999);
            traciVehicle->changeRoute("43", 9999);
        }
        if (t == 30) {
            assertTrue("(Vehicle::changeRoute, 9999) vehicle avoided 42", visitedEdges.find("42") == visitedEdges.end());
            assertTrue("(Vehicle::changeRoute, 9999) vehicle avoided 43", visitedEdges.find("43") == visitedEdges.end());
            assertTrue("(Vehicle::changeRoute, 9999) vehicle took 44", visitedEdges.find("44") != visitedEdges.end());
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            traciVehicle->changeRoute("42", 9999);
            traciVehicle->changeRoute("43", 9999);
        }
        if (t == 3) {
            traciVehicle->changeRoute("42", -1);
            traciVehicle->changeRoute("44", 9999);
        }
        if (t == 30) {
            assertTrue("(Vehicle::changeRoute, -1) vehicle took 42", visitedEdges.find("42") != visitedEdges.end());
            assertTrue("(Vehicle::changeRoute, -1) vehicle avoided 43", visitedEdges.find("43") == visitedEdges.end());
            assertTrue("(Vehicle::changeRoute, -1) vehicle avoided 44", visitedEdges.find("44") == visitedEdges.end());
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            assertClose("(getDistance, air)", 859., floor(traci->getDistance(Coord(25,7030), Coord(883,6980), false)));
            assertClose("(getDistance, driving)", 847., floor(traci->getDistance(Coord(25,7030), Coord(883,6980), true)));
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            traci->vehicle(mobility->getExternalId()).stopAt("43", 20, 0, 10, 30);
        }
        if (t == 30) {
            assertTrue("(Vehicle::stopAt) vehicle is at 43", roadId == "43");
            assertClose("(Vehicle::stopAt) vehicle is stopped", 0.0, mobility->getSpeed());
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            traci->trafficlight("10").setProgram("myProgramRed");
        }
        if (t == 30) {
            assertTrue("(Trafficlight::setProgram) vehicle is at 31", roadId == "31");
            assertClose("(Trafficlight::setProgram) vehicle is stopped", 0.0, mobility->getSpeed());
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            traci->trafficlight("10").setProgram("myProgramGreenRed");
            traci->trafficlight("10").setPhaseIndex(1);
        }
        if (t == 30) {
            assertTrue("(Trafficlight::setProgram, Trafficlight::setPhaseIndex) vehicle is at 31", roadId == "31");
            assertClose("(Trafficlight::setProgram, Trafficlight::setPhaseIndex) vehicle is stopped", 0.0, mobility->getSpeed());
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            std::list<std::string> polys = traci->getPolygonIds();
            assertEqual("(getPolygonIds) number is 1", polys.size(), (size_t)1);
            assertEqual("(getPolygonIds) id is correct", *polys.begin(), "poly0");
            std::string typeId = traci->polygon("poly0").getTypeId();
            assertEqual("(Polygon::getTypeId) typeId is correct", typeId, "type0");
            std::list<Coord> shape = traci->polygon("poly0").getShape();
            assertClose("(Polygon::getShape) shape x coordinate is correct", 130.0, shape.begin()->x);
            assertClose("(Polygon::getShape) shape y coordinate is correct", 81.65, shape.begin()->y);
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            std::list<Coord> shape1 = traci->polygon("poly0").getShape();
            assertClose("(Polygon::getShape) shape x coordinate is correct", 130.0, shape1.begin()->x);
            assertClose("(Polygon::getShape) shape y coordinate is correct", 81.65, shape1.begin()->y);
            std::list<Coord> shape2 = shape1;
            shape2.begin()->x = 135;
            shape2.begin()->y = 85;
            traci->polygon("poly0").setShape(shape2);
            std::list<Coord> shape3 = traci->polygon("poly0").getShape();
            assertClose("(Polygon::setShape, Polygon::getShape) shape x coordinate was changed", 135.0, shape3.begin()->x);
            assertClose("(Polygon::setShape, Polygon::getShape) shape y coordinate was changed", 85.0, shape3.begin()->y);
        }
    }

    if (testNumber == testCounter++) {
        if (t == 30) {
            std::list<Coord> points;
            points.push_back(Coord(100, 100));
            points.push_back(Coord(200, 100));
            points.push_back(Coord(200, 200));
            points.push_back(Coord(100, 200));
            traci->addPolygon("testPoly", "testType", TraCIColor::fromTkColor("red"), true, 1, points);
        }
        if (t == 31) {
            std::list<std::string> polys = traci->getPolygonIds();
            assertEqual("(addPolygon, getPolygonIds) number is 2", polys.size(), (size_t)2);
            assertTrue("(addPolygon, getPolygonIds) ids contain added", std::find(polys.begin(), polys.end(), std::string("testPoly")) != polys.end());
            std::string typeId = traci->polygon("testPoly").getTypeId();
            assertEqual("(Polygon::getTypeId) typeId is correct", typeId, "testType");
            std::list<Coord> shape = traci->polygon("testPoly").getShape();
            assertClose("(Polygon::getShape) shape x coordinate is correct", 100.0, shape.begin()->x);
            assertClose("(Polygon::getShape) shape y coordinate is correct", 100.0, shape.begin()->y);
        }
    }

    if (testNumber == testCounter++) {
        if (t == 30) {
            std::list<std::string> lanes = traci->getLaneIds();
            assertTrue("(getLaneIds) returns test lane", std::find(lanes.begin(), lanes.end(), "10_0") != lanes.end());
            Coord shape_front_coord = traci->lane("10_0").getShape().front();
            assertClose("(Lane::getShape) shape x coordinate is correct", 523., floor(shape_front_coord.x));
            assertClose("(Lane::getShape) shape y coordinate is correct", 79., floor(shape_front_coord.y));
        }
    }

    if (testNumber == testCounter++) {
        if (t == 30) {
            std::list<std::string> junctions = traci->getJunctionIds();
            assertTrue("(getJunctionIds) returns test junction", std::find(junctions.begin(), junctions.end(), "1") != junctions.end());
            Coord pos = traci->junction("1").getPosition();
            assertClose("(Junction::getPosition) shape x coordinate is correct", 25.0, pos.x);
            assertClose("(Junction::getPosition) shape y coordinate is correct", 75.0, pos.y);
        }
    }

    if (testNumber == testCounter++) {
        if (t == 28) {
            bool r = traci->addVehicle("testVehicle0", "vtype0", "route0");
            assertTrue("(addVehicle) command reports success", r);
        }
        if (t == 30) {
            std::map<std::string, cModule*>::const_iterator i = mobility->getManager()->getManagedHosts().find("testVehicle0");
            bool r = (i != mobility->getManager()->getManagedHosts().end());
            assertTrue("(addVehicle) vehicle now driving", r);
            const cModule* mod = i->second;
            const TraCIMobility* traci2 = FindModule<TraCIMobility*>::findSubModule(const_cast<cModule*>(mod));
            assertTrue("(addVehicle) vehicle driving at speed", traci2->getSpeed() > 25);
        }
    }

    if (testNumber == testCounter++) {
        if (t == 1) {
            assertEqual("(Vehicle::getTypeId) is vtype0", traciVehicle->getTypeId(), "vtype0");
        }
    }*/
}


int TraCIAZApp::getAzId(Coord vehiclePos){
    //Send coordinates and return the ID da Anchor Zone where the vehicle stay
    return 0;
}
