#define BUFFSIZ 90 // plenty big
char buffer[BUFFSIZ];
char buffidx;
int retry = 5;


int sendCommand(char* command, char* resp, int delayTime=500, int reps=5, int numData=2);
void setupCommands(){
  sendCommand("AT&F","OK");
  sendCommand("ATE0","OK");
  sendCommand("AT+CLIP=1","OK");
  sendCommand("AT+CMEE=0","OK");
  sendCommand("AT+CIPSHUT","SHUT");

}
void startUp(){

  //delay(1000);
  sendCommand("AT+CGATT?","+CGATT:");
  //  if(sendCommand("AT+CIFSR","10",1000,1)==1){
  //     sendCommand("AT+CIPCLOSE","CLOSE OK",500);
  //     sendCommand("AT+CIPSERVER=0","OK",500);
  //  }
  sendCommand("AT+CIPSHUT","SHUT");
  delay(1000);
  sendCommand("AT+CSTT=\"wap.cingular\"","OK");
  //Serial.println("WAP OK");
  //delay(5000);
  sendCommand("AT+CIICR","OK");
  sendCommand("AT+CIFSR","10");
  //sendCommand("AT+CGATT=0","OK",500);
  //sendCommand("AT+CIPSPRT=0","OK");
  sendCommand("AT+CIPSTART=\"tcp\",\"api.cosm.com\",\"8081\"","CONNECT OK",500,5,10);
  //sendCommand("AT+CIPSTART=\"tcp\",\"www.google.com\",\"80\"","CONNECT OK");
  //Serial.println("CONNECT OK");
  GPRS_Serial.flush();
  Serial.flush();
  delay(1000);
  
  //ATE0
  //AT+CIPSHUT
  //AT+CIFSR
}
int getAttachedGPRS(){
  return sendCommand("AT+CGATT?","+CGATT: 1");
}
void sendStuff(){

  GPRS_Serial.println("AT+CIPSEND");//begin send data to remote server
  delay(500);
  //ShowSerialData();
  String humidity = "1031";//these 4 line code are imitate the real sensor data, because the demo did't add other sensor, so using 4 string variable to replace.
  String moisture = "1242";//you can replace these four variable to the real sensor data in your project
  String temperature = "30";//
  String barometer = "60.56";//
  //GPRS_Serial.print("GET / HTTP/1.1\nHost: www.google.com\nUser-Agent: Arduino\n\n");
  GPRS_Serial.print("{\"method\": \"put\",\"resource\": \"/feeds/"+feedId+"/\",\"params\"");//here is the feed you apply from pachube
  delay(200);
  //ShowSerialData();
  GPRS_Serial.print(": {},\"headers\": {\"X-PachubeApiKey\":");//in here, you should replace your pachubeapikey
  delay(200);
  //ShowSerialData();
  GPRS_Serial.print(" \""+apiKey+"\"},\"body\":");//pachubeapikey
  delay(200);
  GPRS_Serial.print(" {\"version\": \"1.0.0\",\"datastreams\": ");
  delay(200);
  //ShowSerialData();
  GPRS_Serial.println("[{\"id\": \"01\",\"current_value\": \"" + barometer + "\"},");
  delay(200);
  //ShowSerialData();
  GPRS_Serial.println("{\"id\": \"02\",\"current_value\": \"" + humidity + "\"},");
  delay(200);
  //ShowSerialData();
  GPRS_Serial.println("{\"id\": \"03\",\"current_value\": \"" + moisture + "\"},");
  delay(200);
  //ShowSerialData();
  GPRS_Serial.println("{\"id\": \"04\",\"current_value\": \"" + temperature + "\"}]},\"token\": \"lee\"}");


  delay(200);
//  ShowSerialData();

  GPRS_Serial.println((char)26);//sending

  delay(4000);//waitting for reply, important! the time is base on the condition of internet 
  //GPRS_Serial.println();

  ShowSerialData();
  //GPRS_Serial.println("AT+CIPCLOSE");//close the connection
  //sendCommand("AT+CIPCLOSE","CLOSE OK");
  //delay(500);
  //ShowSerialData();
}
void ShowSerialData()
{
  Serial.print("SHOW: ");
  while(GPRS_Serial.available()!=0)
    Serial.write(GPRS_Serial.read());
}
int sendCommand(char* command, char* resp, int delayTime, int reps, int numData){
  int returnVal;
  //delay(100);
  for(int i=0;i<reps;i++){
    if (i > 0) delay(500);
    GPRS_Serial.flush();
    delay(100);
    GPRS_Serial.println(command);
    delay(100);
    Serial.print(command);
    Serial.print(": ");
    //delay(10);
    delay(delayTime);
    //    while(GPRS_Serial.available() < numData+1){
    //      unsigned long currentMillis = millis();
    //      
    //      if(currentMillis - previousMillis > delayTime) {
    //        Serial.println("TIMEOUT");
    //        break;
    //      }
    //    }
    long previousMillis = millis();
    //unsigned long currentMillis;
    while(GPRS_Serial.available())  {
//      unsigned long currentMillis = millis();
//      if(currentMillis - previousMillis > delayTime) {
//        Serial.println("TIMEOUT");
//        break;
//      }
      readline();
      //Serial.println("AFTER READLINE");
      if (strncmp(buffer, resp,numData) == 0) {
        Serial.println(buffer);
        return 1;
      }
    }
    Serial.print("FAILED: ");
    Serial.println(buffer);
  }
  return 0;
}

void readline() {
  memset(buffer,0,sizeof(buffer));
  char c;
  int i =0;
  buffidx = 0; // start at begninning
  //Serial.println("BEFORE READLINE");
  long previousMillis = millis();
  while (1) {
      unsigned long currentMillis = millis();
      if(currentMillis - previousMillis > 20000) {
        Serial.println("TIMEOUT");
        return;
      }
    delay(2);
    c=GPRS_Serial.read();
    //Serial.print(buffidx);
    
    if (c == -1)
      continue;
    if (c == '\n')
      continue;
    if ((buffidx == BUFFSIZ-1) || (c == '\r')) {
      buffer[buffidx] = 0;
      return;
    }
    buffer[buffidx++]= c;
    delay(2);
  }
}
/////////////////////////////////////////
///////* Power Functions for GPRS*///////
/////////////////////////////////////////
void powerOn(){
  if(sendCommand("AT","OK",500,4) !=1){
    Serial.println("Powering On...");
    pinMode(9, OUTPUT); 
    digitalWrite(9,LOW);
    delay(1000);
    digitalWrite(9,HIGH);
    delay(2000);
    digitalWrite(9,LOW);
    delay(4000);
    //delay(15500);
    if(sendCommand("AT+CREG?","+CREG: 0,1",500,10,10)==1){
      Serial.println("REGISTERED");
      //Serial.print(".");
    }
    
    power = true;
    setupCommands();
  }
  else{
    Serial.println("ALREADY ON");
    power = true;
  }

}
/////////////////////////////////////////
void powerOff(){
  delay(1000);
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2500);
  digitalWrite(9,LOW);
  power = false;
  //delay(10500);
}



