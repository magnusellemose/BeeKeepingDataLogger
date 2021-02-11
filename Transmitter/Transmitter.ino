//Transmitter

//========================Variables to be set for each individual Arduino========================//
const int transmitterId = 1; 

//The address of the transmitter
//TODO: is it really necessary to have this here?
const byte transmitterAddress[5] = {'R','x','A','A','A'};

const int noOfAlarms = 3;
int alarmHours[noOfAlarms] = {9,12,16};

//The amount of DHT sensors, used for flow control and to calculate average temperature
const int noOfDhtSensors = 1;
int dhtPins[noOfDhtSensors] = {3};

/////////////////////////////////////////////////////////////////////////////////
//Struct used to group all the data that is sent
struct DATA {
  int id;
  int avgHumidity;
  float avgTemperature;
  int errorCode;
};

// RTC includes
#include <DS3232RTC.h>
#include <avr/sleep.h>
//NRF includes
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//DHT includes
#include "DHT.h"


#define interruptPin 2 
//NRF definitions
#define CE_PIN  9
#define CSN_PIN 10


//All DHT sensors should be defined here, use pin number from the dhtPins-array and DHT11/DHT22 
DHT dht1(dhtPins[0], DHT11);

//All DHT sensors are put in an array, used to call methods on each variable in a loop
DHT dhtArray[noOfDhtSensors] = {dht1};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

//The array used for received data
DATA dataToSend = {transmitterId,0,0,0}; 

//time_t variable used for testing purposes during sleep and wake up
 time_t t;

int nextAlarm = 0;

void setup() {
  //DHT setup
  dht1.begin();

  //RFC setup
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  Serial.begin(9600);
  pinMode(7,OUTPUT); // NRF trnsistor pin

  //Initializes the interrupt pin
  pinMode(interruptPin,INPUT_PULLUP);

  //Arms the first alarm
  armNextAlarm();
}

void loop() {
  initiateNRF();
    
  measureAndCalculateAverageTempAndHum();
    
  send();
    
  sleepAndWakeUp();

  armNextAlarm();
}

//========================User defined methods========================//


//Initiates the NRF chip, has to be done every time before sending the data
//Also starts the interruptpin (pin 7)
void initiateNRF(){
  Serial.println("SimpleTx Starting");
  digitalWrite(7,HIGH);
  delay(10);
    
  radio.begin();
  radio.flush_tx();
  radio.setAutoAck(true);
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3,5); // delay, count
  radio.stopListening();
  radio.openWritingPipe(transmitterAddress);
  delay(10);
}

//Measures the average humidity and temperature and saves those data in the dataToSend-struct
void measureAndCalculateAverageTempAndHum(){
  int hum = calculateAverageHumidity();
  float temp = calculateAverageTemperature();

  dataToSend.avgHumidity = hum;
  dataToSend.avgTemperature = temp;
}

//tries to send the dataToSend
void send() {
  bool succeded = radio.write( &dataToSend, sizeof(dataToSend) );
    
  if (succeded) {
      Serial.println("  Acknowledgement received, data sent succesfully");
      dataToSend.avgHumidity = 0;
      dataToSend.avgTemperature = 0;
      dataToSend.errorCode = 0;
  }
  else {
      Serial.println("  Tx failed");
  }
}

//Sets the interrupt, puts the transmitter to sleep and handles the waking up 
void sleepAndWakeUp(){
  sleep_enable();
  
  //Attaches the interrupt
  attachInterrupt(0, interrupt, LOW); 

  //Puts transmitter to sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  t=RTC.get();
  Serial.println("Sover nu, klokken er: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));
  digitalWrite(7,LOW); // Turns off the transistor to the NRF chip
  delay(1000); //gives time for everything to turn off before the transmitter is put to sleep
  sleep_cpu();

  //Prints the time when the sleep process is interrupted and the transmitter wakes up
  t=RTC.get();
  Serial.println("Vågen nu, klokken er: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));
}

//function that sets an arbitraty amount of alarms, the alarms are defined at the top
void armNextAlarm(){
  RTC.setAlarm(ALM1_MATCH_HOURS,(transmitterId*5-5),0,alarmHours[nextAlarm],0);
  RTC.alarm(ALARM_1);
  RTC.squareWave(SQWAVE_NONE);
  RTC.alarmInterrupt(ALARM_1, true); 

  nextAlarm++;
  if(nextAlarm == noOfAlarms){
    nextAlarm = 0;
  }
}

//used to interrupt the sleeping process 
void interrupt(){
  Serial.println("Interrrupt Fired");
  sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
  delay(1000);
}

//Measures the humidity of all connected DHT sensors and returns an average
int calculateAverageHumidity(){
  float tempHumidity = 0;
  for(int i = 0; i < noOfDhtSensors; i++){
    tempHumidity = tempHumidity + dhtArray[i].readHumidity();
  }

  int humidityToReturn = tempHumidity / noOfDhtSensors;
  
  return humidityToReturn;
}

//Measures the temperature of all connected DHT sensors and returns an average
float calculateAverageTemperature(){
  float tempTemperature = 0;
  for(int i = 0; i < noOfDhtSensors; i++){
    tempTemperature = tempTemperature + dhtArray[i].readTemperature();
  }

  float temperatureToReturn = tempTemperature / noOfDhtSensors;
  
  return temperatureToReturn;
}
