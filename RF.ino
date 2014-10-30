#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include "commands.h"

byte data[DATA_LEN];

void setup_RF(char * address){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)address);
  Mirf.payload = DATA_LEN;
  Mirf.config();
}

void rf_wait_for_cmd(){
  while (true) {
    if(!Mirf.isSending() && Mirf.dataReady()){
      Mirf.getData(data);
      break;
    }
  }
}

void rf_ack() {
  // Echo what was received back to confirm
  Mirf.setTADDR((byte *)"serv1");
  Mirf.send(data);
}

byte rf_cmd_type() {
  return data[0];
}

unsigned int rf_cmd_val() {
  return word(data[1], data[2]);
}
