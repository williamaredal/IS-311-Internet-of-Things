#include <stdio.h>
#include <stdlib.h>


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <WiFiS3.h>  
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


const int microphonePin = A0; // Analog pin where the microphone is connected

const char* ssid = "";      // your Wi-Fi network SSID (network name)
const char* password = "";  // your Wi-Fi password
const char* server_ip = "";  // use ifconfig to identify mac ip by finding 'en0', and its 'inet' ip address 
const int server_port = 12345;            // the port number

WiFiClient client;  // Create a WiFiClient object

Adafruit_MPU6050 mpu;

SoftwareSerial gpsSerial(2, 3); // RXD pin 2, TXD pin 3
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  gpsSerial.begin(9600); // GPS module communication

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Serial.println("Loop");
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isUpdated()) {

        Serial.println("------------------");
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters(), 6);
        Serial.print("Course: ");
        Serial.println(gps.course.deg(), 6);
        Serial.print("Speed: ");
        Serial.println(gps.speed.kmph(), 6);
        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value(), 6);

         if (gps.date.isValid() && gps.time.isValid()) {
          Serial.print("Date: ");
          Serial.print(gps.date.year());
          Serial.print("-");
          Serial.print(gps.date.month());
          Serial.print("-");
          Serial.print(gps.date.day());
          Serial.print(" Time: ");
          Serial.print(gps.time.hour());
          Serial.print(":");
          Serial.print(gps.time.minute());
          Serial.print(":");
          Serial.print(gps.time.second());
          Serial.println();
        }
       

        /* Get new sensor events with the readings */
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        /* Print out the readings */
        Serial.print("Acceleration X: ");
        Serial.print(a.acceleration.x);
        Serial.print(", Y: ");
        Serial.print(a.acceleration.y);
        Serial.print(", Z: ");
        Serial.print(a.acceleration.z);
        Serial.println(" m/s^2");
        Serial.print("Rotation X: ");
        Serial.print(g.gyro.x);
        Serial.print(", Y: ");
        Serial.print(g.gyro.y);
        Serial.print(", Z: ");
        Serial.print(g.gyro.z);
        Serial.println(" rad/s");
        Serial.print("Temperature: ");
        Serial.print(temp.temperature);
        Serial.println(" degC");


        // Read the analog value from the microphone
        float microphoneValue = analogRead(microphonePin);

        // Print the microphone value to the serial monitor
        Serial.print("Microphone: ");
        Serial.println(microphoneValue);

        if (client.connect(server_ip, server_port)) {
          Serial.println("Connected to server");
          // client.write("hello");  // Send the message to the server

          // Convert the floats to byte arrays
          byte x_bytes[sizeof(float)];
          byte y_bytes[sizeof(float)];
          byte z_bytes[sizeof(float)];
          byte gyro_x_bytes[sizeof(float)];
          byte gyro_y_bytes[sizeof(float)];
          byte gyro_z_bytes[sizeof(float)];
          byte temp_bytes[sizeof(float)];

          memcpy(x_bytes, &a.acceleration.x, sizeof(float));
          memcpy(y_bytes, &a.acceleration.y, sizeof(float));
          memcpy(z_bytes, &a.acceleration.z, sizeof(float));
          memcpy(gyro_x_bytes, &g.gyro.x, sizeof(float));
          memcpy(gyro_y_bytes, &g.gyro.y, sizeof(float));
          memcpy(gyro_z_bytes, &g.gyro.z, sizeof(float));
          memcpy(temp_bytes, &temp.temperature, sizeof(float));

          // Send the byte arrays to the server
          client.write(x_bytes, sizeof(float));
          client.write(y_bytes, sizeof(float));
          client.write(z_bytes, sizeof(float));
          client.write(gyro_x_bytes, sizeof(float));
          client.write(gyro_y_bytes, sizeof(float));
          client.write(gyro_z_bytes, sizeof(float));
          client.write(temp_bytes, sizeof(float));


          byte mic_bytes[sizeof(float)];

          memcpy(mic_bytes, &microphoneValue, sizeof(float));

          // Send the byte arrays to the server
          client.write(mic_bytes, sizeof(float));

          // Store the values in variables
          double latitude = gps.location.lat();
          double longitude = gps.location.lng();
          double altitude = gps.altitude.meters();
          double course = gps.course.deg();
          double speed = gps.speed.kmph();
          uint32_t satellites = gps.satellites.value();

          // Convert the variables to byte arrays
          byte lat_bytes[sizeof(double)];
          byte long_bytes[sizeof(double)];
          byte alt_bytes[sizeof(double)];
          byte course_bytes[sizeof(double)];
          byte speed_bytes[sizeof(double)];
          byte sat_bytes[sizeof(uint32_t)];

          memcpy(lat_bytes, &latitude, sizeof(double));
          memcpy(long_bytes, &longitude, sizeof(double));
          memcpy(alt_bytes, &altitude, sizeof(double));
          memcpy(course_bytes, &course, sizeof(double));
          memcpy(speed_bytes, &speed, sizeof(double));
          memcpy(sat_bytes, &satellites, sizeof(uint32_t));


          // Send the byte arrays to the server
          client.write(lat_bytes, sizeof(double));
          client.write(long_bytes, sizeof(double));
          client.write(alt_bytes, sizeof(double));
          client.write(course_bytes, sizeof(double));
          client.write(speed_bytes, sizeof(double));
          client.write(sat_bytes, sizeof(uint32_t));

          client.stop();  // Disconnect from the server
          // delay(10);
        } else {
          Serial.println("Connection failed");
          delay(5000);    // Wait for 5 seconds before retrying
        }

      }
    }
  }
}