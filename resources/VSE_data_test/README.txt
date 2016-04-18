Here we have some dataset to test the VSE (Vehicle State Estimator). 
The dataset is coposed of two files:

Z_k_VSE_TEST
U_K_VSE_TEST

U_k contains the measures of the imput commands for the truck in terms of velocity and steering angle

the data are in U_k:
time[sec]  long-velocity[m/s] steering_angle [rad] 

NOTE: the steering_angle is now referred to the actual steering angle on the axle of the vehicle and NOT the steering wheel. 
In the real truck we will probably get the steering wheel not to the actual angle on the vehicle axle,
hence it is necessary to convert inside the code using the steer_to_wheel_ratio = 1/18.5;  
This value adapts steering angle from the steering wheel to wheels angle


Z_k contains the output measures, x, y, heading and heading_dot
the data are in Z_k:
time[sec] x[m] y[m] theta[rad] theta_dot[rad/s]
