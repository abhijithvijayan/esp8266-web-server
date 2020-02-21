#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

#define WIFI_SSID "My_Wi-Fi"
#define WIFI_PASSWORD "my-awesome-password"
#define HTTP_SERVER_PORT 8266

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

// initialise server
ESP8266WebServer server(HTTP_SERVER_PORT);

void connectToWifi() {
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP &event) {
    Serial.printf("Connected to Wi-Fi: `%s`\n", WiFi.SSID().c_str());
    Serial.print("Local IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    // start server
    startWebServer();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
    // kill webserver
    server.close();
    server.stop();

    Serial.println("Disconnected from Wi-Fi.");
    wifiReconnectTimer.once(4, connectToWifi);
}

void startWebServer() {
    server.begin();

    Serial.print("Web server started, open `");
    Serial.print(WiFi.localIP().toString().c_str());
    Serial.print(":");
    Serial.print(HTTP_SERVER_PORT);
    Serial.println("` in a web browser\n");
}

String generateRootHTML() {
    String pageContent = "<!DOCTYPE html>\n";
    pageContent += "<html lang=\"en\">\n";
    pageContent += "<head>\n<meta charset=\"UTF-8\">\n";
    pageContent += "<meta name=\"viewport\" content=\"width=device-width, "
                   "initial-scale=1.0\">\n";
    pageContent += "<title>ESP8266 Web Server</title>\n</head>\n";
    pageContent += "<body>\n<p>Hello World 🤨</p>\n</body>\n</html>";

    return pageContent;
}

String generateNotFoundHTML() {
    String pageContent = "<!DOCTYPE html>\n";
    pageContent += "<html lang=\"en\">\n";
    pageContent += "<head>\n<meta charset=\"UTF-8\">\n";
    pageContent += "<meta name=\"viewport\" content=\"width=device-width, "
                   "initial-scale=1.0\">\n";
    pageContent += "<title>404 | Not Found</title>\n</head>\n";
    pageContent += "<body>\n<p>404 | Not Found 😴</p>\n</body>\n</html>";

    return pageContent;
}

void serveRootPage() { server.send(200, "text/html", generateRootHTML()); }
void handleNotFound() { server.send(404, "text/html", generateNotFoundHTML()); }

void setup() {
    Serial.begin(115200);
    Serial.println();

    wifiConnectHandler    = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

    // server routes
    server.on("/", serveRootPage);
    server.onNotFound(handleNotFound);

    connectToWifi();
}

void loop() { server.handleClient(); }