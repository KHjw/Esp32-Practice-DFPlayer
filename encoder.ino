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