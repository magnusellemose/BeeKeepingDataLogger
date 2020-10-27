# BeeKeepingDataLogger
The purpose of the project is to help a local beekeeper and to gain experience with working with new technologies and programming languages in a project. The purpose is NOT to make a profit.

The goal of the project is to create a system that can measure the temperature and humidity of a set of beehives and log the data in a centralized computer, so the owner can see exactly when his bees start to become active again during the spring. When the bees start to become active again, they raise the temperature of the beehive from around 24 degrees celsius to above 30 degrees celsius. The humidity of the beehives is useful because if the humidity falls below 50%, no eggs are hatched. A healthy colony has a 

To accomplish this goal, Arduino hardware components and Arduino software are used to create transmitters and receivers (and intermediate transceiver later on). The transmitter exploits Arduino hardware components to measure the temperature and humidity of a beehive. The receiver exploits Arduino hardware components to receive data from transmitters and to log it in an Excel file.

There are two people working on the project: one who does the software(me) and one who does the hardware(my flatmate).


//======================================================================//

The initial plan of the project is to assist a local beekepper keeping track of the temperature and humidity of his currently 7 beehives. The amount of beehives is excepted to rise but not above 12 in the nearest future.

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
- Not in development yet but should work kind of like a mix between the transmitter and the receiver


//======================================================================//

TODO
Must have:
Intermediate transceiver: receives data from up to 6 transmitters and sends the data to another transceiver or the receiver. Necessary because the receiver can't receive data from more than 6 transmitters or transceivers.
Receiver needs functionality to receive data from more than 1 transmitter, need to be able to receive data from 6 receivers
- Transmitters needs functionality to send their measured data at different times so no sent data is lost
The format of the transmitted data should be improved to be more effective and to take up less space
Data that has to be set individually for the transmitters, transceivers and receivers should appear in the top so they are easier to configure

Should have:
The code for setting the alarms in the transmitter should be able to set an arbitrary amount of alarms at different times. Right now there can only be 2 alarms but the beekeeper could change his mind

Could have:
The receiver could check the received temperature and humidity to find out if the values are too high or too low. The temperature should not fall below 24 degrees or exeed 35 degrees. The humidity should not fall below 40% or exceed 80%. The receiver could send an SMS to the beekepper to alert him when the values are too high or too low. This could also be checked by the transmitters and shown physically at the beehives.
