#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include <string.h>
#define BLYNK_MAX_SENDBYTES 1200; 

char auth[] = "l5HloykPNfx13VKB1ICPS4qVg7hWXCIH";        
char ssid1[] = "ahihi";        
char pass1[] = "12345678";        
const char* ssid = "ahihi";
const char* password =    "ahihi";
const char* mqtt_server = "192.168.43.159";   
const uint16_t mqtt_port = 1883;
const byte ledPin = 16;
const long utcOffsetInSeconds = 25200;
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

long lastMsg = 0;
char msg[50];
int value = 0,stt=0;
String date_time;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long o1,e1,t1,o2,e2,t2,o3,e3,t3 ;
unsigned long h,h2,h3;

void setup() {
  pinMode(ledPin,INPUT); 
  Serial.begin(115200);
  setup_wifi();
  timeClient.begin();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  Blynk.begin(auth, ssid1, pass1, IPAddress(192,168,43,159),8080);// Ket noi voi blynk
  Blynk.virtualWrite(V4, "clr");
}
String user01,user02,user03;
BLYNK_WRITE(V0)
{
  const char* user = param.asStr();
  user01 = param.asStr();
  client.publish("user", user);
}

BLYNK_WRITE(V8)
{
  const char* user2 = param.asStr();
  user02 = param.asStr();
  client.publish("user2", user2);
}
BLYNK_WRITE(V9)
{
  const char* user3 = param.asStr();
  user03 = param.asStr();
  client.publish("user3", user3);
}
BLYNK_WRITE(V10) {
  if (param.asInt()) {
    Blynk.virtualWrite(V4, "clr");
    stt = 0;
  }
}

