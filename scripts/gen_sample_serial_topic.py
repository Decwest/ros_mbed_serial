#! /usr/bin/env python

import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Int32MultiArray
from std_msgs.msg import String

import sys, termios, tty

class Gen_Sample_Serial_Topic():
    def __init__(self):
        self.int_msg = rospy.get_param("~int", 2)
        self.float_msg = rospy.get_param("~float", 1.5)
        self.string_msg = rospy.get_param("~string", "abc")
        self.use_joy = rospy.get_param("~use_joy", False)

        self.float_pub = rospy.Publisher('/Serial_sub_float', Float32MultiArray, queue_size=1)
        self.int_pub = rospy.Publisher('/Serial_sub_int', Int32MultiArray, queue_size=1)
        self.string_pub = rospy.Publisher('/Serial_sub_string', String, queue_size=1)

        if self.use_joy == True:
            self.joy_sub = rospy.Subscriber("/joy", Joy, self.Joycallback)
            rospy.spin()

        else:
            self.settings = termios.tcgetattr(sys.stdin)

            while not rospy.is_shutdown():
                key = self.getKey()
                if (key == '\x03'):        
                    rospy.signal_shutdown('finish')
                    rospy.spin()
                    break
                self.pubMsg(key)

    def __del__(self):
        print("gen_sample_serial_topic finished")
        if self.use_joy == False:
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.settings)
            print("Please press Ctrl+C again if you use launch file")

    def Joycallback(self, msg):
        array = [msg.axes[0], msg.axes[1]]
        array_forPublish = Float32MultiArray(data=array)
        self.float_pub.publish(array_forPublish)
        
        if msg.buttons[2] == 1:
            array = [self.int_msg, self.int_msg]
            array_forPublish = Int32MultiArray(data=array)
            self.int_pub.publish(array_forPublish)
            
        if msg.buttons[1] == 1:
            self.string_pub.publish(self.string_msg)         


    def getKey(self):
        tty.setraw(sys.stdin.fileno())
        key = sys.stdin.read(1)

        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.settings)
        return key

    def pubMsg(self, key):
        if key == 'q':
            array = [self.float_msg, self.float_msg]
            array_forPublish = Float32MultiArray(data=array)
            print("publish float array")
            print(array)
            self.float_pub.publish(array_forPublish)

        elif key == 'w':
            array = [self.int_msg, self.int_msg]
            array_forPublish = Int32MultiArray(data=array)
            print("publish int array")
            print(array)
            self.int_pub.publish(array_forPublish)

        elif key == 'e':
            print("publish string")
            print(self.string_msg)
            self.string_pub.publish(self.string_msg)

        else:
            print("invalid input detected.")
            print("valid input  q: float, w: int, e: string")


if __name__ == '__main__':

    try:
        rospy.init_node('gen_sample_serial_topic')
        print("creating gen_sample_serial_topic")

        gen_sample_serial_topic = Gen_Sample_Serial_Topic()

    except rospy.ROSInterruptException:
        print("program interrupted before completion")
