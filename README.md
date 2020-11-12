The work to be done on the project:
https://trello.com/b/8H5htqbK/beekeepingdatalogger

# BeeKeepingDataLogger
The purpose of the project is to help a local beekeeper and to gain experience with working with new technologies and programming languages in the context of a project. The purpose is NOT to make a profit.

The goal of the project is to create a system that can measure the temperature and humidity of a group of beehives and log the data in a centralized computer, so the owner can see exactly when his bees start to become active again during the spring. When the bees start to become active again, they raise the temperature of the beehive from around 24 degrees celsius to above 30 degrees celsius. The humidity of the beehives is useful because if the humidity falls below 50%, no eggs are hatched. 

To accomplish this goal, Arduino hardware components and Arduino software are used to create transmitters, receivers and intermediate transceivers). The transmitter exploits Arduino hardware components to measure the temperature and humidity of a beehive. The receiver exploits Arduino hardware components to receive data from transmitters or intermediate transceivers and to log it in an Excel file. The intermediate transceiver exploits Arduino hardware components to receive data from transmitters or intermediate transceivers and to send the data forward to a receiver or an intermediate transceiver.

There are two people working on the project: one who does the software(me) and one who does the hardware(my flatmate).

//======================================================================//

The initial plan of the project is to assist a local beekepper keeping track of the temperature and humidity of his currently 7 beehives. The amount of beehives is excepted to rise but not above 12 (in the nearest future at least).

If the project becomes a succes, another local beekeeper with more than 30 beehives would be willing to acquire the system.

//======================================================================//

The main programs for this project is:
Arduino IDE:
- To develop the code.

Fritzing:
- To create sketches for the configuation of the hardware.

PLX-DAQ v2.11:
- To log the received data in an Excel spreadsheet.

Microsoft Excel
- To log received data and to communicate the results to an end user.

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
