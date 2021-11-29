extern "C" {
#include <relic.h>
#include <relic_bc.h>
}
#include <NMEAGPS.h>
#include <string.h>
#define MAX_MSG 102400
#define KEY_LEN 32
#define nodeMCU Serial2
#define gpsPort Serial1 // just an alias, not a whole new variable
#define DEBUG 0
int experimento = 0;
NMEAGPS gps;
gps_fix fix;
unsigned long t0, t1, t6, t7;
char save[1024];

void defineKey (uint8_t * key, uint8_t * iv, int key_len);

struct datagps // Cria uma STRUCT para armazenar os dados de uma pessoa
{
  float lat;
  float lon;
  float ve;
  uint8_t mensagem[1024];
};
typedef struct datagps Datagps;
void setup() {
  Serial.begin( 9600 );
  gpsPort.begin( 9600 );
  Serial2.begin(9600);
}

void defineKey (uint8_t * key, uint8_t * iv, int key_len) {

  memcpy(iv, "0123456789012345", 16);

  if (key_len == 16) {

    memcpy(key, "0123456789012345", 16);

  } else if (key_len == 24) {

    memcpy(key, "012345678901234567890123", 24);


  } else if (key_len == 32) {

    memcpy(key, "01234567890123450123456789012345", 32);

  } else {

    printf("Chave inválida");

  }
}

void loop() {
  uint8_t *key;
  uint8_t *iv ;

  defineKey(key, iv, KEY_LEN);

  /* Buffer for the decrypted text */
  uint8_t decryptedtext[MAX_MSG];
  uint8_t ciphertext[MAX_MSG];
  uint8_t plaintext[MAX_MSG];
  Datagps dados, plain;
  int in_len = 0;
  int out_len = MAX_MSG;
  dados.lat = 0.0;
  dados.lon = 0.0;
  dados.ve = 0.0;

  if (gps.available( gpsPort )) {
    fix = gps.read();
    dados.lat = fix.latitude();
    dados.lon = fix.longitude();
    dados.ve = fix.speed_mph();
    sprintf((char*)dados.mensagem, "latitude: %.6f, longitude: %.6f, velocidade: %.6f e o resto é apenas enchecao de linguica para dar o numero de characteres latitude: %.6f, longitude: %.6f, velocidade: %.6f e o resto é apenas enchecao de linguica para dar o numero de characteres testando", dados.lat, dados.lon, dados.ve, dados.lat, dados.lon, dados.ve);
  }

  if (dados.lat == 0.0 && dados.lon == 0.0 && dados.ve == 0.0) {
    sprintf((char*)dados.mensagem, "latitude: %.6f, longitude: %.6f, velocidade: %.6f e o resto é apenas enchecao de linguica para dar o numero de characteres latitude: %.6f, longitude: %.6f, velocidade: %.6f e o resto é apenas enchecao de linguica para dar o numero de characteres testando", dados.lat, dados.lon, dados.ve, dados.lat, dados.lon, dados.ve);
  }
  ciphertext[0] = '\0';

  in_len = sizeof(foto2);
  t0 = micros();
  if (bc_aes_cbc_enc(ciphertext, &out_len, (uint8_t*)foto2, strlen((char*)dados.mensagem), key, 32, iv)) {
    if (DEBUG)
      Serial.println("ERRO enc");

  } else {
    if (DEBUG)
    Serial.println("Sucesso enc");
  }
  t1 = micros();
  uint8_t tamanho[4]; 
  sprintf((char*)tamanho,"%d",out_len);
  //Serial2.write(tamanho,4);
  //delay(200);
  //Serial2.write(ciphertext, out_len);

  in_len = out_len;
  out_len = in_len;
  
  t6 = micros();
  if (bc_aes_cbc_dec(plaintext, &out_len, ciphertext, in_len, key, 32, iv)) {
    if (DEBUG)
      Serial.println("ERRO dec");
  } else {
    if (DEBUG)
      Serial.println("Sucesso dec");
    plaintext[out_len] = '\0';
  }
  t7 = micros();
 
  if(experimento ==0)
  Serial.println("arduino key len gps data;t0;t1;t6;t7");
  
  sprintf(save,"%d;%u;%u;%u;%u",KEY_LEN,t0,t1,t6,t7);
  Serial.println(save);
  experimento ++;
  
    
  //delay(200);

}
