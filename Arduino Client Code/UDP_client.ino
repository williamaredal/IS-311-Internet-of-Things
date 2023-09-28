#include <WiFiS3.h>  

// Upon Verification: Global variables use 4344 bytes (13%) of dynamic memory, leaving 28424 bytes for local variables. Maximum is 32768 bytes.
// So, with each float variable taking up about 4 bytes. 7 x 4 = 28 bytes from variables from MPU6050 will allow 1000 structs with free memory (28424 bytes)
// A sampling of once each 10 miliseconds will then give 100 samples per second, and use around 10% of the total free memory available.
struct SensorsData {
  // MPU6050 sensor data
  float temperature;   ///< Last reading's temperature (C)
  float accX;          ///< Last reading's accelerometer X axis m/s^2
  float accY;          ///< Last reading's accelerometer Y axis m/s^2
  float accZ;          ///< Last reading's accelerometer Z axis m/s^2
  float gyroX;         ///< Last reading's gyro X axis in rad/s
  float gyroY;         ///< Last reading's gyro Y axis in rad/s
  float gyroZ;         ///< Last reading's gyro Z axis in rad/s

  // Microphone sensor data


  // GPS sensor data


  // Temperature/Humidity sensor data

};

const char* ssid = "";      // your Wi-Fi network SSID (network name)
const char* password = "";  // your Wi-Fi password
const char* server_ip = "";  // use ifconfig to identify mac ip by finding 'en0', and its 'inet' ip address 
const int server_port = 12345;            // the port number

WiFiClient client;  // Create a WiFiClient object

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // waits for the wifi to accept the password printing '.' until finished 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {

  if (client.connect(server_ip, server_port)) {
    Serial.println("Connected to server");
    client.write("hello");  // Send the message to the server
    client.stop();  // Disconnect from the server
    delay(3000);    // Wait for 3 seconds before sending again
  } else {
    Serial.println("Connection failed");
    delay(5000);    // Wait for 5 seconds before retrying
  }
}
