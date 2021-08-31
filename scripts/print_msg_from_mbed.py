#! /usr/bin/env python

import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Int32MultiArray
from std_msgs.msg import String


class Print_Msg_From_Mbed():
    def __init__(self):
        self.float_sub = rospy.Subscriber("/Serial_pub_float", Float32MultiArray, self.Floatcallback)
        self.int_sub = rospy.Subscriber("/Serial_pub_int", Int32MultiArray, self.Intcallback)
        self.string_sub = rospy.Subscriber("/Serial_pub_string", String, self.Stringcallback)

        rospy.spin()

    def Floatcallback(self, msg):
        print(msg)  

    def Intcallback(self, msg):
        print(msg)
        
    def Stringcallback(self, msg):
        print(msg)  

if __name__ == '__main__':

    try:
        rospy.init_node('print_msg_from_mbed')
        print_msg_from_mbed = Print_Msg_From_Mbed()

    except rospy.ROSInterruptException:
        print("program interrupted before completion")
