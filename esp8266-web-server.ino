#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ESP8266WebServer.h>

#define WIFI_SSID "My_Wi-Fi"
#define WIFI_PASSWORD "my-awesome-password"

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

// initialise server at port 80
ESP8266WebServer server(80);

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  wifiReconnectTimer.once(2, connectToWifi);
}

String generateRootHTML() {
  String pageContent = "<!DOCTYPE html>\n";
  pageContent += "<html lang=\"en\">\n";
  pageContent += "<head>\n<meta charset=\"UTF-8\">\n";
  pageContent += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  pageContent += "<title>ESP8266 Web Server</title>\n</head>\n";
  pageContent += "<body>\n<p>Hello World :)</p>\n</body>\n</html>";

  return pageContent;
}

void getRootPath() {
  server.send(200, "text/html", generateRootHTML());
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  connectToWifi();

  delay(100);

  server.on("/", getRootPath);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}