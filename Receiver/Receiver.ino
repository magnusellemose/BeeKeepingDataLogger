//Receiver 

/////////////--- Place for varibles to change 
//The ID of the transceiver
const int receiverId = 1;

//Amount of transmitters connected to the receiver
const int connTrans = 1;

// arrays in an array: datatype arrayname[x][y]
const byte transmitterAddresses[5][connTrans] = {'R','x','A','A','A'};


//////////////////////////////////////////////////////////
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);

//The array used for the received data
char drArray[9][connTrans]; 
//char testDataReceived[9] = {'0','1','+','0','7','5','0','7','3'};

//variables for flow control
int drCounter = 0;
bool receivedAllData = false;

//temporary arrays to be used when parsing the received data
// char idArray[2];
char tempArray[5];
char humArray[3];

//variables to be used when the data has to be written in the Excel-file
int id;
double temp;
int hum;

void setup() {
  Serial.begin(9600);
  // Clears all saved data in excel. 
  //Serial.println("CLEARDATA");
  Serial.println("LABEL,DATE,TIME,ID,TEMPERATURE,HUMIDITY");
    
  //Serial.println("SimpleRx Starting");
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  openPipesForReading();
  radio.startListening();
}

void loop() {
  getData();

  if(receivedAllData){
    showData();
  }
  
  //delay only used for testing purposes
  delay(5000);
}


//========================User defined methods========================//


//checks if there is new data. If yes, reads the data and sets newData to true.
void getData(){
  if ( radio.available() ) {
    radio.read( &drArray[drCounter], sizeof(drArray) );
    drCounter++;
  }

  //if all expected data is received, reset pointer
  if(drCounter == connTrans){
    drCounter = 0;
    receivedAllData = true;
  }
}

//The received data is parsed and shown in an Excel-file
void showData(){
  for(int i = 0; i < sizeof(drArray); i++){
    //Serial.print(DParsing the array: ");
    //Serial.print(drArray[i]);
    Serial.print("DATA,DATE,TIME");
    Serial.print(",");
  
    //Assigns received id to an array and prints it
    char idArray[2];
    idArray[0] = drArray[0][i];
    idArray[1] = drArray[1][i];
    //Serial.print("ID: ");
    id = atoi(idArray);
    Serial.print(id);
    Serial.print(",");
  
    //Assigns received temperature to an array and prints it
    tempArray[0] = drArray[2][i];
    tempArray[1] = drArray[3][i];
    tempArray[2] = drArray[4][i];
    tempArray[3] = '.';
    tempArray[4] = drArray[5][i];
    //Serial.print("Temperature: ");
    temp = atof(tempArray);
    Serial.print(temp); Serial.print(",");

    //Assigns received humidity to an array and prints it
    humArray[0] = drArray[6][i];
    humArray[1] = drArray[7][i];
    humArray[2] = drArray[8][i]; 
    //Serial.print("Humidity: ");
    hum = atoi(humArray);  Serial.println(hum);
    //Serial.println();
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
}
