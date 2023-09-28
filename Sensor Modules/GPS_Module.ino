/*
  Continue the setup on:
  https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/
 */
 
#include <SoftwareSerial.h>

// The serial connection to the GPS module
SoftwareSerial ss(4, 3);

// Variables to store GPS data
float latitude;
float longitude;
float velocity;
int satelliteCount;

void setup(){
  Serial.begin(9600);
  ss.begin(9600);
}

void loop(){
  Serial.println("loop");
  while (ss.available() > 0){
    // Get the byte data from the GPS
    char gpsData = ss.read();
    // Accumulate NMEA sentences until a newline character is received
    static String sentenceBuffer = "";
    if (gpsData != '\n'){
      sentenceBuffer += gpsData;
    } else {
      // Process the NMEA sentence when a newline is received
      parseNMEASentence(sentenceBuffer);
      sentenceBuffer = "";
    }
  }
}



void parseNMEASentence(String sentence){
  // Split the NMEA sentence into individual data fields
  String fields[15];  // Adjust the array size as needed
  int fieldCount = 0;
  int fieldStart = 0;
  for (int i = 0; i < sentence.length(); i++) {
    if (sentence.charAt(i) == ',' || sentence.charAt(i) == '*') {
      fields[fieldCount++] = sentence.substring(fieldStart, i);
      fieldStart = i + 1;
    }
  }

  // Check the NMEA sentence type
  if (sentence.startsWith("$GNRMC")) {
    // Recommended Minimum Navigation Information
    // Extract latitude, longitude, and velocity
    if (fieldCount >= 10) {
      latitude = fields[3].toFloat();
      longitude = fields[5].toFloat();
      velocity = fields[7].toFloat();
    }
  } else if (sentence.startsWith("$GNGGA")) {
    // Global Positioning System Fix Data
    // Extract satellite count
    if (fieldCount >= 7) {
      satelliteCount = fields[7].toInt();
    }
  }

  // You can add similar if-else blocks for other sentence types as needed

  // Print the parsed data
  Serial.print("Latitude: ");
  Serial.println(latitude, 6); // 6 decimal places for precision
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.print("Velocity: ");
  Serial.println(velocity, 2); // 2 decimal places for velocity
  Serial.print("Satellite Count: ");
  Serial.println(satelliteCount);
}