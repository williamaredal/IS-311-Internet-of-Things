#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(2, 3); // RXD pin 2, TXD pin 3
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600); // Serial monitor
  gpsSerial.begin(9600); // GPS module communication
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isUpdated()) {
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
      }
    }
  }
}