import schedule
import time
import threading
from mqtt import mqttc

from datetime import datetime

newalarmtime = '13:30'

s2 = '00:30' # for example


stop_threads = False

schedule_job = None

def job():
    mqttc.publish("esp8266/all","trigger")


def run():
    global schedule_job

    if schedule_job is not None: #if a schedule job exists cancel it
        schedule.cancel_job(schedule_job)

    format = '%H:%M'
    schedule_job = schedule.every().day.at(str(datetime.strptime(newalarmtime, format) - datetime.strptime(s2, format))).do(job) #if time in brackets is reached it does job() datetime subtracts 30 minutes from alarmtime as 
#the sunrise takes 30 minutes

    while True:
        global stop_threads

        schedule.run_pending()
        time.sleep(1) #check time every second

        if stop_threads:
            break #if stop_threads is True cancel thread


def startthread(terminate, alarmtime):

    global stop_threads, newalarmtime

    if terminate == False:

        newalarmtime = alarmtime[0]
        stop_threads = False

        alarm = threading.Thread(target = run)
        alarm.start()


    elif terminate == True:
        stop_threads = True
