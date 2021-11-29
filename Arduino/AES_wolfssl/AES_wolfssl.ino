#include <NMEAGPS.h>
#include <wolfssl.h>
#include <foto.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <string.h>
#define gpsPort Serial1 // just an alias, not a whole new variable
#define nodeMCU Serial2
#define KEY_LEN 24
NMEAGPS gps;
gps_fix fix;
Aes enc;
Aes dec;
int count = 0;
struct datagps // Cria uma STRUCT para armazenar os dados de uma pessoa
{
  float flat;
  float flon;
  float fvel;
  unsigned long age;
  char mensagem[1024];
};
typedef struct datagps Datagps;

void  setup()
{
  Serial.begin( 9600 );
  gpsPort.begin( 9600 );
  nodeMCU.begin(9600);
}

void loop()
{

  int ret = 0;
  long int t0,t1,t6,t7;
  char mensagem[1024];
  byte cipher[AES_BLOCK_SIZE * 20]; // Some multiple of 16 bytes
  byte key128[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
  byte key192[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4};
  byte key256[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
  byte iv[]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
  Datagps dados;
  
  if (gps.available( gpsPort )) {
    fix = gps.read();
    dados.flat = fix.latitude();
    dados.flon = fix.longitude();
    dados.fvel = fix.speed_mph();
    sprintf((char*)mensagem, "latitude: %.6f, longitude: %.6f, velocidade: %.6f e o resto é apenas enchecao de linguica para dar o numero de characteres latitude: %.6f, longitude: %.6f, velocidade: %.6f e o resto é apenas enchecao de linguica para dar o numero de characteres testando", dados.flat, dados.flon, dados.fvel, dados.flat, dados.flon, dados.fvel);
   // Serial.print((char*)mensagem);
    
    byte plain[AES_BLOCK_SIZE * 2000];
    if (KEY_LEN == 16) {
     
      if (ret = wc_AesSetKey(&enc, key128, sizeof(key128), iv, AES_ENCRYPTION) != 0) {
        // failed to set aes key
      }
    } else if (KEY_LEN == 24) {
      if (ret = wc_AesSetKey(&enc, key192, sizeof(key192), iv, AES_ENCRYPTION) != 0) {
        // failed to set aes key
      }
    } else if (KEY_LEN == 32) {

      if (ret = wc_AesSetKey(&enc, key256, sizeof(key256), iv, AES_ENCRYPTION) != 0) {
        // failed to set aes key
      }
    }
    t0 = micros();
    if ((ret = wc_AesCbcEncrypt(&enc, cipher, foto2, sizeof(foto2))) != 0 ) {
       Serial.print("erro");
    }
    t1 = micros();
    
    //Serial.println(t1 - t0);
    
    //nodeMCU.write(cipher,sizeof(foto2));
    
   // nodeMCU.readBytes(cipher,1024);

    if (KEY_LEN == 16) {
      if (ret = wc_AesSetKey(&dec, key128, sizeof(key128), iv, AES_DECRYPTION) != 0) {
        // failed to set aes key
      }
    } else if (KEY_LEN == 24) {
    
      if (ret = wc_AesSetKey(&dec, key192, sizeof(key192), iv, AES_DECRYPTION) != 0) {
        // failed to set aes key
      }
    } else if (KEY_LEN == 32){
     
      if (ret = wc_AesSetKey(&dec, key256, sizeof(key256), iv, AES_DECRYPTION) != 0) {
        Serial.print("erro");
      }    
    }
    char save[120];    
    t6 = micros();
    wc_AesCbcDecrypt(&dec, plain, cipher, sizeof(cipher));
    t7 = micros();

    if(count%50 == 0){
       Serial.println("arduino key len gps data;t0;t1;t6;t7");
    }
   
     sprintf(save,"%d;%u;%u;%u;%u",KEY_LEN,t0,t1,t6,t7);

   Serial.println(save);
    count++;   

  }
}
