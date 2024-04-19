# ESP32 MQTT Temperature and Humidity Monitoring

This project utilizes an ESP32 board to monitor the environment's temperature and humidity, transmitting this data over MQTT. A Python script running on a Raspberry Pi (or any compatible MQTT client) receives this data for logging and potential further analysis.

## Features

- Real-time temperature and humidity monitoring.
- MQTT protocol for reliable data transmission.
- Data logging on the receiver side.
- Easy configuration and deployment.

## Getting Started

These instructions will help you set up and run the project on your local machine for development and testing purposes.

### Prerequisites

- ESP32 development board.
- DHT11 temperature and humidity sensor.
- Raspberry Pi or any machine to run the Python MQTT client.
- Visual Studio Code with PlatformIO extension.
- Python 3.x with `paho-mqtt` library installed.

### Installation

#### Setting up the ESP32 Sender

1. Connect the DHT11 sensor to your ESP32:
   - **Data Pin to GPIO 4**
2. Load the provided script onto the ESP32 using PlatformIO or via another similar platfrom.

#### Setting up the Python Receiver

1. Ensure Python 3.x is installed on your Raspberry Pi or host machine.
2. Install the `paho-mqtt` library:
   ```bash
   pip install paho-mqtt
3. Run the Python script to start listening for MQTT messages.

### Configuration

- Modify the ssid and password in the ESP32 code to match your WiFi network.
- Ensure the MQTT server address in the ESP32 code matches your MQTT broker's IP address.
- On the Python receiver, ensure the MQTT broker address (localhost by default) is correctly set to point to where your broker is running.

### Usage

Run the ESP32 sender device in an environment you wish to monitor. The Python script on the receiver will log the temperature and humidity data received over MQTT to environment_data.txt.
