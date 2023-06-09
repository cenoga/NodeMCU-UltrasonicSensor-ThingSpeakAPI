#include <ESP8266WiFi.h>

String apiKey = "33ZK4CPQEY4P3QYI"; // write your "Write API key"
const char* ssid = "GlobeAtHome_192B1_2.4"; // write your "wifi name"
const char* password = "ub56H8Pa"; // write your "wifi password"
const char* server = "api.thingspeak.com";
WiFiClient client;

//for NodeMCU
const int trigPin = D5;
const int echoPin = D6;
// defines variables
long duration;
int distance;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Serial Begin");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); 
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
        
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
}

void loop() 
{
  ultra();
  delay(1000);
  // we have made changes in program to decrease the latency.
  // but it will take minimum 15 second to post data on Thingspeak channel.
    if (client.connect(server,80))
    {
      fwd_to_Thingspeak();
    }
    client.stop();
    Serial.println("Waiting");
    delay(1000);
}

void ultra()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH) ;
  distance= duration*0.034/2;
  Serial.println("Distance =" + String(distance) + " cm");
}

void fwd_to_Thingspeak()
{
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(distance);  // ultrasonic data
  postStr += "\r\n\r\n";
          
  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
          
  Serial.println("Send data to thingspeak: ");
  Serial.print("Content-Length: ");
  Serial.print(postStr.length());
  Serial.print(" Field-1: ");
  Serial.print(distance);  // ultrasonic data
  Serial.println(" data send");            
}
