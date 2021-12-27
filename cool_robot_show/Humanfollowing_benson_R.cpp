#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Int32.h"
#include <vector>

using namespace std;


class HumanFollowingControl {
public:
	HumanFollowingControl();
	~HumanFollowingControl();

//private:
	//=====ros topic=====//
	ros::NodeHandle nh_;
	ros::Publisher vel_pub_R;
	ros::Publisher vel_pub_L;
	ros::Subscriber people_sub_;
	//ros::Publisher mark_array_pub_1;//
	//======function======//
	void get_people(const std_msgs::Float64MultiArray::ConstPtr& people);
	
	void run();
        //=====param========//
	float degree,distance;
};


HumanFollowingControl::HumanFollowingControl()
{
	//=====ros topic=====//
	vel_pub_R= nh_.advertise<std_msgs::Int32>("/motorR", 1);
	vel_pub_L= nh_.advertise<std_msgs::Int32>("/motorL", 1);
  	people_sub_ = nh_.subscribe<std_msgs::Float64MultiArray>("/follow", 10, &HumanFollowingControl::get_people, this);			
	ROS_INFO_STREAM("contrust the human following control class");
}


HumanFollowingControl::~HumanFollowingControl()
{
	ROS_INFO_STREAM("delete the human following control class");
}

void HumanFollowingControl::get_people(const std_msgs::Float64MultiArray::ConstPtr& people)
{
	degree=people->data[0];
	distance=people->data[1];

}

void HumanFollowingControl::run()
{  std_msgs::Int32 cmd_vel_100,cmd_vel_80,cmd_vel_60;
	cmd_vel_100.data=100;
	cmd_vel_80.data=80;
	cmd_vel_60.data=60;
//for distance
	if(distance<10)
	{  	cmd_vel_100.data = cmd_vel_100.data - 30;
		cmd_vel_80.data = cmd_vel_80.data - 30;
		cmd_vel_60.data = cmd_vel_60.data - 30;
	}else if (distance>10&&distance<20)
	{	cmd_vel_100.data=cmd_vel_100.data;
		cmd_vel_80.data=cmd_vel_80.data;
		cmd_vel_60.data=cmd_vel_60.data;
	}else if (distance>20)
	{   cmd_vel_100.data = cmd_vel_100.data + 30;
		cmd_vel_80.data = cmd_vel_80.data + 30;
		cmd_vel_60.data = cmd_vel_60.data + 30;		
	}

//for degree
	if(degree>120&&degree<144){
		vel_pub_R.publish(cmd_vel_60);
		vel_pub_L.publish(cmd_vel_100);
	}
	if(degree>144&&degree<168){
		vel_pub_R.publish(cmd_vel_60);
		vel_pub_L.publish(cmd_vel_80);
	}
	if(degree>168&&degree<192){
		vel_pub_R.publish(cmd_vel_100);
		vel_pub_L.publish(cmd_vel_100);
	}
	if(degree>192&&degree<216){
		vel_pub_R.publish(cmd_vel_80);
		vel_pub_L.publish(cmd_vel_60);
	}
	if(degree>216&&degree<240){
		vel_pub_R.publish(cmd_vel_80);
		vel_pub_L.publish(cmd_vel_100);
	}
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "human_following_control");
	HumanFollowingControl human_following_control;
	ros::Rate rate(30);
	while (ros::ok()) {
		ros::spinOnce();
		human_following_control.run();
		rate.sleep();
	}
	ros::spin();
	return 0;
};



