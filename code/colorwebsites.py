from flask import Blueprint, render_template, session,abort

from flask import Flask, render_template, request, url_for, redirect, jsonify
from flask_login import login_required, current_user, login_user, logout_user
from werkzeug.security import generate_password_hash, check_password_hash

from mqtt import mqttc as mqttc
from mqtt import roomschecked

from alarmthread import startthread

import threading

from wine import dropdown, select_wine

from __main__ import socketio


numbers = ['1', '2', '3', '4', '5', '6', '7', '8', '9']
buttonchange = 0

alarmtime = ['07:00']

colors = Blueprint('colors',__name__)

@colors.route("/", methods=['GET', 'POST'])
@login_required
def main():

    global alarmtime

    if request.method == 'POST':



       if 'alarmtime' in request.form:    #if alarm button was pressed
           newalarmtime = request.form.getlist('alarmtime')
           if newalarmtime != alarmtime:  #if newalarmtime from POST request is different, change alarmtime variable
               alarmtime = newalarmtime
               return alarm()

       if 'rooms' in request.form:
           checked_rooms = request.form.getlist('rooms')

           for room in roomschecked:
               if room in checked_rooms:
                   roomschecked[room] = True
               else:
                   roomschecked[room] = False #if box is checked set room to true, otherwise false


       if 'red' in request.form or 'green' in request.form or 'blue' in request.form:
           number_of_slider_r = request.form['red'] #checks value of sliders once submitted
           number_of_slider_g = request.form['green']
           number_of_slider_b = request.form['blue']

           states = getStates()

           all_numbers = number_of_slider_r + ';' + number_of_slider_g + ';' + number_of_slider_b #puts all values into a single string and pushes it via MQTT

           mqttc.publish('esp8266/CustomColor', all_numbers)

           return render_template('main.html', alarmstate=buttonchange, roomschecked=roomschecked, alarmtime = alarmtime, async_mode=socketio.async_mode, states = getStates())

       elif 'PresetColors' in request.form:
           return PresetColors()


       elif 'CustomColors' in request.form:
           return dropdown()

       elif 'alarm' in request.form:
           return alarm()

       elif 'wine' in request.form:
           return dropdown()

       elif 'LEDOff' in request.form:
          return action('0')
       else:
           print(request.form)

    else:
        return render_template('main.html', alarmstate=buttonchange, roomschecked=roomschecked,alarmtime = alarmtime,async_mode=socketio.async_mode, states = getStates())

@colors.route('/wine', methods = ["GET", "POST"])
def wine():
    if request.method == 'POST':
        data = request.json
        select_wine(data)

    return jsonify(data)
    return redirect('/')

def getStates():
    states =  {"sunset": "329,183,100",
               "relax": "169,279,324",
               "evening": "255,100,100",
               "pink": "355,282,293",
               "green": "100,235,162"
               } #all preset colors


    return states


@colors.route("/PresetColors")
@login_required
def PresetColors():

    return render_template('PresetColors.html', states = getStates())



@colors.route("/CustomColors")
@login_required
def CustomColors():

    return render_template('CustomColors.html')


@colors.route("/<action>")
@login_required
def action(action):
   colors = getStates()
   length = len(colors)


   # If the action part of the URL is "on," execute the code indented below:

   if action in numbers:
       action = int(action) - 1
       values = getStates().values()
       values_list = list(values)

       for key, value in roomschecked.items():
           if value == True:
              chanel = "esp8266/" + key  #post value in every channel that is set to true (living-room, bathroom etc)
              mqttc.publish(chanel,values_list[action])

           else:

              chanel = "esp8266/" + key
              mqttc.publish(chanel,"100,100,100")

   if action == '0':
      mqttc.publish("esp8266/all","0") #make sure to turn all LED's off

   global buttonchange
   return redirect('/')

@colors.route("/alarm")
@login_required
def alarm():

    global buttonchange
    if buttonchange == 0:
        buttonchange = 1
        mqttc.publish("esp8266/all", "alarm1")
        startthread(False, alarmtime)

    else:
        buttonchange = 0
        mqttc.publish("esp8266/all", "alarm0")
        startthread(True, alarmtime)

    return redirect('/')
