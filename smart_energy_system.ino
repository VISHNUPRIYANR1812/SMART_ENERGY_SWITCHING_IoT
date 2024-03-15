#define BLYNK_TEMPLATE_ID "TMPL3NabjJNc8"
#define BLYNK_TEMPLATE_NAME "SMART POWER DISTRIBUTION UNIT"
#define BLYNK_AUTH_TOKEN "7TeQ4HPl-eKsEtxyREnTzbHovZZXl8JK"
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "TP-LINK_MR100_2.4Ghz"; // wifi name
char pass[] = "Vishnu@18122002"; // wifi password
#define BLYNK_PRINT Serial

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const int panel_pow = 25; //panel voltage sensor
const int backup_bat = 26; //battery voltage sensor
const int relay1_pin = 14; //battery relay
const int relay2_pin = 15; //solar panel relay
const float R1 = 30000.0;
const float R2 = 7500.0;
const float Vref = 3.32;

#define BOTtoken "6838766776:AAGqw_1ilLBc7bu8lPpnMpypryze0yggxuo"
#define CHAT_ID "1436137789"

WidgetLED SOL_UNT(V2);
WidgetLED BCK_UNT(V3);

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

bool solarMode = false;
bool prevSolarMode = false; 

void setup()
{
  Serial.begin(115200);

  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);

 
  digitalWrite(relay1_pin, LOW);
  digitalWrite(relay2_pin, LOW);

  setupWiFi();
}

void loop()
{
  Blynk.run();
  float voltage1 = readVoltage(panel_pow);
  float voltage2 = readVoltage(backup_bat);
  Blynk.virtualWrite(V0, voltage1);
  Blynk.virtualWrite(V1, voltage2);


  if (voltage1 >= 6.0 && !solarMode) 
    digitalWrite(relay1_pin, HIGH);
    digitalWrite(relay2_pin, LOW);  
    SOL_UNT.on();
    BCK_UNT.off();
    solarMode = true; 
  }
  else if (voltage1 < 6.0 && solarMode)
  {
    digitalWrite(relay2_pin, HIGH); 
    digitalWrite(relay1_pin, LOW);  
    SOL_UNT.off();
    solarMode = false;
  } 
  if (solarMode != prevSolarMode) 
  {
    sendMessageToTelegram();
    prevSolarMode = solarMode; 

  delay(1000);
}
void setupWiFi()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}
float readVoltage(int pin)
{
  int sensorValue = analogRead(pin);
  float voltage = sensorValue * (Vref / 4095.0);
  voltage = voltage * ((R1 + R2) / R2);
  return voltage;
}
void sendMessageToTelegram()
{
  String message = "Mode of working: ";
  message += solarMode ? "Solar" : "Backup";
  message += "\n";
  message += "Solar power voltage: " + String(readVoltage(panel_pow), 2) + "V\n";
  message += "Backup power voltage: " + String(readVoltage(backup_bat), 2) + "V\n";

  if (client.connect("api.telegram.org", 443))
  {
    bot.sendMessage(CHAT_ID, message, "Markdown");
    delay(100);
  }
}