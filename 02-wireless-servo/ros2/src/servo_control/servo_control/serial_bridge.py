#!/usr/bin/env python3

import rclpy
import serial
import socket
import time
from rclpy.node import Node
from std_msgs.msg import Int64

class serialBridge(Node):
    def __init__(self):
        super().__init__("serial_bridge")
        self.subscriber_ = self.create_subscription(Int64, "motor_pos", self.write_to_serial, 10)
        self.esp32 = serial.serial_for_url('socket://192.168.1.160:3333', timeout=1)
        self.esp32._socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1) 
        self.get_logger().info("Serial bridge has been started")

    def write_to_serial(self, msg):

        command = str(msg.data)

        self.esp32.write((command + '\n').encode())
        print("sent:", command)

def main(args=None):
    rclpy.init(args=args)
    node = serialBridge()
    rclpy.spin(node)
    rclpy.shutdown()
  
if __name__ == "__main__":
  main()
