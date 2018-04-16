#include <ros/ros.h>
#include <iostream>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>

#define RATE_HZ 2

using namespace std;
float info []= {-1, -1, -1};
int pos = -1;

void get_msg(const std_msgs::Float32MultiArray &msg_sensor) {
	info[0] = msg_sensor.data[0];
	info[1] = msg_sensor.data[1];
	info[2] = msg_sensor.data[2];
	ROS_INFO_STREAM("Humedad: " << info[0]);
	ROS_INFO_STREAM("Tempeteratura: " << info[1]);
	ROS_INFO_STREAM("Luminocidad: " << info[2]);
}
bool criterio(float agua, float temp, float luz){
	bool res = false;
	if(luz < 150 && temp > 25){
		res = true;
	}
	else if(luz < 150 && agua < 50){
		res = true;
	}
	else if (temp > 25 && agua < 50){
	    res = true;
	}
	return res;
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"info_tendedero");
	ros::NodeHandle nh;
	
	ROS_INFO_STREAM("info_tendedero initialized");																																							
	ROS_INFO_STREAM(ros::this_node::getName());

	std_msgs::Int32 ang;

	ros::Subscriber sub_sensor = nh.subscribe("sensor", 100, &get_msg);
	ros::Publisher pub_servo = nh.advertise<std_msgs::Int32> ("servo", 1);

	ros::Rate rate(RATE_HZ);

	 /*bool res = criterio(info[0], info[1], info[2]);
		pos++;
	    if(pos < 360){
	  		if(criterio(temp,l,hum) == false){
	      	ang.data = pos;
	      	pub_servo.publish(ang);
	      	//ros::spinOnce();
	      	//rate.sleep();                                  
	  	}else
	  		//rate.sleep();
	  	}else{
	    pos = 0;
	    ang.data = pos;
	    pub_servo.publish(ang);
	    //servo.write(pos);
	    //rate.sleep();
	  	}*/

	while (ros::ok())
	{

		ROS_INFO_STREAM(".");

		ros::spinOnce();
		// ros::spin();
		rate.sleep();
	}

    return 0;
}