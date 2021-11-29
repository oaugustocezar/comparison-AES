#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#include <string.h>
#define PORT 1234
#define HOST "192.168.0.24"
#define SSID "HausOfGaga_2G"
#define PASS "BornThisWay"

SoftwareSerial due(13, 15); // RX = D7, TX  = D8

WiFiClient client;
void setup() {

  WiFi.begin(SSID, PASS);
  Serial.begin(9600);
  due.begin(9600);

  Serial.printf("Connecting to %s ", SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Serial.println(WiFi.localIP());

  if (client.connect(HOST, PORT)) {
    Serial.println("Server connected");
  } else {
    Serial.println("falha ao conectar");
  }

}

void loop() {
  byte ciphertext[1024];
  byte ciphertextReceived[1024];
 int teste = 0;
 
  if (due.available()) {
    teste = due.read(ciphertext, 1024);
    delay(300);
    client.write(ciphertext, teste);
    delay(500);
   
    if (client.available()) {
      int tamanho = client.read(ciphertextReceived, 1024);
      due.write(ciphertextReceived, tamanho);
    }

  }
  delay(5000);
  ESP.restart();
}
