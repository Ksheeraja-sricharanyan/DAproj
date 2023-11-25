


#include <Arduino_MKRENV.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>


char ssid[] = "project1";
char password[] = "project1";
char writeAPIKey[] = "C2DMST59B0YZYN5Y";  // Replace with your ThingSpeak API Key
unsigned long channel_number =2344466;
WiFiClient client;
void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  while (!Serial);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV Shield!");
    while (1);
  }

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
     Serial.print("Attempting to connect to SSID: ");
     Serial.println(ssid);
     delay(1000);
   }

   Serial.println("Connected to WiFi");
   Serial.print("IP Address: ");
   Serial.println(WiFi.localIP());
}
   


void loop() {
  float temperature = ENV.readTemperature();
  float humidity = ENV.readHumidity();
  float pressure = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva = ENV.readUVA();
  float uvb = ENV.readUVB();
  float uvIndex = ENV.readUVIndex();
  int value = analogRead(A0);
  float temp = (value * 100) / 1024;

  Serial.print("Soil dryness = ");
  Serial.print(temp);
  Serial.println("%");

  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure    = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  Serial.print("Illuminance = ");
  Serial.print(illuminance);
  Serial.println(" lx");

  Serial.print("UVA         = ");
  Serial.println(uva);

  Serial.print("UVB         = ");
  Serial.println(uvb);

  Serial.print("UV Index    = ");
  Serial.println(uvIndex);

  // Update ThingSpeak fields
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, temperature);
  ThingSpeak.setField(3, humidity);
  ThingSpeak.setField(4, pressure);
  ThingSpeak.setField(5, illuminance);
  ThingSpeak.setField(6, uva);
  ThingSpeak.setField(7, uvb);
  ThingSpeak.setField(8, uvIndex);

  int statusCode = ThingSpeak.writeFields(channel_number,writeAPIKey);
  if (statusCode == 200) {
    Serial.println("ThingSpeak update successful");
  } else {
    Serial.println("Error updating ThingSpeak. HTTP status code: " + String(statusCode));
   
  }
  delay(2*60*1000);

  // Wait 15 seconds before updating ThingSpeak again
  
}
