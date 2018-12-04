[![Udacity - Robotics NanoDegree Program](https://s3-us-west-1.amazonaws.com/udacity-robotics/Extra+Images/RoboND_flag.png)](https://www.udacity.com/robotics)

# RoboND-MinkowskiSum
Inflate any obstacle and generate its configuration space using the Minkowski Sum.

### Example
Generating the configuration space of the blue Robot and red Obstacle
![alt text](Images/Minkowski_Sum_Initial.png)

### Compiling
```sh
$ cd ~
$ git clone https://github.com/karimchamaa/RoboND-MinkowskiSum
$ cd RoboND-MinkowskiSum/
$ mkdir build/
$ cd src/
$ g++ minkowski_sum.cpp -o ../build/app -std=c++11 -I/usr/include/python2.7 -lpython2.7
```

### Running
```sh
$ ../build/app
```
Ignore the matplotlib error 

### Generated Configuration Space
This program will first translate the blue robot position to the red obstacle, generate the green configuration space of the blue robot and red obstacle, and translate it to the red obstacle position as follows:
![alt text](Images/Minkowski_Sum_Final.png)

