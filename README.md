# Kidnapped-Vehicle-Project
Self-Driving Car Engineer Nanodegree Program

## Overview
A particle filter is implemented in this project. The initial location is given by the GPS-like data. At each time step, particel filter receive noisy observation data and control data and update all the particles' weights and choose heightest waeight particle's X, Y, Wheel then send back to simulator for visulization.

## Prerequisites

+ cmake >= 3.5
+ make >= 4.1
+ gcc/g++ >= 5.4
+ [Udacity's simulator](https://github.com/udacity/self-driving-car-sim/releases)

When you developed in Ubuntu 16.04, you should run [install-ubuntu.sh](https://github.com/wuqianliang/Kidnapped-Vehicle-Project/blob/master/install-ubuntu.sh) script to install uWebsocket and other required packages.

## Basic Build Instructions
+ Clone this repo and cd directory which include CMakeLists.txt
+ Sh ./clean.sh
+ Sh ./build.sh
+ Sh ./run.sh
+ Start simulator , choose kidnapped vehicle and click start


Following is an image of simulator:

After about 1000 timestep the image of simulator:

![Alt text](https://github.com/wuqianliang/Kidnapped-Vehicle-Project/blob/master/images/dataset1-1000.png "Optional title")


# Rubric points
### Does your particle filter localize the vehicle to within the desired accuracy?
Yes, via the output below, we can see that the accuracy is satisfied.

![Alt text](https://github.com/wuqianliang/Kidnapped-Vehicle-Project/blob/master/images/dataset1-accuracy.png "Optional title")

### Does your particle run within the specified time of 100 seconds?
Maybe my laptop cpu is week, but my code was optimized already.So would you please check the speed for me again,thanks.

### Does your code use a particle filter to localize the robot?
Yes, my code follows the flow in the following diagram：

![Alt text](https://github.com/wuqianliang/Kidnapped-Vehicle-Project/blob/master/images/flowchat.png "Optional title")


#### Reference:

https://github.com/JunshengFu/kidnapped-vehicle

https://github.com/jeremy-shannon/CarND-Kidnapped-Vehicle-Project
