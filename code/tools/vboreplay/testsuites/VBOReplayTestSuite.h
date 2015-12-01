/**
 * vboreplay - Tool to replay from a VBO file.
 * Copyright (C) 2015 Chalmers ReVeRe
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef VBOREPLAYTESTSUITE_H_
#define VBOREPLAYTESTSUITE_H_

#include "cxxtest/TestSuite.h"

// Include local header files.
#include "../include/VBOReplay.h"

using namespace std;
using namespace core::data;
using namespace revere;

/**
 * This class derives from VBOReplay to allow access to protected methods.
 */
class VBOReplayTestling : public VBOReplay {
    private:
        VBOReplayTestling();
    
    public:
        VBOReplayTestling(const int32_t &argc, char **argv) :
            VBOReplay(argc, argv) {}

        // Here, you need to add all methods which are protected in VBOReplay and which are needed for the test cases.
};

/**
 * The actual testsuite starts here.
 */
class VBOReplayTest : public CxxTest::TestSuite {
    private:
        VBOReplayTestling *dt;

    public:
        /**
         * This method will be called before each testXYZ-method.
         */
        void setUp() {
            // Prepare the data that would be available from commandline.
            string argv0("vboreplay");
            string argv1("--cid=100");
            int32_t argc = 2;
            char **argv;
            argv = new char*[2];
            argv[0] = const_cast<char*>(argv0.c_str());
            argv[1] = const_cast<char*>(argv1.c_str());

            // Create an instance of sensorboard through SensorBoardTestling which will be deleted in tearDown().
            dt = new VBOReplayTestling(argc, argv);
        }

        /**
         * This method will be called after each testXYZ-method.
         */
        void tearDown() {
            delete dt;
            dt = NULL;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Below this line the actual testcases are defined.
        ////////////////////////////////////////////////////////////////////////////////////

        void testVBOReplaySuccessfullyCreated() {
            TS_ASSERT(dt != NULL);
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Below this line the necessary constructor for initializing the pointer variables,
        // and the forbidden copy constructor and assignment operator are declared.
        //
        // These functions are normally not changed.
        ////////////////////////////////////////////////////////////////////////////////////

    public:
        /**
         * This constructor is only necessary to initialize the pointer variable.
         */
        VBOReplayTest() : dt(NULL) {}

    private:
        /**
         * "Forbidden" copy constructor. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the copy constructor.
         *
         * @param obj Reference to an object of this class.
         */
        VBOReplayTest(const VBOReplayTest &/*obj*/);

        /**
         * "Forbidden" assignment operator. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the assignment operator.
         *
         * @param obj Reference to an object of this class.
         * @return Reference to this instance.
         */
        VBOReplayTest& operator=(const VBOReplayTest &/*obj*/);

};

#endif /*VBOREPLAYTESTSUITE_H_*/

