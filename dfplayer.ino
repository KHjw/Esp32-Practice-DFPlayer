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