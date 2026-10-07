#!/usr/bin/env python3
import curses

import rclpy
from rclpy.node import Node
from std_msgs.msg import Int64


class KeyboardControl(Node):
    def __init__(self):
        super().__init__("keyboard_control")
        self.publisher_ = self.create_publisher(Int64, "motor_pos", 10)
        self.get_logger().info("Motor control has been started")

    def publish_pos(self, motorPosition):
        msg = Int64()
        msg.data = motorPosition
        self.publisher_.publish(msg)

def run(screen, node):
    motorPosition = 90
    stepSize = 5
    screen.keypad(True)
    screen.addstr(0, 0, f"Motor position: {motorPosition} ")

    while True:
        pressedKey = screen.getch()

        if pressedKey == curses.KEY_RIGHT:
            motorPosition = motorPosition + stepSize
        elif pressedKey == curses.KEY_LEFT:
            motorPosition = motorPosition - stepSize
        elif pressedKey == ord("q"):
            break
            
        motorPosition = max(0, min(180, motorPosition))

        node.publish_pos(motorPosition)
        screen.addstr(0, 0, f"Motor position: {motorPosition} ")


def main(args=None):
    rclpy.init(args=args)
    node = KeyboardControl() 
    curses.wrapper(run, node)
    rclpy.shutdown()
  
if __name__ == "__main__":
  main()

