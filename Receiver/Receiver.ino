//Receiver 

/////////////--- Place for varibles that should be set for each individual receiver
//The ID of the transceiver
const int receiverId = 1;

//Amount of transmitters connected to the receiver
const int connTrans = 1;

// arrays in an array: datatype arrayname[x][y]
const byte transmitterAddresses[connTrans][5] = {{'R','x','A','A','A'}};

//////////////////////////////////////////////////////////

struct DATA {
  int id;
  int avgHumidity;
  float avgTemperature;
  int errorCode;
};

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);

//Temporary DATA-struct variable used during the radio.read function
DATA receivedData = {0,0,0,0};

//Array containing all received data, placed in DATA-structs
DATA receivedDataArray[connTrans];

//variables for flow control
int receivedDataCounter = 0;
bool receivedAllData = false;

//Temporary variables to be used when the data has to be written to Excel Data Streamer
int id;
int hum;
float temp;
int errorCode;

void setup() {
  Serial.begin(9600);
    
  //Initialize radio and open reading pipes
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  openPipesForReading();
  radio.startListening();
}

void loop() {
  getData();

  if(receivedAllData){
    printDataToExcelDataStreamer();
  }
  
  //delay only used for testing purposes
  //delay(5000);
}


//========================User defined methods========================//

//checks if there is new data. If yes, reads the data and sets newData to true.
void getData(){
  if ( radio.available() ) {
    //Read data is placed in a temporary variable because of how the radio.read function works
    radio.read( &receivedData, sizeof(receivedData) );
    
    //Adds the received data to an array to be used when printing the data
    receivedDataArray[connTrans] = {receivedData.id, receivedData.avgHumidity, receivedData.avgTemperature, receivedData.errorCode};

    //Resetting receivedData-variable and incrementing receivedDataCounter
    receivedData = {0,0,0,0};
    receivedDataCounter++;
  }

  //if all expected data is received, reset pointer and be ready to print the data
  if(receivedDataCounter == connTrans){
    receivedDataCounter = 0;
    receivedAllData = true;
  }
}

//The received data is parsed and shown in an Excel-file
void printDataToExcelDataStreamer(){
  for(int i = 0; i < connTrans; i++){
    //Local variable used to extract data from the receivedData-object
    DATA tempData = receivedDataArray[i];
    
    //Extracts the received ID and prints it
    id = tempData.id;
    Serial.print(id);
    Serial.print(",");

    //Extracts the received average humidity and prints it
    hum = tempData.avgHumidity;
    Serial.print(hum);
    Serial.print(",");
    
    //Extracts the received average temperature and prints it
    temp = tempData.avgTemperature;
    Serial.print(temp); 
    Serial.println();
  }
  
  //get ready to receive data again
  receivedAllData = false;

  //Resetting the data of the receivedData-struct by setting all values to 0
  for(int i = 0; i < connTrans; i++){
    receivedDataArray[i] = {0,0,0,0};
  }
}

//Opens the reading pipe for all transmitters that send data to this receiver
void openPipesForReading(){
  for(int i = 0; i < connTrans; i++){
    radio.openReadingPipe(i, transmitterAddresses[i]);
  }
  radio.startListening();
}
