import paho.mqtt.client as mqtt
from flask_socketio import SocketIO, emit, join_room, leave_room

from __main__ import socketio


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/esp8266/temperature")
    client.subscribe("/esp8266/humidity")

# The callback for when a PUBLISH message is received from the ESP8266.
def on_message(client, userdata, message):
    #socketio.emit('my variable')

    print("Received message " + str(message.payload) + ' on topic ' + message.topic + " with QoS " + str(message.qos))
    if message.topic == "/esp8266/temperature":
        print("temperature update")
        socketio.emit('dht_temperature', message.payload.decode("UTF-8"), broadcast=True)
    if message.topic == "/esp8266/humidity":
        print("humidity update")
        socketio.emit('dht_humidity', message.payload.decode("UTF-8"), broadcast=True)

mqttc=mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.connect("localhost",1883)
mqttc.loop_start()
