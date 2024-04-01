#define BLYNK_TEMPLATE_ID "TMPL3NabjJNc8"
#define BLYNK_TEMPLATE_NAME "SMART POWER DISTRIBUTION UNIT"
#define BLYNK_AUTH_TOKEN "7TeQ4HPl-eKsEtxyREnTzbHovZZXl8JK"
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "TP-LINK_MR100_2.4Ghz"; // wifi name
char pass[] = "Vishnu@18122002";     // wifi password
#define BLYNK_PRINT Serial
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HardwareSerial.h> // Include UART library
const int panel_pow = 33;   // panel voltage sensor
const int backup_bat = 32;  // battery voltage sensor
const float R1 = 30000.0;
const float R2 = 7500.0;
const float Vref = 3.32;
HardwareSerial SerialESP32(1); // Use Serial1 for ESP32
#define BOTtoken "6838766776:AAGqw_1ilLBc7bu8lPpnMpypryze0yggxuo"
#define CHAT_ID "1436137789"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
bool solarMode = false;
bool prevSolarMode = false;
bool systemInitialized = false;
WidgetLED SOL_UNT(V2);
WidgetLED BCK_UNT(V3);
void setup()
{
  Serial.begin(115200); 
  SerialESP32.begin(9600, SERIAL_8N1, 16, 17);
  delay(1); 
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  setupWiFi();
  Blynk.run();
  if (!systemInitialized)
  {
    sendInitMessageToTelegram();
    systemInitialized = true;
  }
}
void loop()
{
  Blynk.run();
  float voltage1 = readVoltage(panel_pow);
  float voltage2 = readVoltage(backup_bat);
  Serial.print("Voltage 1: ");
  Serial.println(voltage1);
  Serial.print("Voltage 2: ");
  Serial.println(voltage2);
  SerialESP32.println(voltage1);
  Blynk.virtualWrite(V0, voltage1);
  Blynk.virtualWrite(V1, voltage2);
  if (voltage1 >= 7.0 && !solarMode)
  {
    SOL_UNT.on();
    BCK_UNT.off();
    solarMode = true;
   // Serial.println("Switched to Solar Mode");
  }
  else if (voltage1 < 6.0 && solarMode)
  {
    BCK_UNT.on();
    SOL_UNT.off();
    solarMode = false;
   // Serial.println("Switched to Backup Mode");
  }
  if (solarMode != prevSolarMode)
  {
    sendMessageToTelegram();
    prevSolarMode = solarMode;
   // Serial.println("Sending message to Telegram"); // Debug statement
  }

  delay(1);
}

void setupWiFi()
{
  //Serial.println("Connecting to WiFi...");
  Blynk.begin(auth, ssid, pass);
}
float readVoltage(int pin)
{
  int sensorValue = analogRead(pin);
  float voltage = sensorValue * (Vref / 4095.0z);
  voltage = voltage * ((R1 + R2) / R2);
  return voltage;
}
void sendInitMessageToTelegram()
{
 // Serial.println("Sending initialization message to Telegram...");
  String message = "System initialized and ready.";

  if (client.connect("api.telegram.org", 443))
  {
    bot.sendMessage(CHAT_ID, message, "Markdown");
    delay(100);
  }
}
void sendMessageToTelegram()
{
 // Serial.println("Sending message to Telegram...");
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
