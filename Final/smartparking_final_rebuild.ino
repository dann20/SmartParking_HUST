#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include <string.h>
#define BLYNK_MAX_SENDBYTES 1200;

char auth[] = "F1-e7o1tVBhcJTc1aLRAOKsE8EWEeqDv";
char ssid1[] = "ahihi";
char pass1[] = "12345678";
const char* ssid = "ahihi";
const char* password = "12345678";
const char* mqtt_server = "192.168.43.117";
const uint16_t mqtt_port = 1883;
const byte ledPin = 16;
const long utcOffsetInSeconds = 25200;
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

long lastMsg = 0;
char msg[50];
int value = 0, stt=0;
String date_time;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String users[3]; // define lot number =0,1,2
unsigned long timeIn[3], timeOut[3], dur[3];

// NodeMCU pins
unsigned int pins[3] = {16,5,4};

// Blynk pins
unsigned int blynkStatus[3] = {1,2,3};
unsigned int blynkDur[3] = {5,6,7};

// Mosquitto topics
char* lotTopic[3] = {"lot1/status", "lot2/status", "lot3/status"};
char* durTopic[3] = {"duration", "duration2", "duration3"};
char* timeInTopic[3] = {"time-in", "time-in2", "time-in3"};
char* timeOutTopic[3] = {"time-out", "time-out2", "time-out3"};
char* feeTopic[3] = {"fee", "fee2", "fee3"};

// Email list
const char* emailList[3] = {
	"shinnan10xs@gmail.com",
	"quangnb2000@gmail.com",
	"letuanfake@gmail.com"
};

void setup() {
  pinMode(ledPin,INPUT);
  Serial.begin(115200);
  setup_wifi();
  timeClient.begin();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  Blynk.begin(auth, ssid1, pass1, IPAddress(192,168,43,117),8080);
  Blynk.virtualWrite(V4, "clr");
}
BLYNK_WRITE(V0)
{
  const char* user = param.asStr();
  users[0] = param.asStr();
  client.publish("user", user);
}

BLYNK_WRITE(V8)
{
  const char* user2 = param.asStr();
  users[1] = param.asStr();
  client.publish("user2", user2);
}
BLYNK_WRITE(V9)
{
  const char* user3 = param.asStr();
  users[2] = param.asStr();
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

String* getTimeRecord(){
	static String timeRecord[2];
	timeClient.update();
	String day=daysOfTheWeek[timeClient.getDay()];
	String formattedTime=timeClient.getFormattedTime();
	date_time=timeClient.getFormattedDate();
	int index_date = date_time.indexOf("T");
	String date = date_time.substring(0, index_date);
	timeRecord[0] = day + ", " +formattedTime + ", " +date;
	timeRecord[1] = formattedTime + "     " + date;
	Serial.println(timeRecord[0]);
	Serial.println(timeRecord[1]);
	return timeRecord;
}

char* getFee(unsigned long dur){
	static char fee[sizeof(unsigned long)*8+1];
	unsigned long tempFee;
	if (dur < 60)
		tempFee = 5000;
	else{
		tempFee = dur/60;
		if ((dur - tempFee*60) > 30)
			tempFee+=1;
			tempFee = tempFee*20000;
	}
	ultoa(tempFee,fee,10);
	return fee;
}

void emailFunction(unsigned int lotNum, char* fee){
	String mail;
    mail = "Phí gửi xe của bạn là: " + String(fee) + " VND";
	if (users[lotNum] == "dan1") Blynk.email(emailList[0],"Tiền gửi xe",mail);
	else if (users[lotNum] == "quang1") Blynk.email(emailList[1],"Tiền gửi xe",mail);
	else if (users[lotNum] == "fil") Blynk.email(emailList[2],"Tiền gửi xe",mail);
}

void lotFunction(unsigned int lotNum){
	if (digitalRead(pins[lotNum]) == HIGH){
		Serial.println(String( "LOT " )+String(lotNum+1)+String( ": EMPTY" ));
		client.publish(lotTopic[lotNum], "NO");
		Blynk.virtualWrite(blynkStatus[lotNum], "EMPTY");
		timeClient.update();
		Serial.println(timeIn[lotNum]);
		Serial.println(timeOut[lotNum]);
		if ( (timeOut[lotNum] > timeIn[lotNum]) && (timeOut[lotNum] != 0) ){
			dur[lotNum] = timeOut[lotNum] - timeIn[lotNum];
			Serial.print("Thời gian đỗ: ");
			Serial.println(dur[lotNum]);
			Blynk.virtualWrite(blynkDur[lotNum], dur[lotNum]);
			char strDur[50];
			ultoa(dur[lotNum],strDur,10);
			client.publish(durTopic[lotNum], strDur);
			String* timeRecord = getTimeRecord();
			char mqttRecord[30];
			timeRecord[0].toCharArray(mqttRecord,40);
			Serial.println(mqttRecord);
			Serial.println(timeRecord[1]);
			client.publish(timeOutTopic[lotNum], mqttRecord);
			timeOut[lotNum] = 0;

			char* fee = getFee(dur[lotNum]);
			client.publish(feeTopic[lotNum], fee);
			Serial.println(fee);
			emailFunction(lotNum, fee);
			Blynk.virtualWrite(V4,"add",stt,timeRecord[1],"Lot "+String(lotNum+1));
			Blynk.virtualWrite(V4,"deselect",stt);
			stt++;
			users[lotNum]="";
		}
		timeIn[lotNum]=timeClient.getEpochTime();
		Serial.println(users[lotNum]);
	}
	else{
		Serial.println(String( "LOT " )+String(lotNum+1)+String( ": OCCUPIED" ));
		client.publish(lotTopic[lotNum], "YES");
		Blynk.virtualWrite(blynkStatus[lotNum], "OCCUPIED");
		timeClient.update();
		if (timeOut[lotNum] == 0){
			String* timeRecord = getTimeRecord();
			char mqttRecord[30];
			timeRecord[0].toCharArray(mqttRecord,40);
			client.publish(timeInTopic[lotNum], mqttRecord);
			Blynk.virtualWrite(V4,"add",stt,timeRecord[1],"Lot "+String(lotNum+1));
			Blynk.virtualWrite(V4,"pick",stt);
			Blynk.virtualWrite(V4,"select",stt);
			stt++;
		}
		timeOut[lotNum]=timeClient.getEpochTime();
		Serial.println(timeIn[lotNum]);
		Serial.println(timeOut[lotNum]);
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
	lotFunction(0);
	lotFunction(1);
	lotFunction(2);
  }
  Blynk.run();
  delay(100);
}
