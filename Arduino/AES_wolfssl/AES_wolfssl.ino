#include <NMEAGPS.h>
#include <wolfssl.h>
#include <user_settings.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <string.h>
#define gpsPort Serial2 // just an alias, not a whole new variable
int experimento = 0;
NMEAGPS gps;
gps_fix fix;
Aes enc;
Aes dec;
struct datagps // Cria uma STRUCT para armazenar os dados de uma pessoa
{
  float flat;
  float flon;
  float fvel;
  unsigned long age;
};
typedef struct datagps Datagps;

void  setup()
{
  Serial.begin( 115200 );
  gpsPort.begin( 9600 );
}

void loop()
{
  int ret = 0;
  byte cipher[AES_BLOCK_SIZE * 10]; // Some multiple of 16 bytes
  byte key128[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
  byte key192[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4};
  byte key256[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
  byte iv[]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
  Datagps dados, plain;
  if (gps.available( gpsPort )) {
    fix = gps.read();
    dados.flat = fix.latitude();
    dados.flon = fix.longitude();
    dados.fvel = fix.speed_mph();
    Serial.print("Lat");
    Serial.println( dados.flat ); // positive for N, negative for S
    Serial.print("Long ");
    Serial.println( dados.flon ); // positive for E, negative for W
    Serial.println("---------------");

    if (experimento == 0) {
      if (ret = wc_AesSetKey(&enc, key128, sizeof(key128), iv, AES_ENCRYPTION) != 0) {
        // failed to set aes key
      }
    } else if (experimento == 1) {
      if (ret = wc_AesSetKey(&enc, key192, sizeof(key192), iv, AES_ENCRYPTION) != 0) {
        // failed to set aes key
      }
    } else {
      if (ret = wc_AesSetKey(&enc, key256, sizeof(key256), iv, AES_ENCRYPTION) != 0) {
        // failed to set aes key
      }
    }

    if ((ret = wc_AesCbcEncrypt(&enc, cipher, (byte*)&dados, sizeof(dados))) != 0 ) {
      // block align error
    }

    if (experimento == 0) {
      if (ret = wc_AesSetKey(&dec, key128, sizeof(key128), iv, AES_DECRYPTION) != 0) {
        // failed to set aes key
      }
    }else if(experimento == 1){
       if (ret = wc_AesSetKey(&dec, key192, sizeof(key192), iv, AES_DECRYPTION) != 0) {
        // failed to set aes key
      }
    }else{
      if (ret = wc_AesSetKey(&dec, key128, sizeof(key256), iv, AES_DECRYPTION) != 0) {
        // failed to set aes key
      }
      experimento = 0;
    }
   
    wc_AesCbcDecrypt(&dec, (byte*)&plain, cipher, sizeof(cipher));
    Serial.print("Lat");
    Serial.println( plain.flat ); // positive for N, negative for S
    Serial.print("Long ");
    Serial.println( plain.flon ); // positive for E, negative for W
    experimento ++;
    delay(500);

  }
}
