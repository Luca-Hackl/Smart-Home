import paho.mqtt.client as mqtt

mqttc=mqtt.Client()
mqttc.connect("localhost", 1883)
mqttc.loop_start()

