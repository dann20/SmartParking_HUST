import paho.mqtt.client as mqtt
import os
import urllib.parse as urlparse
import add_receipt

MQTT_TOPIC = [("lot1/status",2),("time-in",2),("time-out",2), ("fee", 2), ("user",2)]
MQTT_TOPIC += [("lot2/status",2),("time-in2",2),("time-out2",2), ("fee2", 2), ("user2",2)]
MQTT_TOPIC += [("lot3/status",2),("time-in3",2),("time-out3",2), ("fee3", 2), ("user3",2)]
lot_1 = {}
lot_2 = {}
lot_3 = {}

#Define event callbacks
def on_connect(client, userdata, flags, rc):
    print("rc: " + str(rc))

def on_message(client, obj, msg):
    try:
        payload = str(msg.payload).replace("b'", '').replace("'", '')
        print(str(msg.topic), payload)
        print(lot_1)
        print(lot_2)
        print(lot_3)
        
        if ("user" in lot_1.keys()) and ("fee" in lot_1.keys()):
            add_receipt.insertReceipt(add_receipt.count_receipt(), '1', lot_1["time-in"], lot_1["time-out"], lot_1["user"], lot_1["fee"]) 
            print("ADD LOT 1")
            lot_1.clear()
        if ("user" in lot_2.keys()) and ("fee" in lot_2.keys()):
            add_receipt.insertReceipt(add_receipt.count_receipt(), '2', lot_2["time-in"], lot_2["time-out"], lot_2["user"], lot_2["fee"]) 
            print("ADD LOT 2")
            lot_2.clear()
        if ("user" in lot_3.keys()) and ("fee" in lot_3.keys()):
            add_receipt.insertReceipt(add_receipt.count_receipt(), '3', lot_3["time-in"], lot_3["time-out"], lot_3["user"], lot_3["fee"]) 
            print("ADD LOT 3")
            lot_3.clear()
        
        if "2" in msg.topic:
            if "fee" in msg.topic:
                lot_2[msg.topic.replace('2', '')] = float(payload)
            else:
                lot_2[msg.topic.replace('2', '')] = payload
        elif "3" in msg.topic:
            if "fee" in msg.topic:
                lot_3[msg.topic.replace('3', '')] = float(payload)
            else:
                
                lot_3[msg.topic.replace('3', '')] = payload
        else:
            if "fee" in msg.topic:
                lot_1[msg.topic.replace('1', '')] = float(payload)
            else:
                lot_1[msg.topic.replace('1', '')] = payload
       
        if "NO" in lot_1["lot/status"] and "user" not in lot_1.keys():
            lot_1.clear()
        if "NO" in lot_2["lot/status"] and "user" not in lot_2.keys():
            lot_2.clear()
        if "NO" in lot_3["lot/status"] and "user" not in lot_3.keys():
            lot_3.clear()
    except Exception as er:
        print(er)
def on_publish(client, obj, mid):
    print("mid: " + str(mid))

def on_subscribe(client, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(client, obj, level, string):
    print(string)

mqttc = mqtt.Client()
# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe


# Parse CLOUDMQTT_URL (or fallback to localhost)
url_str = os.environ.get('CLOUDMQTT_URL', 'mqtt://localhost:1883')
url = urlparse.urlparse(url_str)

# Connect
mqttc.username_pw_set(url.username, url.password)
mqttc.connect(url.hostname, url.port)


mqttc.subscribe(MQTT_TOPIC)


# Continue the network loop, exit when an error occurs


rc = 0
while rc == 0:
    rc = mqttc.loop()
print("rc: " + str(rc))
