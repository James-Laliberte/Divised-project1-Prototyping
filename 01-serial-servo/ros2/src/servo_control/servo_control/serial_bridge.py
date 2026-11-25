#!/usr/bin/env python3

import rclpy
import serial
import time
from rclpy.node import Node
from std_msgs.msg import Int64

class serialBridge(Node):
    def __init__(self):
        super().__init__("serial_bridge")
        self.subscriber_ = self.create_subscription(Int64, "motor_pos", self.write_to_serial, 10)
        self.esp32 = serial.Serial('/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0', 9600, timeout=1)  
        time.sleep(2)
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