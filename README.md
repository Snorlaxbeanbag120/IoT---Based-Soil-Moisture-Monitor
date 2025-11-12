**# IoT---Based-Soil-Moisture-Monitor**

IoT-based Soil Moisture and Environmental Monitoring System with Remote Control
 
**Motivation**

Efficient water management is essential for healthy plants and environmental conservation. Many watering methods are inefficient, wasting water or failing to respond to actual soil needs. This project aims to create a smart, remotely accessible system to monitor soil moisture and environmental conditions, helping optimize plant care.
 
**Introduction**

This project demonstrates an IoT prototype using an ESP8266 microcontroller with soil moisture and temperature/humidity sensors. Data is sent to Firebase Realtime Database for storage and made accessible via MQTT messaging. Remote control of actuators (LED and buzzer) is achieved through a mobile MQTT client app, allowing real-time alerts and device management over the internet.
 
**Problem Statement**

Traditional plant watering systems lack feedback mechanisms and remote accessibility, often leading to inefficient watering. The challenge is to develop a real-time monitoring system that alerts users to soil moisture conditions and allows remote control of warning devices.
 
**Architecture / Block Diagram**

![image](https://github.com/user-attachments/assets/aed65fa3-a20a-42ee-b264-e987ac5da9c5)


  
**Hardware Components: **

•	ESP8266 microcontroller
•	Soil moisture sensor (analog)
•	DHT11 temperature and humidity sensor
•	LED with resistor
•	Buzzer
•	Push button for buzzer silence

**Software Components:**

•	Arduino IDE with ESP8266 board libraries
•	Firebase ESP Client library
•	PubSubClient MQTT library
•	Mobile MQTT Client App (MyMQTT) 
 

**Circuit Diagram**

•	Soil moisture sensor → A0
•	DHT11 → D3
•	LED → D5 (GPIO14) through resistor
•	Buzzer → D6 (GPIO12)
•	Push button → D2 (GPIO4) with pull-up


 <img width="399" height="181" alt="image" src="https://github.com/user-attachments/assets/02d6a74c-d361-445c-acdb-c2a69260f03f" />

**Flowchart**

•	Initialize WiFi, MQTT, Firebase, sensors, and actuators
•	Continuously read sensor values
•	Check soil moisture threshold
•	Control LED and buzzer accordingly
•	Upload sensor data to Firebase every second
•	Publish data to MQTT topics
•	Listen for MQTT messages to control LED/buzzer remotely
•	Silence buzzer on button press
 
**Implementation and Results**

•	The device successfully connects to WiFi and Firebase Realtime Database, uploading sensor data every second.
•	Sensor readings for soil moisture, temperature, and humidity were reliably captured and stored in the cloud.
•	The MQTT communication setup was implemented to allow remote control of the LED and buzzer actuators via a mobile MQTT client app.
•	However, due to poor internet connectivity during testing, the control commands to turn the LED and buzzer on or off were not consistently received or executed by the device. This limited the demonstration of remote actuation functionality.
•	Despite this, the system’s architecture supports real-time remote control, and with a stable internet connection, the MQTT-based control would operate smoothly and responsively.
 
**Firebase realtime database results**

<img width="337" height="110" alt="image" src="https://github.com/user-attachments/assets/18bfe7cf-7d96-4961-b164-0001c45716b0" />

**Photo of Prototype**

 ![image](https://github.com/user-attachments/assets/e7759e57-6673-4ed8-be99-9199ea0a5ffd)
 
**Screenshot from MyMQTT app **

![image](https://github.com/user-attachments/assets/c8fccb0c-f5ef-49b3-a0dd-20a085b31322)

 
**Serial monitor output showing sensor data readings**

![image](https://github.com/user-attachments/assets/1b743d0f-21d6-45b2-b8af-f3e0e131345b)

 
**Conclusion**

The IoT prototype demonstrates an effective integration of environmental sensors with cloud data storage and an MQTT-based communication protocol. While sensor data was reliably monitored and uploaded to Firebase, remote control of actuators via MQTT was hindered by poor internet connectivity during testing. This connectivity issue impacted the real-time responsiveness of the LED and buzzer controls. With a more stable and faster internet connection, the system is expected to perform as intended, providing seamless remote monitoring and control. Future improvements could focus on enhancing network reliability and extending actuator control features for automation.
 
**References**

•	Firebase ESP Client library: https://github.com/mobizt/Firebase-ESP-Client
•	PubSubClient MQTT library: https://github.com/knolleary/pubsubclient
•	ESP8266 Arduino core: https://github.com/esp8266/Arduino
•	MQTT protocol documentation: http://mqtt.org/
•	DHT11 sensor datasheet and Arduino library

