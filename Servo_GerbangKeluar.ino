#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

const char* ssid = "XXXX";  //SSID Wifi 
const char* password = "XXXX"; //"elektropetra";  //Password Wifi
const char* mqtt_server = "192.168.xxx.xxx";  // Alamat Broker

int servo_pin = D0 ;
//int BUTTON_PIN = D2;
Servo myservo;  // Variable untuk kontrol servo 
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
   // koneksi pada jaringan WIFI
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  Serial.print(" Rotation angle is:");
  Serial.print("   ");
  int p =(char)payload[0]-'0';
  // if MQTT=0, servo mati di pin D1
//  if(p==0 && BUTTON_PIN==LOW) 
  if(p==0 )
  {
    myservo.attach(D0);
    myservo.write(0);
    Serial.println(" Servo Close! ");
  } 
  // if MQTT=1, Servo aktif di pin D1
//  if(p==1 && BUTTON_PIN==HIGH)
  if(p==1)
  {
//    digitalWrite(myservo, ); 
    myservo.attach(D1);
    myservo.write(90);
    Serial.println(" Servo Open! ");
  }
    Serial.println();
}
//end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    
    // Create Name ClientId
    String clientId = "Gerbang Masuk-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("yuhu"); //Topik MQTT yang di Subscribe
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // delay 5 detik
      delay(5000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myservo.attach(D0);  // pin output
//  pinMode(BUTTON_PIN,INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
