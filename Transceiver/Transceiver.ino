//Transceiver

/////////////--- Place for varibles to change 
//The ID of the transceiver
const int transceiverId = 1;

//Amount of transmitters connected to the transceiver
const int connTrans = 0;

const byte receiverAddress[5] = {'0','0','0','0','0'};
// arrays in an array: datatype arrayname[x][y]
const byte transmitterAddresses[5][connTrans];


//////////////////////////////////////////////////////////
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

//The array used for received data
char drArray[9][connTrans];

//Variables for flow control
int drCounter = 0;
bool receivedAllData = false;
bool isReading = false;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(3,5); //delay, count
  
  openPipesForReading();
}

void loop() {
  receiveData();

  if(receivedAllData){
    send();
  }
  //delay for 1 sec, so the transceiver doesn't just spin all the time
  delay(1000);
} 


//========================User defined methods========================//


void receiveData(){
  if(!isReading){
    openPipesForReading();
  }
  //If new data is available, the data is read into the drArray
  if(radio.available()){
    radio.read(&drArray[drCounter], sizeof(drArray));
    drCounter++;
  }

  //if all expected data is received, reset pointer and get ready to send data
  if(drCounter == connTrans){
    receivedAllData = true;
    drCounter = 0;
  }
}

void send(){
  //Open writing pipe to send data
  openPipeForWriting();
  
  //Trying to send all received arrays
  //TODO: Could perhaps be sent as one array? (array of arrays)
  int succededCounter = 0;
  for(int i = 0; i < connTrans; i++){
    radio.write(&drArray[i],sizeof(drArray));
    succededCounter++;
    delay(100);
  }

  //Testing if data was sent succesfully
  bool succeded = false;
  if(succededCounter == connTrans){
    succeded = true;
  }

  //Handling messages for succes and failure
  if(succeded){
    Serial.println("Ack received, data sent succesfully");
  }
  else{
    Serial.println("Tx failed");
  }

  //get ready to receive data again
  receivedAllData = false;
  //Resetting all data in drArray (replacing existing data with '0'
  memset(drArray, '0', sizeof(drArray));
}

void openPipesForReading(){
  for(int i = 0; i < sizeof(transmitterAddresses); i++){
    radio.openReadingPipe(i, transmitterAddresses[i]);
  }
  radio.startListening();
  isReading = true;
}

void openPipeForWriting(){
  radio.stopListening();
  radio.openWritingPipe(receiverAddress);
  isReading = false;
}
