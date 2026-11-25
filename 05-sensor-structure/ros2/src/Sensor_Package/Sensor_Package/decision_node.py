import rclpy
from rclpy.node import Node
from std_msgs.msg import Int64MultiArray


class DecisionNode(Node):

    def __init__(self):
        super().__init__("decision_node")

        self.subscriber_ = self.create_subscription(
            Int64MultiArray, "sensor_distances", self.process_message, 10)

        self.publisher_ = self.create_publisher(Int64MultiArray, "servo_positions", 10)

        self.messageCount = 0
        self.triggerDistance = 100 

        self.get_logger().info("Decision node has been started")

    def process_message(self, message):

        receivedValue = list(message.data)

        outputValue = []
        for distance in receivedValue:
            if distance != -1 and distance < self.triggerDistance:
                outputValue.append(180)
            else:
                outputValue.append(0)

        outputMessage = Int64MultiArray()
        outputMessage.data = outputValue

        self.publisher_.publish(outputMessage)
        self.get_logger().info(f"Received: {receivedValue}  Sent: {outputValue}")

        self.messageCount += 1


def main(args=None):
    rclpy.init(args=args)
    node = DecisionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()