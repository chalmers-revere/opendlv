# Lane Marking Vanishing Point Detection

Detecting the vanishing point of a lane in an image by looking for lane markings, fitting lines
through them and calculating their intersection point.

A sample configuration for the module is given in `resources/configuration`.

### Using Eclipse

To load this project into Eclipse, create a folder for the CMake files outside of the opendlv source
tree. In this folder, run:

    cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_CXX_COMPILER_ARG1=-std=c++11 \
          -DLIBRARIES="opendavinci;opencv_core;opencv_highgui;opencv_imgproc" \
          /PATH_TO_OPENDLV_TREE/code/system/application/legacy/vanishingpoint
          
In Eclipse, select `Import` -> `Existing Projects into Workspace` and select the folder where
you ran CMake.