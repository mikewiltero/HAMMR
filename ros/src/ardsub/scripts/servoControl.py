#!/usr/bin/env python
import rospy
import sys
import maestro
from std_msgs.msg import UInt32

def callback(data):
    
    sys.stdout.write("\r Potentiometer Value: %d \x1b[K" % data.data)
    sys.stdout.write("\thello %d" % data.data)
    sys.stdout.flush()
    
def listener():

    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("potval", UInt32, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()