void setup_wifi() {

  delay(10);
  // Kết nối wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Đợi tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.publish("event", "hello world");
      client.subscribe("event");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
     if(digitalRead(16)==HIGH){
      Serial.print("LOT 1: ");
      Serial.println("EMPTY");
      client.publish("lot1/status", "NO");
      Blynk.virtualWrite(V1, "EMPTY");
      timeClient.update();
      Serial.println(e1);Serial.println(o1);
      if ((o1>e1)&&(o1!=0))
        {
          t1=o1-e1;
          Serial.print("Thời gian đỗ: ");
          Serial.println(t1);//seconds
          Blynk.virtualWrite(V5,t1);
          char t1str[50];
          ultoa(t1,t1str,10);
          client.publish("duration",t1str);
          timeClient.update();
          String day=daysOfTheWeek[timeClient.getDay()];
          String timee=timeClient.getFormattedTime();
          date_time=timeClient.getFormattedDate();
          int index_date = date_time.indexOf("T");
          String date = date_time.substring(0, index_date);
          String recordstr =day + ", " +timee + ", " +date;
          String recordtb = timee + "     " + date;
          char record[30];
          recordstr.toCharArray(record,40);Serial.println(record);
          client.publish("time-out",record);
          o1=0;
          h=t1/60;
          if ((t1-h*60)>30) 
          {
            h+=1;
            h*=20000;
          }
          else h*=20000;
          char hstr[50];
          ultoa(h,hstr,10);
          client.publish("fee",hstr); 
          Serial.println(h);
          String hmail;
          hmail = "Phí gửi xe của bạn là: " + String(hstr) + " VND";
          if (user01 == "dan1") Blynk.email ("shinnan10xs@gmail.com", "Tiền gửi xe" , hmail);
          else if (user01 == "quang1") Blynk.email("quangnb2000@gmail.com","Tiền gửi xe", hmail);
          else if (user01 == "phu1") Blynk.email("phu17102017@gmail.com","Tiền gửi xe", hmail);
          Blynk.virtualWrite(V4,"add",stt,recordtb,"Lot 1");
          Blynk.virtualWrite(V4,"deselect",stt);
          stt++;
          user01="";
        }
        e1=timeClient.getEpochTime();
        Serial.println(user01);
        
    } else{
      Serial.print("LOT 1: ");
      Serial.println("OCCUPIED");
      client.publish("lot1/status", "YES");
      Blynk.virtualWrite(V1, "OCCUPIED");
      timeClient.update();
      Serial.print(daysOfTheWeek[timeClient.getDay()]);
      String day=daysOfTheWeek[timeClient.getDay()];
      Serial.print(", ");
      Serial.print(timeClient.getFormattedTime());
      String timee=timeClient.getFormattedTime();
      Serial.print(", ");
      date_time = timeClient.getFormattedDate();
      int index_date = date_time.indexOf("T");
      String date = date_time.substring(0, index_date);
      Serial.println(date);
      if (o1==0)
      {
      String recordstr =day + ", " +timee + ", " +date;
      String recordtb = timee + "     " + date;
      Serial.println(recordstr);
      char record[30];
      recordstr.toCharArray(record,40);Serial.println(record);
      client.publish("time-in",record);
      Blynk.virtualWrite(V4,"add",stt,recordtb,"Lot 1");
      Blynk.virtualWrite(V4,"pick",stt);
      Blynk.virtualWrite(V4,"select",stt);
      stt++;
      }
      o1=timeClient.getEpochTime();Serial.println(o1);Serial.println(e1);
      }

       if(digitalRead(5)==HIGH){
      Serial.print("LOT 2: ");
      Serial.println("EMPTY");
      client.publish("lot2/status", "NO");
      Blynk.virtualWrite(V2, "EMPTY");
      timeClient.update();
      Serial.println(e2);Serial.println(o2);
      if ((o2>e2)&&(o2!=0))
        {
          t2=o2-e2;
          Serial.print("Tgian do: ");
          Serial.println(t2);//seconds
          Blynk.virtualWrite(V6,t2);
          char t2str[50];
          ultoa(t2,t2str,10);
          client.publish("duration2",t2str);
          timeClient.update();
          String day=daysOfTheWeek[timeClient.getDay()];
          String timee=timeClient.getFormattedTime();
          date_time = timeClient.getFormattedDate();
          int index_date = date_time.indexOf("T");
          String date = date_time.substring(0, index_date);
          String recordstr =day + ", " +timee + ", " +date;
          String recordtb = timee + "     " + date;
          char record[30];
          recordstr.toCharArray(record,40);Serial.println(record);
          client.publish("time-out2",record);
          o2=0;
          h2=t2/60;
          if ((t2-h2*60)>30) 
          {
            h2 +=1;
            h2*=20000;
          }
          else h2*=20000;
          char h2str[50];
          ultoa(h2,h2str,10);
          client.publish("fee2",h2str); 
          Serial.println(h2);
          String hmail;
          hmail = "Phí gửi xe của bạn là: " + String(h2str) + " VND";
          if (user02 == "dan1") Blynk.email ("shinnan10xs@gmail.com", "Tiền gửi xe" , hmail);
          else if (user02 == "quang1") Blynk.email("quangnb2000@gmail.com","Tiền gửi xe", hmail);
          else if (user02 == "phu1") Blynk.email("phu17102017@gmail.com","Tiền gửi xe", hmail);
          Blynk.virtualWrite(V4,"add",stt,recordtb,"Lot 2");
          Blynk.virtualWrite(V4,"deselect",stt);
          stt++;
          user02="";
        }
        e2=timeClient.getEpochTime();
    } else{
      Serial.print("LOT 2: ");
      Serial.println("OCCUPIED");
      client.publish("lot2/status", "YES");
       Blynk.virtualWrite(V2, "OCCUPIED");
       timeClient.update();
      Serial.print(daysOfTheWeek[timeClient.getDay()]);
      String day=daysOfTheWeek[timeClient.getDay()];
      Serial.print(", ");
      Serial.print(timeClient.getFormattedTime());
      String timee=timeClient.getFormattedTime();
      Serial.print(", ");
      date_time = timeClient.getFormattedDate();
      int index_date = date_time.indexOf("T");
      String date = date_time.substring(0, index_date);
      Serial.println(date);
      if (o2==0)
      {
        String recordstr =day + ", " +timee + ", " +date;
        String recordtb = timee + "     " + date;
      Serial.println(recordstr);
      char record[30];
      recordstr.toCharArray(record,40);Serial.println(record);
      client.publish("time-in2",record);
      Blynk.virtualWrite(V4,"add",stt,recordtb,"Lot 2");
      Blynk.virtualWrite(V4,"pick",stt);
      Blynk.virtualWrite(V4,"select",stt);
      stt++;
      }
      o2=timeClient.getEpochTime();Serial.println(o2);Serial.println(e2);}

       if(digitalRead(4)==HIGH){
      Serial.print("LOT 3: ");
      Serial.println("EMPTY");
      client.publish("lot3/status", "NO");
      Blynk.virtualWrite(V3, "EMPTY");
      timeClient.update();
      Serial.println(e3);Serial.println(o3);
      if ((o3>e3)&&(o3!=0))
        {
          t3=o3-e3;
          Serial.print("Tgian do: ");
          Serial.println(t3);//seconds
          Blynk.virtualWrite(V7,t3);
          char t3str[50];
          ultoa(t3,t3str,10);
          client.publish("duration3",t3str);
          timeClient.update();
          String day=daysOfTheWeek[timeClient.getDay()];
          String timee=timeClient.getFormattedTime();
          date_time = timeClient.getFormattedDate();
          int index_date = date_time.indexOf("T");
          String date = date_time.substring(0, index_date);
          String recordstr =day + ", " +timee + ", " +date;
          String recordtb = timee + "     " + date;
          char record[30];
          recordstr.toCharArray(record,40);Serial.println(record);
          client.publish("time-out3",record);
          o3=0;
          h3=t3/60;
          if ((t3-h3*60)>30) 
          {
            h3+=1;
            h3*=20000;
          }
          else h3*=20000;
          char h3str[50];
          ultoa(h3,h3str,10);
          client.publish("fee3",h3str); 
          Serial.println(h3);
          String hmail;
          hmail = "Phí gửi xe của bạn là: " + String(h3str) + " VND";
          if (user03 == "dan1") Blynk.email ("shinnan10xs@gmail.com", "Tiền gửi xe" , hmail);
          else if (user03 == "quang1") Blynk.email("quangnb2000@gmail.com","Tiền gửi xe", hmail);
          else if (user03 == "phu1") Blynk.email("phu17102017@gmail.com","Tiền gửi xe", hmail);
          Blynk.virtualWrite(V4,"add",stt,recordtb,"Lot 3");
          Blynk.virtualWrite(V4,"deselect",stt);
          stt++;
          user03="";
        }
        e3=timeClient.getEpochTime();
    } else{
      Serial.print("LOT 3: ");
      Serial.println("OCCUPIED");
      Serial.println();
      client.publish("lot3/status", "YES");
       Blynk.virtualWrite(V3, "OCCUPIED");
       timeClient.update();
      Serial.print(daysOfTheWeek[timeClient.getDay()]);
      String day=daysOfTheWeek[timeClient.getDay()];
      Serial.print(", ");
      Serial.print(timeClient.getFormattedTime());
      String timee=timeClient.getFormattedTime();
      Serial.print(", ");
      date_time = timeClient.getFormattedDate();
      int index_date = date_time.indexOf("T");
      String date = date_time.substring(0, index_date);
      Serial.println(date);
      if (o3==0)
      {
        String recordstr =day + ", " +timee + ", " +date;
        String recordtb = timee + "     " + date;
      Serial.println(recordstr);
      char record[30];
      recordstr.toCharArray(record,40);Serial.println(record);
      client.publish("time-in3",record);
      Blynk.virtualWrite(V4,"add",stt,recordtb,"Lot 3");
      Blynk.virtualWrite(V4,"pick",stt);
      Blynk.virtualWrite(V4,"select",stt);
      stt++;
      }
      o3=timeClient.getEpochTime();Serial.println(o3);Serial.println(e3);
    }
  }
   Blynk.run();
}
