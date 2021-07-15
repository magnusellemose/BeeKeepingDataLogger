//Transmitter

//========================Variables to be set for each individual Arduino========================//
const int transmitterId = 1; 

//The address of the transmitter used when opening writing pipe
const byte transmitterAddress[5] = {'R','x','A','A','A'};

const int noOfAlarms = 3;
int alarmHours[noOfAlarms] = {9,12,16}; //Reads temperature and humidity at 9, 12 and 16 o'clock

//The amount of DHT sensors, used for flow control and to calculate average temperature
const int noOfDhtSensors = 1;

//Array containing the pin numbers that the DHT sensors are connected at
int dhtPins[noOfDhtSensors] = {3};

//All DHT sensors should be defined here, use pin number from the dhtPins-array and DHT11/DHT22 (depending on which type of DHT sensor is connected)
DHT dht1(dhtPins[0], DHT11);

//All DHT sensors are put in an array, used to call methods on each variable in a for-loop
DHT dhtArray[noOfDhtSensors] = {dht1};

/////////////////////////////////////////////////////////////////////////////////
//Struct used to group all the data that is sent
struct DATA {
  int id;
  int avgHumidity;
  float avgTemperature;
  int errorCode;
};

//Included libraries
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


// Radio is created
RF24 radio(CE_PIN, CSN_PIN); 

//The DATA-struct variable used to group data to be sent
DATA dataToSend = {transmitterId,0,0,0}; 

//Counter used when arming the alarms
int nextAlarm = 0;

void setup() {
  //DHT setup
  for(int i = 0; i < noOfDhtSensors; i++){
    dhtArray[i].begin();
  }

  //RFC setup
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  Serial.begin(9600);
  pinMode(7,OUTPUT); // NRF transistor pin

  //Initializes the interrupt pin
  pinMode(interruptPin,INPUT_PULLUP);
}

void loop() {
  //The next alarm is armed
  armNextAlarm();

  //NRF is initialized
  initiateNRF();

  //Average temperature and humidity is measured
  measureAndCalculateAverageTempAndHum();

  //The Arduino tries to send the data
  send();

  //The Arduino is put to sleep and the interrupt is set
  //When interrupt is fired, the Arduino is woken up and continues the loop() 
  sleepAndSetInterrupt();
}

//========================User defined methods========================//


//Initiates the NRF chip, has to be done every time before sending the data 
//Doing this every time redices the amount of errors, could perhaps be improved later on
void initiateNRF(){
  Serial.println("SimpleTx Starting");
  //Starts the interrupt pin
  digitalWrite(7,HIGH); 
  delay(10);

  //Initialize radio properties and opens the writing pipe
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
void sleepAndSetInterrupt(){
  sleep_enable();
  
  //Attaches the interrupt
  attachInterrupt(0, interrupt, LOW); 

  //Puts transmitter to sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 

  // Turns off the transistor to the NRF chip
  digitalWrite(7,LOW); 
  
  //Delay set to give everything time to power down before the Arduino is put to sleep.
  delay(1000); 
  sleep_cpu();
}

//function that sets an arbitraty amount of alarms, the alarms are defined at the top
void armNextAlarm(){
  RTC.setAlarm(ALM1_MATCH_HOURS,(transmitterId*5-5),0,alarmHours[nextAlarm],0);
  RTC.alarm(ALARM_1);
  RTC.squareWave(SQWAVE_NONE);
  RTC.alarmInterrupt(ALARM_1, true); 

  //If last alarm in the array has been used, reset and arm the first alarm in the array
  nextAlarm++;
  if(nextAlarm == noOfAlarms){
    nextAlarm = 0;
  }
}

//Interrupt handler, wakes up the arduino when the interrupt is fired.
void interrupt(){
  //Disable sleep mode
  sleep_disable();
  
  //Removes the interrupt from pin 2;
  detachInterrupt(0); 
}

//Measures the humidity of all connected DHT sensors and returns an average
int calculateAverageHumidity(){
  float tempHumidity = 0;
  
  //Looping through all sensors and measures humidity
  for(int i = 0; i < noOfDhtSensors; i++){
    tempHumidity = tempHumidity + dhtArray[i].readHumidity();
  }

  //Calculating average
  int humidityToReturn = tempHumidity / noOfDhtSensors;
  
  return humidityToReturn;
}

//Measures the temperature of all connected DHT sensors and returns an average
float calculateAverageTemperature(){
  float tempTemperature = 0;

  //Looping through all sensors and measures temperature
  for(int i = 0; i < noOfDhtSensors; i++){
    tempTemperature = tempTemperature + dhtArray[i].readTemperature();
  }

  //Calculating average
  float temperatureToReturn = tempTemperature / noOfDhtSensors;
  
  return temperatureToReturn;
}
