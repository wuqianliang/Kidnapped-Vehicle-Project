# Kidnapped-Vehicle-Project
Self-Driving Car Engineer Nanodegree Program

## Overview
This project implemented an Unscented Kalman Filter for RADAR and LIDAR sensor data. The Udacity simulator generates noisy RADAR and LIDAR measurements of the position and velocity of an object. This project implemented the Extended Kalman Filter  to fusion those measurements to predict the position and velocity of the object. 

## Prerequisites

+ cmake >= 3.5
+ make >= 4.1
+ gcc/g++ >= 5.4
+ [Udacity's simulator](https://github.com/udacity/self-driving-car-sim/releases)

As described in [Udacity seed project](https://github.com/udacity/CarND-Extended-Kalman-Filter-Project), when you developed in Ubuntu 16.04, you should run [install-ubuntu.sh](https://github.com/wuqianliang/CarND-EKF-Project/blob/master/install-ubuntu.sh) script to install uWebsocket and other required packages.

## Basic Build Instructions
+ Clone this repo and cd directory which include CMakeLists.txt
+ cmake .
+ make (This will create UnscentedKF executable) 

## Running Unscented Kalman Filter
After launch the simulator, when executed ./UnscentedKF, output will be:
>     Listening to port 4567
>     Connected!!!

Following is an image of simulator:

After emit dataset1,the green triangle is the predicted postion of object, and the final RMSE:

![Alt text](https://github.com/wuqianliang/CarND-UKF-Project/blob/master/images/dataset1-RMSE.png "Optional title")

After switch dataset2, the final RMSE:

![Alt text](https://github.com/wuqianliang/CarND-UKF-Project/blob/master/images/dataset2-RMSE.png "Optional title")

## NIS checking:

From the following imags,we can see the estimations of uncertainty of noise are Appropriately.

![NIS laser](https://github.com/wuqianliang/CarND-UKF-Project/blob/master/images/nis-laser.png "Optional title")

![NIS radar](https://github.com/wuqianliang/CarND-UKF-Project/blob/master/images/nis-radar.png "Optional title")

# Rubric points
## Accuracy
You can see in the above image,the final RMSE is:

dataset1 : RMSE of \[px,py,vx,vy\] is \[0.0662,0.0827,0.3326,0.2145\]

dataset2 : RMSE of \[px,py,vx,vy\] is \[0.0830,0.0850,0.5149,0.5712\]

## Follows the Correct Algorithm
### Your Sensor Fusion algorithm follows the general processing flow as taught in the preceding lessons.
yes!!

### Your Kalman Filter algorithm handles the first measurements appropriately.
src/ukf.cpp line 102 ~ 130.

### Your Kalman Filter algorithm first predicts then updates.
First predict at line 134 of src/ukf.cpp then update at line 139 and line 142.

### Your Kalman Filter can handle radar and lidar measurements.
Implement measurement update from line 242 ~ line 391 at src/ukf.cpp.

## Code Efficiency
These requirements also are satisfied!


Reference:
https://github.com/YoungGer/Udacity-SelfDrivingCar-NanoDegree/tree/master/CarND-Unscented-Kalman-Filter-Project
