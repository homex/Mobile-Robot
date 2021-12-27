#include "ros/ros.h"
#include <wiringPi.h>
#include <iostream>
//#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
//#include <std_msgs/Float32.h>
#include<unistd.h>
using namespace std;

//light receive pin 3
const short int lightpin = 3;
const short int touchLpin = 12;
const short int touchRpin = 13;
const short int touchDpin = 14;
const short int OSpin = 21;

ros::Time previous_time;
ros::Time current_time;

int main (int argc, char **argv){

ros::init(argc, argv, "sensor_data");

ros::NodeHandle n;

// constructure the topics which receive the "press information" from the touch sensor and publish to the topic "red word" 
//ros::Publisher light_pub = n.advertise<std_msgs::Int16>("light_data", 1);        
//ros::Publisher touchL_pub = n.advertise<std_msgs::Int16>("touchL_data", 1);
//ros::Publisher touchR_pub = n.advertise<std_msgs::Int16>("touchR_data", 1);
//ros::Publisher touchD_pub = n.advertise<std_msgs::Int16>("touchD_data", 1);
//ros::Publisher OS_pub = n.advertise<std_msgs::Int16>("OS_data", 1);
//ros::Publisher door_pub = n.advertise<std_msgs::Int16>("choosed_data", 1);
ros::Publisher light_pub = n.advertise<std_msgs::Int32>("light_data", 1);        
ros::Publisher touchL_pub = n.advertise<std_msgs::Int32>("touchL_data", 1);
ros::Publisher touchR_pub = n.advertise<std_msgs::Int32>("touchR_data", 1);
ros::Publisher touchD_pub = n.advertise<std_msgs::Int32>("touchD_data", 1);
ros::Publisher OS_pub = n.advertise<std_msgs::Int32>("OS_data", 1);
ros::Publisher door_pub = n.advertise<std_msgs::Int32>("choosed_data", 1);


unsigned short int light_rev = 0;
unsigned short int touchL_rev = 0;
unsigned short int touchR_rev = 0;
unsigned short int touchD_rev = 0;
unsigned short int OS_rev = 0;

// set the message type 
//std_msgs::Int16 light_data;
//std_msgs::Int16 touchL_data;
//std_msgs::Int16 touchR_data;
//std_msgs::Int16 touchD_data;
//std_msgs::Int16 OS_data;
//std_msgs::Int16 choosed_data;
std_msgs::Int32 light_data;
std_msgs::Int32 touchL_data;
std_msgs::Int32 touchR_data;
std_msgs::Int32 touchD_data;
std_msgs::Int32 OS_data;
std_msgs::Int32 choosed_data;
//use this command whithout sudo
setenv("WIRINGPI_GPIOMEM", "1", 1);
//library setup function
wiringPiSetup () ;
pinMode (lightpin, INPUT) ;
pinMode (touchLpin, INPUT) ;
pinMode (touchRpin, INPUT) ;
pinMode (touchDpin, INPUT) ;
pinMode (OSpin, INPUT) ;
//10hz
ros::Rate loop_rate(10);


int c;
cout << "Choose door: " << endl;
cin >> c;
//cout << "Choose door: " << c << endl;
choosed_data.data = c; 


while(ros::ok())
{
	light_rev = digitalRead(lightpin) ;
	light_data.data = light_rev;
	touchL_rev = digitalRead(touchLpin) ;
	touchL_data.data = touchL_rev;
	touchR_rev = digitalRead(touchRpin) ;
	touchR_data.data = touchR_rev;
	touchD_rev = digitalRead(touchDpin) ;
	touchD_data.data = touchD_rev;
    
    OS_rev = digitalRead(OSpin) ;
    int count0 = 0;
    //int count1 = 0;
    int i;
    for(i = 0; i < 150; i++)
    {
        //ROS_INFO("i : %d ", i);         
        OS_rev = digitalRead(OSpin);
        if(OS_rev == 0)
            count0 ++;
        usleep(100);
    }
    //int a = count0 * 1000 / 150.0;
    double a = count0 / 150.0;
    OS_data.data = a * 1000;

   	   
    ROS_INFO("count0 : %d ", count0);    
	ROS_INFO("light_receive : %d ", light_rev);
	ROS_INFO("touchL_receive : %d ", touchL_rev);
	ROS_INFO("touchR_receive : %d ", touchR_rev);
	ROS_INFO("touchD_receive : %d ", touchD_rev);
	//ROS_INFO("Ratio : %d ", a);
    ROS_INFO("Ratio : %lf ", a);

	light_pub.publish(light_data);
	touchL_pub.publish(touchL_data);
	touchR_pub.publish(touchR_data);
	touchD_pub.publish(touchD_data);
	OS_pub.publish(OS_data);
	door_pub.publish(choosed_data);

	ros::spinOnce();
	loop_rate.sleep();
}
return 0 ;
}
