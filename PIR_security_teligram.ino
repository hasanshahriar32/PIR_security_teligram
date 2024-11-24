#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


LiquidCrystal_I2C lcd (0x27, 16, 2);
const char* ssid = "One_Sky";
const char* password = "amartyatushar11";


#define BOTtoken "5623049233:AAFX7zAZjHrsRYhAzcLiKLZ3dVWQiJHdnC8"
#define CHAT_ID "1191587070"

#define Sensor D0
#define LEDR D3
#define LEDG D4
#define Buzzer D5

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  Wire.begin(D2, D1);
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  pinMode(Sensor, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int a = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.setCursor(a, 0);
    lcd.print(".");
    delay(500);
    a++;
  }

  Serial.println("");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  delay(500);


//   String ipAddress = WiFi.localIP().toString();
// bot.sendMessage(CHAT_ID, "System started", "");
// bot.sendMessage(CHAT_ID, "IP address: " + ipAddress);

// Get the current date and time
  time_t now = time(nullptr);
  struct tm* timeinfo = gmtime(&now);

  // Send a message to Telegram with system info
  String ipAddress = WiFi.localIP().toString();
  String message = "System started\n";
  message += "IP address: " + ipAddress + "\n";
  message += "Date: " + String(timeinfo->tm_year + 1900) + "/" + String(timeinfo->tm_mon + 1) + "/" + String(timeinfo->tm_mday) + "\n";
  message += "Time: " + String(timeinfo->tm_hour) + ":" + String(timeinfo->tm_min) + ":" + String(timeinfo->tm_sec) + "\n";
  // message += "Region: " + String(TIMEZONE) + "\n";
  bot.sendMessage(CHAT_ID, message);


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System started");
  delay(1000);
  lcd.clear();
}
void loop() {
  bool value = digitalRead(Sensor);
  Serial.println(value);
  if (value == 1) {
    Serial.println("Motion Detected");
    digitalWrite(LEDR, HIGH);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(LEDG, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Motion Detected");
    bot.sendMessage(CHAT_ID, "Warning!! Sensor Triggered", "");
    // loop();
  } else if (value == 0) {
    digitalWrite(LEDR, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LEDG, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("No Motion      ");
  }
    delay(500);
}
