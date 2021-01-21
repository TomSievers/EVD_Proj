# Build application

## UNIX
The following packages need to be installed
- curses (sudo apt-get install libncurses5-dev libncursesw5-dev)
- cairo (sudo apt-get install libcairo2-dev)
- libdrm (sudo apt-get install libdrm-dev)
- openCV

### How to install openCV
1. Download opencv
  wget -O opencv.zip https://github.com/opencv/opencv/archive/4.4.0.zip
2. unzip into directory
  unzip opencv.zip -d opencv
3. create build folder
  cd opencv
  mkdir build
  cd build
4. configure and build
  cmake ..
  make -j2
5. install
  sudo make install
  
### Build the application  
To build the application go into the application folder. In this folder create a build folder. Go into this folder and execute the following commands:

> cmake ..

> make -j<n_of_cores>

### Run the application
To run the application go to the previously created build folder and run the pool_aid executable

***

## Windows

The application won't work on a windows application, however the test can be ran on windows.

### Add path variables
The following paths need to be in the path 
* C:\Qt\5.15.2\mingw81_64\bin
* C:\Qt\5.15.2\mingw81_64\lib
* C:\Qt\Tools\CMake_64\bin
* C:\Qt\Tools\mingw810_64\bin
* C:\opencv\include
* C:\opencv\x64\mingw\bin
* C:\lib (This one needs to be created by oneself, is the install dir for libs)

### Build the application
To build the application go into the application folder. In this folder create a build folder. Go into this folder and execute the following commands:
cmake ..
mmingw32-make.exe

## Run the tests
The tests can be found in different places
- To test the UserInterface run the UserInterface_test (found in the UserInterface folder) from the build folder
- To test the TrajectoryCalculator run the trajectory_test (found in the TrajectoryCalculator folder) from the build folder
- To test the CueBallDetector change the add_executable (line 43) in the CMakeLists.txt in the Detector folder to: add_executable(${PROJECT_NAME}_test src/BallDetectorTest.cpp)
and run the detector_test (found in the Detector folder) from the build folder
- To test the CueDetector change the add_executable (line 43) in the CMakeLists.txt from the build folder in the Detector folder to:
add_executable(${PROJECT_NAME}_test src/test.cpp)
and run the detector_test (found in the Detecor folder) from the build folder
- To test the ImageCapture run the image_cap_test (found in the ImageCapture folder) from the build folder 
- To test the ImageDrawer run the ImageDrawer_test (found in the ImageDrawer folder) from the build folder
- To test the Visualizer run the visualizer_test(found in the Vizualization folder) from the build folder
