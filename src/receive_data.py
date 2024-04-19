import paho.mqtt.client as mqtt
import time

# MQTT settings
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC_TEMP = "home/room/temperature"
MQTT_TOPIC_HUM = "home/room/humidity"

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    topic = msg.topic
    message = msg.payload.decode("utf-8")

    try:
        if topic == MQTT_TOPIC_TEMP:
            print(f"Temperature Received: {message}")
            data_type = "Temperature"
        elif topic == MQTT_TOPIC_HUM:
            print(f"Humidity Received: {message}")
            data_type = "Humidity"
        else:
            return  # If the topic isn't one we subscribe to, do nothing

        # Log data to a file
        with open("environment_data.txt", "a") as file:
            file.write(f"{time.ctime()} - {data_type}: {message}\n")

    except Exception as e:
        print(f"Error: {e}")

# Initialize client with the appropriate callback API version using the Enum
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

# Connect to the MQTT broker
try:
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
except Exception as e:
    print(f"Could not connect to MQTT broker: {e}")
    exit(1)

# Set the on_message callback
client.on_message = on_message

# Subscribe to both temperature and humidity topics
try:
    client.subscribe([(MQTT_TOPIC_TEMP, 0), (MQTT_TOPIC_HUM, 0)])
except Exception as e:
    print(f"Failed to subscribe: {e}")
    client.disconnect()
    exit(1)

# Start the network loop in a separate thread
client.loop_start()

# Main loop to keep the script running
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Exiting...")
    client.disconnect()  # Properly disconnect the client
    client.loop_stop()