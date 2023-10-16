#include <stdio.h>
#include <stdlib.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <WiFiS3.h>  
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const char* ssid = "";      // your Wi-Fi network SSID (network name)
const char* password = "";  // your Wi-Fi password
const char* server_ip = "";  // use ifconfig to identify mac ip by finding 'en0', and its 'inet' ip address 
const int server_port = 12345;            // the port number

const int microphonePin = A0; // Analog pin where the microphone is connected
int microphoneValue;

WiFiClient client;  // Create a WiFiClient object

Adafruit_MPU6050 mpu;

SoftwareSerial gpsSerial(2, 3); // RXD pin 2, TXD pin 3
TinyGPSPlus gps;

sensors_event_t a, g, temp;

// Calibrated offsets (replace with your own values)
float accelXOffset = 0.2;
float accelYOffset = -0.17;
float accelZOffset = 4.92335;

float gyroXOffset = -0.08;
float gyroYOffset = 0.00;
float gyroZOffset = -0.04;

float calibratedAccelX;
float calibratedAccelY;
float calibratedAccelZ;

float calibratedGyroX;
float calibratedGyroY;
float calibratedGyroZ;


// Declare global variables for GPS values
double latitude = 0.0;
double longitude = 0.0;
double altitude = 0.0;
double course = 0.0;
double speed = 0.0;
int satellites = 0;

bool areValidValues = false;


void setup() {
  Serial.begin(115200); // Initialize serial communication
  initMpu();
  initGps();
  initWifi();

}

void initMpu() {
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
}

void initGps() {
  gpsSerial.begin(9600); // GPS module communication
}

void initWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {

  Serial.println("-----------------------------");

  getMpu();
  getMic();
  getGps();

  if (areValidValues) {
    // commented out 'printValues()', only required for debugging
    //printValues();
    sendData();
  }

  areValidValues = 0;

  delay(100);

       
}

void printValues() {
  // Print the calibrated mpu values
  Serial.print("Calibrated Acceleration X: ");
  Serial.print(calibratedAccelX);
  Serial.print(", Y: ");
  Serial.print(calibratedAccelY);
  Serial.print(", Z: ");
  Serial.print(calibratedAccelZ);
  Serial.println(" m/s^2");

  Serial.print("Calibrated Rotation X: ");
  Serial.print(calibratedGyroX);
  Serial.print(", Y: ");
  Serial.print(calibratedGyroY);
  Serial.print(", Z: ");
  Serial.print(calibratedGyroZ);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  // Print the mic values
  Serial.print("Microphone: ");
  Serial.println(microphoneValue);

  // Print the gps values 
  Serial.print("Latitude: ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.print("Altitude: ");
  Serial.println(altitude, 6);
  Serial.print("Course: ");
  Serial.println(course, 6);
  Serial.print("Speed: ");
  Serial.println(speed, 6);
  Serial.print("Satellites: ");
  Serial.println(satellites, 6);
  Serial.print("Valid?: ");
  Serial.println(areValidValues);
}

void getMic() {
  int mn = 1024;
  int mx = 0;

  for (int i = 0; i < 10; ++i) {
    int val = analogRead(microphonePin);
    mn = min(mn, val);
    mx = max(mx, val);
  }

  int delta = mx - mn;
  microphoneValue = delta;
}

void getMpu() {
  mpu.getEvent(&a, &g, &temp);

  // Apply offset compensation and create variables for calibrated values
  calibratedAccelX = a.acceleration.x - accelXOffset;
  calibratedAccelY = a.acceleration.y - accelYOffset;
  calibratedAccelZ = a.acceleration.z - accelZOffset;

  calibratedGyroX = g.gyro.x - gyroXOffset;
  calibratedGyroY = g.gyro.y - gyroYOffset;
  calibratedGyroZ = g.gyro.z - gyroZOffset;
}

void getGps() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      
      areValidValues = gps.location.isValid() && gps.course.isValid() && gps.altitude.isValid() && gps.speed.isValid() && gps.satellites.isValid();

      if (areValidValues) {

        // Update global variables with GPS data
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        altitude = gps.altitude.meters();
        course = gps.course.deg();
        speed = gps.speed.kmph();
        satellites = gps.satellites.value();

        break;
      }
    }
  }
}

// Function to send a variable as bytes to the server
template <typename T>
void sendVariableAsBytes(Client &client, T &variable) {
  byte bytes[sizeof(T)];
  memcpy(bytes, &variable, sizeof(T));
  client.write(bytes, sizeof(T));
}

void sendData() {
  if (client.connect(server_ip, server_port)) {
    Serial.println("Connected to server");

    // Send accelerometer data
    sendVariableAsBytes(client, calibratedAccelX);
    sendVariableAsBytes(client, calibratedAccelY);
    sendVariableAsBytes(client, calibratedAccelZ);

    // Send gyroscope data
    sendVariableAsBytes(client, calibratedGyroX);
    sendVariableAsBytes(client, calibratedGyroY);
    sendVariableAsBytes(client, calibratedGyroZ);

    // Send temperature data
    sendVariableAsBytes(client, temp.temperature);

    // Send microphone data
    sendVariableAsBytes(client, microphoneValue);

    // Send GPS data
    sendVariableAsBytes(client, latitude);
    sendVariableAsBytes(client, longitude);
    sendVariableAsBytes(client, altitude);
    sendVariableAsBytes(client, course);
    sendVariableAsBytes(client, speed);
    sendVariableAsBytes(client, satellites);

    client.stop();  // Disconnect from the server
  } else {
    Serial.println("Connection failed");
    delay(1000);    // Wait for 1 seconds before retrying
  }
}
