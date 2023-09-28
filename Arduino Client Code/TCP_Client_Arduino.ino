#include <stdio.h>
#include <stdlib.h>


#include <Wire.h>

#include <WiFiS3.h>  

#define MAX 100

const int microphonePin = A0; // Analog pin where the microphone is connected

const char* ssid = "";      // your Wi-Fi network SSID (network name)
const char* password = "";  // your Wi-Fi password
const char* server_ip = "";  // use ifconfig to identify mac ip by finding 'en0', and its 'inet' ip address 
const int server_port = 12345;            // the port number

WiFiClient client;  // Create a WiFiClient object


void setup() {
  Serial.begin(115200); // Initialize serial communication


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {

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
  Serial.println("");


  // Read the analog value from the microphone
  float microphoneValue = analogRead(microphonePin);

  // Print the microphone value to the serial monitor
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

    /*
    memcpy(x_bytes, &a.acceleration.x, sizeof(float));
    memcpy(y_bytes, &a.acceleration.y, sizeof(float));
    memcpy(z_bytes, &a.acceleration.z, sizeof(float));
    memcpy(gyro_x_bytes, &g.gyro.x, sizeof(float));
    memcpy(gyro_y_bytes, &g.gyro.y, sizeof(float));
    memcpy(gyro_z_bytes, &g.gyro.z, sizeof(float));
    memcpy(temp_bytes, &temp.temperature, sizeof(float));
    */
    memcpy(, &g.gyro.x, sizeof(float));
    memcpy(, &g.gyro.y, sizeof(float));
    memcpy(, &g.gyro.z, sizeof(float));
    memcpy(, &temp.temperature, sizeof(float));

    // Send the byte arrays to the server
    /*
    client.write(x_bytes, sizeof(float));
    client.write(y_bytes, sizeof(float));
    client.write(z_bytes, sizeof(float));
    client.write(gyro_x_bytes, sizeof(float));
    client.write(gyro_y_bytes, sizeof(float));
    client.write(gyro_z_bytes, sizeof(float));
    client.write(temp_bytes, sizeof(float));
    */


    byte mic_bytes[sizeof(float)];

    memcpy(mic_bytes, &microphoneValue, sizeof(float));

    // Send the byte arrays to the server
    client.write(mic_bytes, sizeof(float));

    client.stop();  // Disconnect from the server
    delay(100);
  } else {
    Serial.println("Connection failed");
    delay(5000);    // Wait for 5 seconds before retrying
  }
}
