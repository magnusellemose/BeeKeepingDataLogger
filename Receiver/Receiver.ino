//Receiver 

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN  9
#define CSN_PIN 10

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

//The array used for the received data
char dr[9]; 
//Size MUST match format of the data sent from the transmitter
//format: ['x','x','y','y','y','y','z','z','z'] where x=id, y=temperature, z=humidity
//testDataReceived = {'0','1','+','0','7','5','0','7','3'};
bool newData = false;

//temporary arrays to be used when parsing the received data
// char idArray[2];
char tempArray[5];
char humArray[3];

//variables to be used when the data has to be written to a .CSV or .xlsx file (to be used in Excel)
int id;
double temp;
int hum;

void setup() {
  // Starts Serial at 9600
  Serial.begin(9600);
  // Clears all saved data in excel. 
  //Serial.println("CLEARDATA");
  Serial.println("LABEL,DATE,TIME,ID,TEMPERATURE,HUMIDITY");
    
  //Serial.println("SimpleRx Starting");
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();
}

void loop() {
  getData();
  showData();
    
  //delay only used for testing purposes
  delay(5000);
}

//checks if there is new data. If yes, reads the data and sets newData to true.
void getData(){
if ( radio.available() ) {
  radio.read( &dr, sizeof(dr) );
  newData = true;
  }
}

//if new data has been received (newData == true), the data is parsed and put into variables
void showData(){
if(newData == true){
  //Serial.print("Data received, received the array: ");
  //Serial.print(dr);
  Serial.print("DATA,DATE,TIME");
  Serial.print(",");
  
  //Assigns received id to an array and prints it
  char idArray[2];
  idArray[0] = dr[0];
  idArray[1] = dr[1];
  //Serial.print("ID: ");
  id = atoi(idArray);
  Serial.print(id);
  Serial.print(",");
  
  //Assigns received temperature to an array and prints it
  tempArray[0] = dr[2];
  tempArray[1] = dr[3];
  tempArray[2] = dr[4];
  tempArray[3] = '.';
  tempArray[4] = dr[5];
  //Serial.print("Temperature: ");
  temp = atof(tempArray);
  Serial.print(temp); Serial.print(",");

  //Assigns received humidity to an array and prints it
  humArray[0] = dr[6];
  humArray[1] = dr[7];
  humArray[2] = dr[8]; 
  //Serial.print("Humidity: ");
  hum = atoi(humArray);  Serial.println(hum);
  
  //Serial.println();
  newData = false;
  }
}
