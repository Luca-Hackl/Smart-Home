# Discord bot, cumulative incidence values germany

## Table of contents
* [General info](#general-info)
* [Setup of Raspberry](#setup-of-raspberry)
* [Setup of Raspberry](#setup-of-esp)
* [To-Do](#To-Do)

## General info
This is a local network smarthome. It mainly controls LED strips and it pushes live information about temperature and humidity of the room. 
The LED strip and temp information can only be accessed if you are connected to the same network as the server.

It can also set an alarm and will start to simulate a sunrise to slowly wake up the user. If you set the alarm at 7 am the sunrise will begin to simulate at 6:30 am. 
It takes 30 minutes to reach full brightness.

## Setup of Raspberry 
First of all you have to set up the Raspberry Pi (i used the raspberry pi OS light), download the code and create a .env file with the apps secret key. 
If you want to change the rooms that you can individually control you have to edit the colorwebsites.py file. 

Just add the individual room to:
roomschecked = {
"living-room": True,
"bathroom": True,
"bedroom": True
}

## Setup of ESP 
First of all change:
const char *ssid = "";
const char *password = ""; 
to your specific need. You also need to consider that every ESP needs a unique *ChipID. It's also important to consider that the *area variable needs to have a room that was added
in the roomschecked file on the Raspberry pi.

The variable "*mqtt_server" needs to be changed to your Raspberry Pi IP address. And as a very last setup change you need to change NUMPIXELS to the number of pins on your LEDStrip.
