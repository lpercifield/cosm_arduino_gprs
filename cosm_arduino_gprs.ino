/*
   ***_-_-_ Air Quality Kite Sensing v00_01 _-_-_***
 
 Circuit:
 * RESET pin connected to: pin 7
 
 Sensors:
 * humidity on                     pin A3  - +5v - GND
 * MQ-7 Carbon Monoxide sensor on  pin A2  - +5v - GND - D14
 * MG-811 Carbon Dioxide sensor on pin A0 - +9v - GND
 * MQ-135 Air Quality sensor on    pin A1 - +5v - GND
 * TMP102 Temperature sensor on    pin A4 (SDA-BLUE) - A5 (SCL-WHITE) - +3v - GND
 
 c.piuggi & l.percifield
 june 2012
 
 modified from
 
 joseph saavedra
 http://jos.ph
 january 2011
 weather tunnel sensors
 
 
 */
 
#include <SoftwareSerial.h>
//#include <SD.h>
//#include <SPI.h>
//#include <Ethernet.h>
//#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//One Wire Temp Sensor

#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 12


OneWire onewire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&onewire);


String feedId = "YOUR FEED ID";
String apiKey = "YOUR API KEY";

DeviceAddress temp = { 0x28, 0x10, 0x6D, 0xDC, 0x3, 0x00,0x00, 0x4F};

//CS_MQ7 MQ7(3, 2); //Citizen Sensor MQ-7 Carbon Monoxide Breakout instance (toggle,indicator)int CoPrev = 0; //for storing CO Data
//int CoPrev = 0;
//const int softwareResetPin = 7; //connect this pin to RESET pin
//const long int resetInterval = 120000; //if no successful connection has been made in 2 min, RESET board

//String dataString; //will hold all sensor data

SoftwareSerial GPRS_Serial(7, 8);
boolean config = false;
boolean power = false;
void setup() {
  //allocate memory reserve for this string to concat
  //dataString.reserve(210);
  //Wire.begin();
  //digitalWrite(softwareResetPin, HIGH); //softwareReset hack
  //pinMode(softwareResetPin, OUTPUT);  //softwareReset hack
  pinMode(6, OUTPUT);
  GPRS_Serial.begin(19200);
  Serial.begin(19200);
  
  Serial.println("start setup");
  tempSensor.begin();
  tempSensor.setResolution(temp, TEMPERATURE_PRECISION);
  tempSensor.requestTemperatures();
  float celcius = tempSensor.getTempC(temp);
  Serial.println(celcius);
  digitalWrite(6,LOW);
  //configModem();
  
  Serial.println("end setup");
  delay(1000);
  
  
}

void loop() {
  powerOn();
  startUp();
  sendStuff();
  powerOff();
  delay(240000);
  //powerOff();
//  Serial.println("Reading Sensors...");
//
//  // read the sensors:
//  Serial.println("sensor 1 - temperature..."); //temperature
//  //tempSensor.requestTemperatures();
//  //float sensor1_reading = tempSensor.getTempC(temp);
//  int sensor1_reading = 12;
//  //sensor1_reading = getTemperature();
////  String dataString;
////  dataString = "Temp,";
////  dataString += String(sensor1_reading);
////  Serial.println(dataString);
////  dataString+="\n\r";
//
//  Serial.println("sensor 2 - humidity..."); //humidity
//  //int sensor2_reading = getHumidity();
//  int sensor2_reading = 56;
////  dataString += "Humidity,";
////  dataString += String(sensor2_reading);
////  dataString+="\n\r";
////  Serial.println(dataString);
//
//  Serial.println("sensor 4 - CO2..."); //CO2
//  //int sensor4_reading = getCO2();
//  int sensor4_reading = 78;
////  dataString += "CO2,";
////  dataString += String(sensor4_reading);
////  dataString += "\n\r";
////  Serial.println(dataString);
//
//  Serial.println("sensor 5 - CO..."); //CO
//  //int sensor5_reading = getCO();
//  int sensor5_reading = 99;
////  dataString += "CO,";
////  dataString +=  String(sensor5_reading);
////  dataString += "\n\r";
////  Serial.println(dataString);
//
//  Serial.println("sensor 6 - Air Qual..."); //Air Quality
//  //int sensor6_reading = getAirQual();
//  int sensor6_reading = 56;
////  dataString += "VOC,";
////  dataString += String(sensor6_reading);
////  dataString +="\n\r\n\r";
////  Serial.println(dataString);
//  
//  int length= 48;//dataString.length();
//  Serial.print("String Length: ");
//  Serial.println(length);
//  
//  String sLength = String(length);
//  if(!config){
//    digitalWrite(6,LOW);
//    //configModem();
//  }
//  //sendToPachube(sLength, sensor1_reading, sensor2_reading, sensor4_reading, sensor5_reading, sensor6_reading);
//  //dataString=" ";
//  Serial.println(config);
//  Serial.println("... end of loop ...");
//  //powerOff();
//  //digitalWrite(6,HIGH);
//  //delay(1000);
  delay(10000);
}


