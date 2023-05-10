//Librerias
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
//Pines
#define PinSensor 35   // Pin digital al que se conecta el sensor
#define Tipo DHT11    // Tipo de sensor
#define PinLed 2
DHT dht(PinSensor, Tipo);

//Configuración WiFi 
//const char *ssid = "Doctorado"; 
//const char *password = "docInge2018$";  
const char *ssid = "Megcable_2.4G_FD48"; 
const char *password = "thTdXdR7";  
WiFiClient espClient;

// mqtt brocker:
const char *mqttBrocker = "192.168.1.205";
const char *topic = "esp32/output";   
const char *topic2 = "esp32/temperature";   
const char *topic3 = "esp32/humidity";   
const char *mqttUsername = "";  //Dejar en blanco para BB
const char *mqttPassword = "";  //Dejar en blanco para BB
const int mqttPort = 1883;
PubSubClient client(espClient);

//Variables
char hum[10];
char tem[10];
float humedad;
float temperatura;
char mensajeOn ="n";
char mensajeOff ="f";

void setup(){
  pinMode(PinLed , OUTPUT)
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Connecting to ");
    Serial.println(ssid); 
  }
  Serial.println("Connection done.");
  //connecting to a mqtt brocker:
  client.setServer(mqttBrocker, mqttPort);
  client.setCallback(callback);
  while(!client.connected()){
    String clientID = "esp32-bere";
    clientID += String(WiFi.macAddress());  //Esta linea se puede quitar
    Serial.printf("The %s tries to connect to mqtt borcker...\n",clientID.c_str());
    if(client.connect(clientID.c_str(), mqttUsername, mqttPassword)){
      Serial.println("mqtt brocker connected");
    }
    else {
      Serial.print("mqtt connection failed");
      Serial.println(client.state());
      delay(2000);
    }
  }
  //once connected publish and suscribe:
  client.publish(topic, "Hi EMQX broker I'm a ESP32 :)");   //Valor ADC
  client.subscribe(topic);

}

void loop(){
  client.loop();
  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();
  sprintf(hum, "%f", humedad);
  sprintf(tem, "%f", temperatura);
  client.publish(topic2, hum);   //Valor ADC
  client.publish(topic3, tem);   //Valor ADC
  Serial.println("Temperatura:");
  Serial.println(temperatura);
  Serial.println("Humedad: ");
  Serial.println(humedad);
  delay(5000);
}

void callback(char *topic, byte *payload, unsigned int length){
  Serial.print("Message recived in topic: ");
  Serial.println(topic);
  Serial.print("The message is: ");
  for(int i=0;i<length; i++){
    Serial.print((char) payload[i]);
    sprintf(mensaje, "%c",payload[i])
    if (mensajeOn[0]==mensaje[i]){
        Serial.print("Encender Led");
        digitalWrite(PinLed, HIGH);
    }
    else if(mensajeOff[0]==mensaje[i]]){
        Serial.print("Apagar Led");
        digitalWrite(PinLed, LOW);
    }
  }
  Serial.println();
  Serial.println("-+-+-+End+-+-+-+");
}
