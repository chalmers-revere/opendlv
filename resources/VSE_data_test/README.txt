Here we have some dataset to test the VSE. 
The dataset is coposed of two files:

Z_k_VSE_TEST
U_K_VSE_TEST

U_k contains the measures of the imput commands for the truck in terms of velocity and steering angle

the data are in U_k:
time[sec]  long-velocity[km/h] steering_angle [deg] 

NOTE: the steering_angle is referred to the steering wheel not to the actual wheels of the truck hence it is necessary to convert inside the code using the steer_to_wheel_ratio = 1/18.5;  which adapts steering angle from the steering wheel to wheels angle


Z_k contains the output measures, x, y, heading and heading_dot
the data are in Z_k:
x[m] y[m] theta[deg] theta_dot[deg/s]
