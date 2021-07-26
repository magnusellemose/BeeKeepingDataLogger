import serial
import requests
import json

# global variables
# perhaps some variables should be moved to a config file?
url = "http://www.webhook/pseudourl" # use webhook to test http requests?
ser = serial.Serial('COM4')
json_object = None

# function: get serial data and put in JSON object
# data is written to serial port in the order: ID, humidity, temperature
ser_vals = ser.readline().decode('ASCII')
ser_vals = ser_vals.replace("\r\n", "")
dec_vals_list = ser_vals.split(',')

# read and compare incremented value
#if num.txt doesn't exist, create new file and add the appropriate number
saved_counter = -1
#with open('num.txt') as f:
#    int(f.readline, saved_counter)

# for testing
saved_counter = 0
dec_vals_list[0] = 0

if(saved_counter == dec_vals_list[0]):
    print(dec_vals_list[0])
    temp_dict = {}
    temp_dict["id"] = dec_vals_list[1]
    temp_dict["hum"] = dec_vals_list[2]
    temp_dict["temp"] = dec_vals_list[3]

    json_object = json.dumps(temp_dict)
    print(json_object)

# function: make HTTP POST request with JSON object 
if(json_object != None):
    print("sending data")

    #make http request

    #update num.txt number