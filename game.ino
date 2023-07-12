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