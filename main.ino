

#include <WiFi.h>  // Use #include <ESP8266WiFi.h> for ESP8266
#include <WebServer.h>  // Include the appropriate WebServer library
#include <ArduinoJson.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "ssdi";
const char* password = "****";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 16
#define OLED_CS 5
#define OLED_RESET 17

const int speakerPin = 19; // GPIO 19

WebServer server(80);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  Serial.begin(9600);

  ledcSetup(0, 2000, 8); // Channel 0, 2 kHz frequency, 8-bit resolution
  ledcAttachPin(speakerPin, 0); // Attach channel 0 to GPIO 19


  ledcWriteTone(0, 500);
  delay(500); // Play for 1 second
  ledcWriteTone(0, 0);


  // Start display
  display.begin(SSD1306_SWITCHCAPVCC);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");

  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setRotation(2);
  display.setCursor(0, 0);
  display.print(WiFi.localIP());
  display.display();

  // Define the POST route and handle the incoming data
  server.on("/post", HTTP_POST, []() {


    String entityBody = server.arg("plain");  // Get the body of the request
    Serial.println("Received POST data:");
    Serial.println(entityBody);

    JsonDocument doc;
    deserializeJson(doc, entityBody);


    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setRotation(2);
    display.setCursor(0, 0);
    display.print("COMMIT ON\n MAIN\n BRANCH!");
    display.display();

    ledcWriteTone(0, 1000);
    delay(1000); // Play for 1 second
    ledcWriteTone(0, 0);
    delay(1000);

    ledcWriteTone(0, 1000);
    delay(1000); // Play for 1 second
    ledcWriteTone(0, 0);
    delay(1000);

    ledcWriteTone(0, 1000);
    delay(1000); // Play for 1 second
    ledcWriteTone(0, 0);
    delay(1000);

    display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setRotation(2);
    display.setCursor(0, 0);
    display.print(doc["commits"][0]["author"]["name"].as<const char*>());

    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println(doc["ref"].as<const char*>());

    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print(doc["commits"][0]["message"].as<const char*>());

    display.display();

    server.send(200, "text/plain", "Cool");
  });
  
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}

void showText(String text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(2);
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}
