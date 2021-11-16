#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#include <string.h>
#define PORT 1234
#define HOST "172.22.225.116"
#define SSID "Evaldotania_2G"
#define PASS "21455151"

struct datagps // Cria uma STRUCT para armazenar os dados de uma pessoa
{
  float flat;
  float flon;
  float fvel;
  unsigned long age;
  char mensagem[100];

};
typedef struct datagps Datagps;

Datagps dados;

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
  uint8_t *ciphertext;
  if (due.available()) {
    int teste = due.readBytes((byte*)&ciphertext, 1024);

    client.write((byte*)&ciphertext, sizeof(ciphertext));

  }

}
