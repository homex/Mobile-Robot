#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Float64MultiArray.h"
#include <iostream>
#define RAD2DEG(x) ((x)*180./M_PI)

using namespace std;

class PUBandSUB
{
  private:
    ros::Publisher pub;
    ros::Subscriber sub;
    ros::NodeHandle n;
    
  public:
    std_msgs::Float64MultiArray A;
    float B[2];
    sensor_msgs::LaserScan scan_msg;
    PUBandSUB()
    {
	  sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, &PUBandSUB::scanCallback,this);
    pub = n.advertise<std_msgs::Float64MultiArray>("/follow", 1000);//std_msgs::Float64MultiArraysensor_msgs::LaserScan
    }

  void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
  {    
      float lidar_data[360];
      float ratio_data[360];
      int scandegree = 120;
      int k = 8;
      int a = 0;
      float f = 8.0; 
      float sum = 0;
      float ratio = 0;
      int count = scan->scan_time / scan->time_increment;
      ROS_INFO("I heard a laser scan %s[%d]:", scan->header.frame_id.c_str(), count);
      ROS_INFO("angle_range, %f, %f", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
  
      for(int i = 0; i < count; i++){
          float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
          ROS_INFO(": [%f, %f]", degree, scan->ranges[i]);
          lidar_data[i]=scan->ranges[i];
          
          if(lidar_data[i] >= 5.0){
            lidar_data[i] = 1.0/0.0;
          }
        
          if(scan->ranges[i]<0.60 && !(isinf(scan->ranges[i]))){
            ratio_data[i] = 1;
          }
        
          else{
            ratio_data[i] = 0;
          }
      }

      while(1){
      
        for(int j = 0; j < k; j++){
          a = scandegree + j;
          //ROS_ERROR("a=%d",a);
          sum = sum + ratio_data[a];
        }
        //ROS_ERROR("sum=%f",sum);
        ratio = sum/f;
        sum = 0;
        //ROS_INFO("ratio = %f",ratio);  
          
        if(ratio >= 0.5&&lidar_data[a]<0.65){
          B[0] = a;
          B[1] = lidar_data[a];
          A.data.clear();
          A.data.push_back(a);
          A.data.push_back(lidar_data[a]);
          //scan_msg.range_min = a;
          //scan_msg.range_max = lidar_data[a];
          pub.publish(A);//scan_msg
          ROS_WARN("=============ratio = %f==============%d", ratio, scandegree);
        }
        //else{
        //  ROS_ERROR("============ratio is %f===============", ratio);
       // }
        scandegree = scandegree + 5;
      
        if(scandegree > 240){
          ROS_ERROR("Degree > 240");
          break;
        }
      }
  }

};
int main(int argc, char **argv)
{
    
    ros::init(argc, argv, "rplidar_node_client");
    PUBandSUB SAPObject;
    
    ros::spin();
    
    return 0;
}

