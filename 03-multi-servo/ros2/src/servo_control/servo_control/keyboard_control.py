#!/usr/bin/env python3
import curses

import rclpy
from rclpy.node import Node
from std_msgs.msg import Int64MultiArray


class KeyboardControl(Node):
    def __init__(self):
        super().__init__("keyboard_control")
        self.publisher_ = self.create_publisher(Int64MultiArray, "servo_positions", 10)
        self.get_logger().info("Motor control has been started")

    def publish_pos(self, motorPositions):
        msg = Int64MultiArray()
        msg.data = motorPositions
        self.publisher_.publish(msg)

def run(screen, node):
    motorPositions = [90, 90, 90, 90]
    stepSize = 5
    keyMap = {
        "q": (0, stepSize), "a": (0, -stepSize),
        "w": (1, stepSize), "s": (1, -stepSize),
        "e": (2, stepSize), "d": (2, -stepSize),
        "r": (3, stepSize), "f": (3, -stepSize),
    }
    screen.keypad(True)
    screen.addstr(0, 0, f"Motor positions: {motorPositions}   ")
    screen.addstr(1, 0, "Q/A W/S E/D R/F to move, X to quit")
    node.publish_pos(motorPositions)

    while True:
        pressedKey = screen.getch()
        pressedCharacter = chr(pressedKey).lower() if 0 <= pressedKey < 256 else ""

        if pressedCharacter == "x":
            break
        if pressedCharacter not in keyMap:
            continue
        servoIndex, change = keyMap[pressedCharacter]
        motorPositions[servoIndex] = max(0, min(180, motorPositions[servoIndex] + change))

        node.publish_pos(motorPositions)
        screen.addstr(0, 0, f"Motor positions: {motorPositions}   ")


def main(args=None):
    rclpy.init(args=args)
    node = KeyboardControl() 
    curses.wrapper(run, node)
    rclpy.shutdown()
  
if __name__ == "__main__":
  main()