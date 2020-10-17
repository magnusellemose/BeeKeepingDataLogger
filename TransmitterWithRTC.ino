// UNO Transmitter


/////////////--- Place for varibles to change 
// Set Alarm Hour. 
int time1 = 20;
int time2 = 21;
int hour1 = 17;


/////////////---


// RTC includes
#include <DS3232RTC.h>
#include <avr/sleep.h>
#define interruptPin 2 


//NRF includes
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//DHT includes
#include "DHT.h"

//NRF definitions
#define CE_PIN  9
#define CSN_PIN 10

//DHT definitions
#define DHTPIN 3 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//NRF constants
const byte slaveAddress[5] = {'R','x','A','A','A'};
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
char dataToSend[9]; //testDataToSend = {'0','1','+','0','7','5','0','7','3'};

//has to be unique for each transmitter and between 1 and 5/6
const int transmitterId = 1;

void setup() {
//DHT setup
    dht.begin();

//RFC setup
    pinMode(3,OUTPUT);
    digitalWrite(3,HIGH);
    Serial.begin(9600);

    Serial.println("SimpleTx Starting");
    radio.begin();
    radio.flush_tx();
    radio.setAutoAck(true);
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.stopListening();
    radio.openWritingPipe(slaveAddress);

// Initiates the Alarms, and refreseh them. 
    pinMode(interruptPin,INPUT_PULLUP);
    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    RTC.squareWave(SQWAVE_NONE);

// Arms alarm 1 
    RTC.setAlarm(ALM1_MATCH_HOURS,0,time1,hour1,0);
    RTC.alarm(ALARM_1);
    RTC.squareWave(SQWAVE_NONE);
    RTC.alarmInterrupt(ALARM_1, true);

// Arms Alarm 2
    RTC.setAlarm(ALM2_MATCH_HOURS,0,time2,hour1,0);
    RTC.alarm(ALARM_2);
    RTC.squareWave(SQWAVE_NONE);
    RTC.alarmInterrupt(ALARM_2, true);
    
   
}

void loop() {
    if (minute(RTC.get()==time1 && minute(RTC.get()==time2)  {
       resetAlarms();
       delay(10);
       measureAndSaveToArray();
       send();
    }
   
    sleep_and_alarm();

}

//=====================================================================================//

//Measures humidity and temperature and converts them into the dataToSend-format:
//['x','x','y','y','y','y','z','z','z'] where x=id, y=temperature, z=humidity
void measureAndSaveToArray(){
  int hum = dht.readHumidity();
  float temp = dht.readTemperature();

  //converting id, humidity and temperature to strings to format them into array
  String tId = String(transmitterId);
  String tHum = String(hum);
  String tTemp = String(temp);
  
  //Formatting transmitter id into array
  if(transmitterId < 10){
    dataToSend[0] = '0';
    dataToSend[1] = tId[0];
  }
  else{
    dataToSend[0] = tId[0];
    dataToSend[1] = tId[1];
  }

  //Formatting temperature into array
  if(temp >= 0){
    dataToSend[2] = '+';
  }
 
  if(temp < 100 && temp >= 10){
    dataToSend[3] = tTemp[0];
    dataToSend[4] = tTemp[1];
    dataToSend[5] = tTemp[3];
  }
  else if(temp < 10 && temp > 0){
    dataToSend[3] = '0';
    dataToSend[4] = tTemp[0];
    dataToSend[5] = tTemp[2];
  }
  else if(temp < 0 && temp > -10){
    dataToSend[2] = tTemp[0];
    dataToSend[3] = '0';
    dataToSend[4] = tTemp[1];
    dataToSend[5] = tTemp[3];
  }
  else if(temp < -10){
    dataToSend[2] = tTemp[0];
    dataToSend[3] = tTemp[1];
    dataToSend[4] = tTemp[2];
    dataToSend[5] = tTemp[5];
  }

  //formatting humidity into array
  if(hum < 100){
    dataToSend[6] = '0';
  }
  else{
     dataToSend[6] = '1';
     dataToSend[7] = '0';
     dataToSend[8] = '0';
  }
  
  if(hum < 10){
    dataToSend[7] = '0';
    dataToSend[8] = tHum[0];
  }
  else{
    dataToSend[7] = tHum[0];
    dataToSend[8] = tHum[1];
  }

}

//tries to send the dataToSend
void send() {
    bool succeded = radio.write( &dataToSend, sizeof(dataToSend) );

    Serial.print("Data Sent, sent the array: ");
    Serial.print(dataToSend);
    if (succeded) {
        Serial.println("  Acknowledgement received, data sent succesfully");
    }
    else {
        Serial.println("  Tx failed");
    }
}

void sleep_and_alarm(){
 sleep_enable();
  attachInterrupt(0, interUPT, LOW); 
set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
 time_t t;
 t=RTC.get();
 Serial.println("Sover nu, klokken er: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));
  delay(1000);
  sleep_cpu();
 t=RTC.get();
Serial.println("Vågen nu, klokken er: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));
 
  // Arms the Alarm 1 again 
  if (time1 == minute(t)) {
      RTC.alarm(ALARM_1);
      Serial.println("Slukker alarm 1");
      delay(10);
      RTC.setAlarm(ALM1_MATCH_HOURS,0,time1,time1,0);
      delay(10); 
}
// Arms alarm 2 again
  if (time2 == minute(t)) {
    RTC.alarm(ALARM_2);
    Serial.println("Slukker alarm 2");
    delay(10);
    RTC.setAlarm(ALM2_MATCH_HOURS,0,time2,hour1,0);
    delay(10);

}
}


void interUPT(){
  Serial.println("Interrrupt Fired");//Print message to serial monitor
   sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
delay(1000);
}
