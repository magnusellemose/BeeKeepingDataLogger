// UNO Transmitter

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
   pinMode(7,OUTPUT);  
   delay(10);
   digitalWrite(7,HIGH);
   delay(100);
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
   
}

void loop() {
    measureAndSaveToArray();
    
    send();

    delay(5000);
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
        Serial.println("  Tx failed, no ACK received");
    }
}
