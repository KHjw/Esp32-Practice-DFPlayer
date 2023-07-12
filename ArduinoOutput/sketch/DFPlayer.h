#line 1 "c:\\Github\\Esp32-Practice-DFPlayer\\DFPlayer.h"
#ifndef _DONE_ENCODERRFID_CODE_
#define _DONE_ENCODERRFID_CODE_

#include "Library_and_pin.h"

//****************************************Rfid SETUP****************************************
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void RfidInit();
void RfidLoop();
void Rfid_Identify(uint8_t data[32]);
void RfidCheckLoop();

char RfidID;

//****************************************Encoder SETUP**************************************************
void Encoder_Setup();
long readEncoderValue(void);
void updateEncoder();
void Encoder_Progress_Loop();

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

int CorrectRev = 1;              // 정답처리를 위한 회전수

//****************************************Game SETUP**************************************************
void GameSetup();
void GameSystem();

int RfidPASS = 0;
int StarterProgress = 0;

//****************************************DFPlayer SETUP**************************************************
HardwareSerial DFPlayerSerial(2);
DFRobotDFPlayerMini myDFPlayer;

void DfpSetup();

int PlayFolder = 0;
int PlayFile = 0;
int CurrentTrack[2] = {0, 0};
int TrackInfo[2] = {0, 0};

#endif