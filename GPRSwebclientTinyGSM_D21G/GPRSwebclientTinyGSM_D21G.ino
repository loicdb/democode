/**************************************************************
 *
 * This sketch connects to a website and downloads a page.
 * It can be used to perform HTTP/RESTful API calls.
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 **************************************************************/

// Select your modem:
#define TINY_GSM_MODEM_SIM800
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

#include <TinyGsmClient.h>

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "YourAPN";
const char user[] = "";
const char pass[] = "";

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

const char server[] = "cdn.rawgit.com";
const char resource[] = "/vshymanskyy/tinygsm/master/extras/logo.txt";

int port = 80;

void setup() {
  // Set console baud rate
  SerialUSB.begin(115200);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(115200);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialUSB.println(F("Initializing modem..."));
  modem.restart();

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
}

void loop() {
  SerialUSB.print(F("Waiting for network..."));
  if (!modem.waitForNetwork()) {
    SerialUSB.println(" fail");
    delay(10000);
    return;
  }
  SerialUSB.println(" OK");

  SerialUSB.print(F("Connecting to "));
  SerialUSB.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialUSB.println(" fail");
    delay(10000);
    return;
  }
  SerialUSB.println(" OK");

  SerialUSB.print(F("Connecting to "));
  SerialUSB.print(server);
  if (!client.connect(server, port)) {
    SerialUSB.println(" fail");
    delay(10000);
    return;
  }
  SerialUSB.println(" OK");

  // Make a HTTP GET request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    // Print available data
    while (client.available()) {
      char c = client.read();
      SerialUSB.print(c);
      timeout = millis();
    }
  }
  SerialUSB.println();

  client.stop();
  SerialUSB.println("Server disconnected");

  modem.gprsDisconnect();
  SerialUSB.println("GPRS disconnected");

  // Do nothing forevermore
  while (true) {
    delay(1000);
  }
}

