The work to be done on the project:
https://trello.com/b/8H5htqbK/beekeepingdatalogger

# BeeKeepingDataLogger
The purpose of the project is to help a local beekeeper and to gain experience with working with new technologies and programming languages in the context of a project. The purpose is NOT to make a profit.

The goal of the project is to create a system that can measure the temperature and humidity of a group of beehives and log the data in a centralized computer, so the owner can see exactly when his bees start to become active again during the spring. When the bees start to become active again, they raise the temperature of the beehive from around 24 degrees celsius to above 30 degrees celsius. The humidity of the beehives is useful because if the humidity falls below 50%, no eggs are hatched. 

To accomplish this goal, Arduino hardware components and Arduino software are used to create transmitters, receivers and intermediate transceivers. The transmitter exploits Arduino hardware components to measure the temperature and humidity of a beehive. The receiver exploits Arduino hardware components to receive data from transmitters or intermediate transceivers and to log it in an Excel file using Microsoft Data Streamer for Excel. The intermediate transceiver exploits Arduino hardware components to receive data from transmitters or intermediate transceivers and to send the data forward to a receiver or an intermediate transceiver. The logged data is automatically backed up by Dropbox and is easily accessible to the developers via TeamViewer. This will serve as the Minimal Viable Product (MVP).

The extended product will include a python script reading data from the Arduino and making HTTP requests to a Azure-hosted backend solution that consists of an ASP.NET WebAPI and an SQL database. The WebAPI handles the HTTP requests and accesses the database to retrieve specific data. The python script will be made to run periodically by registering it in the Task Scheduler. The main goal of the extended product is for me to become more experienced with backend development using these technologies. This will also provide some other benefits, including easy extraction of specific data to create statistical reports. This also makes it possible to add a website to consume the API later on, which is currently out of scope. 
//======================================================================//

The initial plan of the project is to assist a local beekepper keeping track of the temperature and humidity of his currently 7 beehives. The amount of beehives is excepted to rise but not above 12 (in the nearest future).

If the project becomes a succes, another local beekeeper with more than 30 beehives would be willing to acquire the system.

//======================================================================//

Core programs/technologies (MVP):
Arduino IDE:
- To develop the code.

Fritzing:
- To create sketches for the hardware configuration.

Microsoft Datastreamer for Excel:
- To log received data and to communicate the results to an end user.

Dropbox:
- To backup received data.

TeamViewer:
- To access the central computer remotely.



Programs/technologies for extended product:
Visual Studio Code
- To develop the python script.
- Possibly to develop the Arduino code.

Visual Studio
- To develop the WebAPI.

Microsoft SQL Server Management Studio 18
- To manage the SQL server.
- Will maybe use Azure SQL Database.

Task Scheduler:
- To periodically run the python script.

Coding languages:
- Arduino coding language (C/C++)
- C#
- Python
- SQL

Microsoft Azure Cloud Hosting:
- To host the server and database


On some computers, it is necessary to install the CH340g driver to get unoriginal Arduino boards to work. It can be downloaded following this link: 
- 32 bit: https://ardustore.dk/error/CH341SER_32_bit.zip 
- 64 bit: https://ardustore.dk/error/CH341SER_64_bit.zip




//======================================================================//

More technical description of the components:
Transmitter:
- Hardware components:
    - Arduino board
    - DHT chip (to measure temperature and humidity)
    - NRF24l01+ chip (to transmit data)
    - DS3231 I2C Real Time Clock Module (to control when the arduino should wake up and measure and transmit data)
- Used libraries:
    - DS3232RTC
    - avr/sleep
    - SPI
    - nRF24L01
    - RF24
    - DHT

Receiver:
- Hardware components:
    - Arduino board
    - NRF24l01+ chip (to receive data)
- Used libraries
    - SPI
    - nRF24L01
    - RF24

Intermediate transceiver
- Hardware components:
    - Arduino board
    - NRF24l01+ chip (to receive data)
- Used libraries:
    - nRF24L01
    - RF24

//======================================================================//
