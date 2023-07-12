 /*
 * @file Esp32-Practice-EncoderRfid.ino
 * @author 홍진우 KH.jinu (kevinlike@naver.com)
 * @brief
 * @version 1.0
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "DFPlayer.h"

void setup(void) {
  Serial.begin(115200);
  RfidInit();
  DfpSetup();
  Encoder_Setup();
}

void loop(void) {
  GameSystem();
}
