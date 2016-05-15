#!/usr/bin/env python
import rospy
import sys
from std_msgs.msg import UInt32

def callback(data):
    # don't need loginfo for now
    # rospy.loginfo("I heard %d", data.data)
    #print "Potentiometer Value: %d \r" % data.data
    
    sys.stdout.write("\r Potentiometer Value: %d \x1b[K" % data.data)
    sys.stdout.write("\thello %d" % data.data)
    sys.stdout.flush()
    
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("potval", UInt32, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()