#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// ---------- WiFi ----------
const char* ssid = "iphone mark";
const char* password = "0956950402";

// ---------- Email ----------
#define SMTP_HOST "smtp.yourserver.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "s6503014622057@email.kmutnb.ac.th"
#define AUTHOR_PASSWORD "1679900586771"
#define RECIPIENT_EMAIL "pruettapongpoonpun@gmail.com"

// ---------- NTP ----------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); // GMT+7

// ---------- Data Struct ----------
struct DataEntry {
  String dateTime;
  int value1;
  int value2;
};

DataEntry dataList[10];
int dataCount = 0;

// ---------- SMTP Client ----------
SMTPSession smtp;

void sendEmail() {
  SMTP_Message message;

  message.sender.name = "ESP32";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Sensor Data Report";
  message.addRecipient("Receiver", RECIPIENT_EMAIL);

  String content = "Datetime, Value1, Value2\n";
  for (int i = 0; i < 10; i++) {
    content += dataList[i].dateTime + ", ";
    content += String(dataList[i].value1) + ", ";
    content += String(dataList[i].value2) + "\n";
  }

  message.text.content = content.c_str();
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  smtp.debug(1);
  ESP_Mail_Session session;

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  Serial.println("Sending Email...");
  if (!MailClient.sendMail(&smtp, &message, true)) {
    Serial.println("Send failed: " + smtp.errorReason());
  } else {
    Serial.println("Email sent successfully!");
  }

  // Reset data
  dataCount = 0;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  timeClient.begin();
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
}

void loop() {
  if (dataCount < 10) {
    timeClient.update();

    DataEntry entry;
    entry.dateTime = timeClient.getFormattedTime();
    entry.value1 = random(0, 100);
    entry.value2 = random(0, 100);

    dataList[dataCount++] = entry;

    Serial.printf("Data %d: %s, %d, %d\n", dataCount, entry.dateTime.c_str(), entry.value1, entry.value2);
  }

  if (dataCount == 10) {
    sendEmail();
  }

  delay(2000);  // Every 2 seconds
}
