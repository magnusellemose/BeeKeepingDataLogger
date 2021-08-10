import serial
import requests
import json
import os

# global variables
# perhaps some variables should be moved to a config file?
url = "http://www.webhook/pseudourl" # use webhook to test http requests?
# TODO should handle if serial port is incorrect
ser = serial.Serial('COM3')
json_object = None

# data is written to serial port in the order: ID, humidity, temperature
# decode, format and put serial input into list
ser_vals = ser.readline().decode('ASCII')
ser_vals = ser_vals.replace("\r\n", "")
dec_vals_list = ser_vals.split(',')

# read and compare incremented value
#if num.txt doesn't exist, create new file and add the appropriate number
saved_counter = -1
dir_path = os.path.dirname(os.path.realpath(__file__))
num_txt_path = dir_path + "\\num.txt"
if(os.path.exists(num_txt_path)):
    f = open(num_txt_path)
    #print("entered 'open num.txt' block")
    saved_counter = f.read()
    #print("Saved counter is: {}".format(saved_counter))
    f.close()

# for testing
saved_counter = 0
dec_vals_list[0] = 0

#Create json object from decoded values
if(saved_counter == dec_vals_list[0]):
    print(dec_vals_list[0])
    temp_dict = {}
    temp_dict["id"] = dec_vals_list[1]
    temp_dict["hum"] = dec_vals_list[2]
    temp_dict["temp"] = dec_vals_list[3]

    json_object = json.dumps(temp_dict)
    print(json_object)

#Make HTTP POST request with JSON object 
if(json_object != None):
    print("sending data")
    #TODO - add custom headers for security when backend is created
    cust_headers = {}
    r = requests.post(url, json_object, headers=cust_headers)
    
    #If request succeds
    if(r.status_code >=200 and r.status_code < 300):
        new_saved_counter = saved_counter + 1
        f = open(num_txt_path, "w")
        f.write(new_saved_counter)
        f.close()
    else:
        print("server responed with code {}!".format(r.status_code))
        #handle failed request