from requests.models import HTTPError
import serial
import requests
import json
import os

# global variables
# perhaps some variables should be moved to a config file?
url = "http://www.webhook/pseudourl" # use webhook to test http requests?
pos_serials = ['COM3', 'COM4', 'COM5', 'COM6']
ser_obj = serial.Serial(pos_serials[0])
json_obj = None

#If serial port is closed, try other possible physical or virtual ports
if(ser_obj.closed):
    i = 1
    ser_list_size = pos_serials.count()
    while(i <= ser_list_size):
        ser_obj = serial.Serial(pos_serials[i])
        if(ser_obj.isOpen()):
            break
        i+1
    if(i==ser_list_size):
            print('Failed to connect to a serial port')
            quit()

# data is written to serial port in the order: ID, humidity, temperature
# decode, format and put serial input into list
ser_vals = ser_obj.readline().decode('ASCII')
ser_vals = ser_vals.replace("\r\n", "")
dec_vals_list = ser_vals.split(',')

# read and compare incremented value
saved_counter = -1
dir_path = os.path.dirname(os.path.realpath(__file__))
num_txt_path = dir_path + "\\num.txt"
if(os.path.exists(num_txt_path)):
    f = open(num_txt_path)
    saved_counter = f.read()
    f.close()
else:
    print("couldn't find 'num.txt file")
    quit()

# for testing
saved_counter = 0
dec_vals_list[0] = 0

#Create json object from decoded values
#TODO - only handles data from one beehive as of now, should handle data from an arbitrary amount
if(saved_counter == dec_vals_list[0]):
    #print(dec_vals_list[0])
    temp_dict = {}
    temp_dict["id"] = dec_vals_list[1]
    temp_dict["hum"] = dec_vals_list[2]
    temp_dict["temp"] = dec_vals_list[3]

    json_obj = json.dumps(temp_dict)
    #print(json_obj)

#Make HTTP POST request with JSON object 
if(json_obj != None):
    #print("sending data")
    #TODO - add custom headers for security when backend is created
    cust_headers = {}
    try:
        r = requests.post(url, json_obj, headers=cust_headers)
        r.raise_for_status()
    except(HTTPError):
        print('request returned unsuccesful status code: {}'.format(r.status_code))
        quit() 
    except(ConnectionError):
        print('request failed due to a problem with the internet connection')
        quit()
    
    #If request succeds, rewrite num.txt with incremented counter
    if(r.status_code >=200 and r.status_code <= 300):
        new_saved_counter = saved_counter + 1
        f = open(num_txt_path, "w")
        f.write(new_saved_counter)
        f.close()
    else:
        print('An unexpected error occured with the request')
        quit()