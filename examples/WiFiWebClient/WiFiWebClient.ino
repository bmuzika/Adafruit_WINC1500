/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <SPI.h>
#include <Adafruit_WINC1500.h>

#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4

Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);

// Default Hardware SPI (SCK/MOSI/MISO), SS -> #10, INT -> #7, RST -> #5, EN -> 3-5V
//Adafruit_WINC1500 WiFi;


char ssid[] = "yournetwork";     //  your network SSID (name)
char pass[] = "yourpassword";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(141,101,112,175);  // numeric IP for Google (no DNS)
char server[] = "www.adafruit.com";    // name address for Google (using DNS)
#define webpage "/testwifi/index.html"


// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
Adafruit_WINC1500Client client;

void setup() {
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  Serial.println("WINC1500 Web client test");

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET ");
    client.print(webpage);
    client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.println("Connection: close");
    client.println();
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

