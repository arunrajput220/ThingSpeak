#include <DHT.h>  // Including library for dht

#include <ESP8266WiFi.h>
 
String apiKey = "API KEY";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "SSID";     // replace with your wifi ssid and wpa2 key
const char *pass =  "Password";
const char* server = "api.thingspeak.com";


#define DHTTYPE DHT11  
#define dht_dpin 4


DHT dht(dht_dpin, DHTTYPE); 

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
const int ldrPin =  5;  //D1
int smokeA0 = A0;
const int IRSensor= 14;  //D5
String fireStatus = "";   
int led_status ;
long duration;
int distance;

WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
       pinMode(echoPin, INPUT); // Sets the echoPin as an 
       dht.begin();
       pinMode(ldrPin,INPUT);
       pinMode(smokeA0, INPUT);
       pinMode(IRSensor,INPUT);
 
       Serial.println("Connecting to ");
       Serial.println(ssid);

 
 
       WiFi.begin(ssid, pass);
 
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
  
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      int dist = ultrasonic();
      int airqua = air_quality();
      int ldr_val = ldr_sensor();
      int ir_value = ir_sensor();

      Serial.print("Humidity : ");
      Serial.print(h);
      Serial.print("\nTemperature : ");
       Serial.print(t);
      Serial.print("\nDistance : ");
       Serial.print(dist);
      Serial.print("\nAir Quality : ");
       Serial.print(airqua);
      Serial.print("\nIDR : ");
       Serial.print(ldr_val);
      Serial.print("\nIR : ");
       Serial.print(ir_value);
      

      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(airqua);
                             postStr +="&field4=";
                             postStr += String(dist);
                             postStr +="&field5=";
                             postStr += String(ir_value);
                             postStr +="&field6=";
                             postStr += String(ldr_val);
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
                          
                             Serial.println("\nDATA Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  
  delay(5000);
}

int   ultrasonic(){
digitalWrite(trigPin, LOW);   // Makes trigPin low
delayMicroseconds(2);       // 2 micro second delay 

digitalWrite(trigPin, HIGH);  // tigPin high
delayMicroseconds(10);      // trigPin high for 10 micro seconds
digitalWrite(trigPin, LOW);   // trigPin low

duration = pulseIn(echoPin, HIGH);   //Read echo pin, time in microseconds
distance= duration*0.034/2;        //Calculating actual/real distance
 return distance;
  }


 int   ir_sensor(){
    int IRValue = digitalRead(IRSensor);
    return IRValue ;
    
  }

 int   air_quality(){
    int analogSensor = analogRead(smokeA0);
    return analogSensor;
  }

 int   ldr_sensor(){
    int temp=digitalRead(5);      
   return temp ;
  }





 

 
