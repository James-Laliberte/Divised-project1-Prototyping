#!/usr/bin/env python3


import rclpy
import serial
from rclpy.node import Node
from std_msgs.msg import Int64MultiArray

#Wifi connection to the sensor ESP32
esp32 = serial.serial_for_url("socket://192.168.1.160:3333", timeout=0.1)


def read_from_esp32():

    receivedBytes = esp32.readline()
    receivedText = receivedBytes.decode(errors="ignore").strip()

    if receivedText == "":
        return None

    return receivedText


class MyPublisher(Node):

    def __init__(self):
        super().__init__("Sensor_bridge")

        self.publisher_ = self.create_publisher(Int64MultiArray, "sensor_distances", 10)
        self.timer_ = self.create_timer(0.1, self.publish_message)
        self.counter = 0
        self.get_logger().info("Publisher has been started")

    def publish_message(self):

        receivedText = read_from_esp32()

        if receivedText is None:
            return

        receivedParts = receivedText.split(",")
        if len(receivedParts) != 4:
            return

        distances = []
        for part in receivedParts:
            try:
                distances.append(int(part))
            except ValueError:
                return

        message = Int64MultiArray()
        message.data = distances
        self.publisher_.publish(message)
        self.get_logger().info(f"Published: {distances}")

        self.counter += 1


def main(args=None):
    rclpy.init(args=args)
    node = MyPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()