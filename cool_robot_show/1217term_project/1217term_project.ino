#include <PID_v1.h>
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Empty.h>
std_msgs::Int32 str_msg;

const byte encoder0pinA = 2;//A pin -> the interrupt pin 2
const byte encoder0pinB = 4;//B pin -> the digital pin 4
const byte encoder0pinC = 3;//A pin -> the interrupt pin 3
const byte encoder0pinD = 7;//B pin -> the digital pin 7

//Ports de commande du moteur Left
int motorPinL1 = 8; //8 pin to set H BRIDGE of L298N (IN1)
int motorPinL2 = 9; //9 pin to set H BRIDGE of L298N (IN2)
const int enablePinL= 5; //5 //  Commande de vitesse moteur, to Output ENA pour L298 the second
//Ports de commande du moteur Right
int motorPinR1 = 12; //10 pin to set H BRIDGE of L298N (IN3)
int motorPinR2 = 13; //11 pin to set H BRIDGE of L298N (IN4)
const int enablePinR= 6; //6 //  Commande de vitesse moteur, to Output ENA pour L298 the second
///////////////////////////////////////////////////////////////////////////////////////////////////////
int right, left;
ros::NodeHandle  nh;
ros::Publisher people("people", &str_msg);
//////////////////////////////////////////////////////////////////////////////////////////////////////
void motorR( const std_msgs::Int32& motorR_msg){
  right = motorR_msg.data;
}
void motorL( const std_msgs::Int32& motorL_msg){
  left = motorL_msg.data;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
ros::Subscriber<std_msgs::Int32> sub("motorR", &motorR );
ros::Subscriber<std_msgs::Int32> sub2("motorL", &motorL );
/////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{  
   nh.initNode();
   nh.advertise(people);
   nh.subscribe(sub);
   nh.subscribe(sub2);
   nh.getHardware()->setBaud(57600);
   
   pinMode(motorPinL1, OUTPUT);   //L298N Control port settings direction of motor Left (originaly L298P)
   pinMode(motorPinL2, OUTPUT);  //L298N Control port settings direction of motor Left
   pinMode(enablePinL, OUTPUT);  // powerRate to control speed of motor Left
   
   pinMode(motorPinR1, OUTPUT);   //L298N Control port settings direction of motor Right (originaly L298P)
   pinMode(motorPinR2, OUTPUT);  //L298N Control port settings direction of motor Right
   pinMode(enablePinR, OUTPUT);  // powerRate to control speed of motor Right

}

void loop()
{   
  nh.spinOnce();
  Right();
  Left();
  
}
void Right()//Motor Forward
{
     digitalWrite(motorPinR1,HIGH);
     digitalWrite(motorPinR2,LOW);

     analogWrite(enablePinR,right); 

}
void Left()//Motor reverse
{
     digitalWrite(motorPinL1,HIGH);
     digitalWrite(motorPinL2,LOW);

     analogWrite(enablePinL,left);

}
