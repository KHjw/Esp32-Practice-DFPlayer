#include <Arduino.h>
#line 1 "c:\\Github\\Esp32-Practice-DFPlayer\\Esp32-Practice-DFPlayer.ino"
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

#line 1 "c:\\Github\\Esp32-Practice-DFPlayer\\dfplayer.ino"
void DfpSetup(){
  DFPlayerSerial.begin(9600, SERIAL_8N1, DFP_RX, DFP_TX);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(DFPlayerSerial)){
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(10);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}

void DfpPlay(){
	TrackInfo[0] = PlayFolder;
	TrackInfo[1] = PlayFile;

	if(CurrentTrack != TrackInfo){
		myDFPlayer.playFolder(TrackInfo[0], TrackInfo[1]);
	}
	CurrentTrack = TrackInfo;
}
#line 1 "c:\\Github\\Esp32-Practice-DFPlayer\\encoder.ino"
long readEncoderValue(void){
    return encoderValue/4;
}

boolean isButtonPushDown(void){
  if(!digitalRead(buttonPin)){
    delay(5);
    if(!digitalRead(buttonPin))
      return true;
  }
  return false;
}

void Starter_Guage(int AnswerRev){
  int SingleRevVal = 3000;
  int AnswerVal = 3000;

  AnswerVal=SingleRevVal*AnswerRev;

  if(readEncoderValue()>=(AnswerVal+50*AnswerRev)){
    Serial.print("====== Starter Guage Full!!!");
    StarterProgress = 1;
  }
  else{
    Serial.print("====== Keep going");
    StarterProgress = 0;
  }
}

void Encoder_Setup(){
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(encoderPinA, updateEncoder, CHANGE);
  attachInterrupt(encoderPinB, updateEncoder, CHANGE);
}

void Encoder_Progress_Loop(){                                // "encoder값, 버튼눌림" 을 표시
  if(isButtonPushDown()){
    Serial.print(readEncoderValue());
    Serial.print(", ");
    Serial.print("1");
  }else{
    Serial.print(readEncoderValue());
    Serial.print(", ");
    Serial.print("0");
  }
  Serial.print(", ");
  Starter_Guage(CorrectRev);
  Serial.println("");
  delay(50);
}

void updateEncoder(){
  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue   ;

  lastEncoded = encoded; //store this value for next time
}
#line 1 "c:\\Github\\Esp32-Practice-DFPlayer\\game.ino"
void GameSetup(){

}

void GameSystem(){
  if(RfidPASS == 0){
    RfidLoop();
  }
  else{
    if(StarterProgress == 0){
      Encoder_Progress_Loop();
    }
    else{
      RfidCheckLoop();
    }
  }
}
#line 1 "c:\\Github\\Esp32-Practice-DFPlayer\\rfid.ino"
void RfidInit(){
  Serial.println("------------Rfid Initialized------------");
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  Serial.println("Found PN532");              // Got ok data, print it out!
  nfc.SAMConfig();                            // configure board to read RFID tags
  Serial.println("Waiting for Card ...");
}

void Rfid_Identify(uint8_t data[32]){
  String RfidID = "";

  for(int i=0; i<4; i++){
    RfidID += (char)data[i];
  }

  if(RfidID == "G1P9")
  {
    Serial.print("G1P9");
    PlayFolder = 1;
  }
  else if(RfidID == "G1P7")
  {
    Serial.print("G1P7");
    PlayFolder = 2;
  }
  else{
    Serial.print("Unidentified Chip");
  }
  Serial.println("");
}

void RfidLoop(){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    if (uidLength == 7)
    {
      uint8_t data[32];

      Serial.print("NTAG2xx tag Detected : ");     

      success = nfc.ntag2xx_ReadPage(7, data); 

      if (success) 
      {
        Rfid_Identify(data);
      }
    }
    else
    {
      Serial.println("This doesn't seem to be an NTAG203 tag");
    }
    Serial.flush();    
  }
  delay (50);
}

void RfidCheckLoop(){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    if (uidLength == 7)
    {
      uint8_t data[32];
      success = nfc.ntag2xx_ReadPage(7, data); 
      if (success) 
      {
        Serial.print("NTAG2xx tag Detected");
        RfidPASS = 0;
        StarterProgress = 0;
        encoderValue = 0;
      }
    }
    else
    {
      Serial.println("This doesn't seem to be an NTAG203 tag");
    }
    Serial.flush();    
  }
  delay (50);
}
