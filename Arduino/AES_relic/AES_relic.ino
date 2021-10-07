extern "C" {
#include <relic.h>
#include <relic_bc.h>
}
#include <NMEAGPS.h>
#include <string.h>
#define MAX_MSG 1024
#define gpsPort Serial2 // just an alias, not a whole new variable
#define DEBUG 0
int experimento = 0;
NMEAGPS gps;
gps_fix fix;

struct datagps // Cria uma STRUCT para armazenar os dados de uma pessoa
{
  float flat;
  float flon;
  float fvel;
  unsigned long age;
  char teste[100];

};
typedef struct datagps Datagps;
void setup() {
  Serial.begin( 115200 );
  Serial1.begin(9600);
  gpsPort.begin( 9600 );
}

void loop() {
  uint8_t key[] = "01234567890123456789012345678901"; // KEY 256 BITS
  uint8_t iv[] = "01234567890123456789012345678901"; // IV 256 BITS

  /* Buffer for the decrypted text */
  unsigned char decryptedtext[MAX_MSG];
  uint8_t ciphertext[MAX_MSG];
  char plaintext[MAX_MSG];
  char *mensagem = "Isso e um teste muito serio";
  int in_len = 0;
  int out_len = MAX_MSG;
  in_len = strlen(mensagem);
  Datagps dados, plain;
  if (gps.available( gpsPort )) {
    fix = gps.read();
    dados.flat = fix.latitude();
    dados.flon = fix.longitude();
    dados.fvel = fix.speed_mph();

    

  }
  if (bc_aes_cbc_enc(ciphertext, &out_len, (uint8_t*)&dados, sizeof(dados), key, 32, iv)) {
    if (DEBUG)
      Serial.println("ERRO enc");

  } else {
    if (DEBUG)
      Serial.println("Sucesso enc");
  }

  in_len = out_len;
  out_len = in_len;

  if (bc_aes_cbc_dec((uint8_t*)&plain, &out_len, ciphertext, in_len, key, 32, iv)) {
    if (DEBUG)
      Serial.println("ERRO dec");
  } else {
    if (DEBUG)
      Serial.println("Sucesso dec");

  }
  strcpy(plain.teste,"testando");
  

  Serial1.write((byte*)&plain, sizeof(plain));
  Serial.print("Resultado da cifra: "); Serial.println (plain.flat,6);  

  delay(5000);





}