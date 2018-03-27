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

package org.car2x.veins.subprojects.anchor_zone;

import org.car2x.veins.base.modules.IBaseApplLayer;

simple TraCIAZApp like IBaseApplLayer
{
	parameters:
		bool debug = default(false);  // output debugging information
		int headerLength @unit("bit") = default (0bit);  // length of the application message header (in bits)
		//int testNumber;  // number of test to run (-1: no test)
        int segundo;
		@display("i=block/app");
		@class("Veins::TraCIAZApp");
	gates:
		input lowerLayerIn; // from network layer
		output lowerLayerOut; // to network layer
		input lowerControlIn; // control from network layer
		output lowerControlOut; // control to network layer
}
