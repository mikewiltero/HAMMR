/*
 * rosserial Publisher Example
 * Prints "hello world!"
 * 
 * its working now. It always was. i'm just stupid and didn't use rostopic echo chatter :|
 */

#include <ros.h>
//#include <std_msgs/String.h>
//#include <std_msgs/Float32.h>
#include <std_msgs/UInt32.h>

ros::NodeHandle nh;

std_msgs::UInt32 potval_msg;

// this instantiates the publisher with a topic "chatter" and a reference
// to the information to publish
ros::Publisher potval("potval", &potval_msg);

int potvalRead = 0;

void setup()
{
  nh.initNode();
  nh.advertise(potval);
}

void loop()
{
  potvalRead = analogRead(A0);
  potval_msg.data = potvalRead;
  potval.publish( &potval_msg );
  nh.spinOnce();
  delay(1000);
}
