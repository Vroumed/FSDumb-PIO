from time import sleep
import asyncio
import websocket
import json

from dualsense_controller import DualSenseController

# list availabe devices and throw exception when tzhere is no device detected
device_infos = DualSenseController.enumerate_devices()
if len(device_infos) < 1:
    raise Exception('No DualSense Controller available.')

# flag, which keeps program alive
is_running = True

# create an instance, use fiŕst available device
controller = DualSenseController()

uri = "ws://172.20.10.4/ws"
ws = websocket.WebSocket()
ws.connect(uri)



l2val = 0
r2val = 0
stickval = 0

lstickX = 0
lstickY = 0

def l2_callback(value):
    global l2val
    l2val = value

def r2_callback(value):
    global r2val
    r2val = value

def stick_callback(value):
    global stickval
    stickval = value

def lstickX_callback(value):
    global lstickX
    lstickX = value

def lstickY_callback(value):
    global lstickY
    lstickY = value

def camera_trigger():
    global lstickX
    global lstickY

    data = {
        'cmd' : 2,
        'data' : {
            'headX' : -lstickX,
            'headY' : lstickY
        }
    }
    
    raw_data = json.dumps(data)
    ws.send(raw_data)



def horn_on():
    ws.send('{"cmd": 3, "data": {"horn":1, "frequency": 300 }}')

def horn_off():
    ws.send('{"cmd": 3, "data": {"horn":0, "frequency": 300 }}')

mode = 0
def mode_add():
    global mode
    mode = mode + 1
    if mode > 6:
        mode = 0

    ws.send('{"cmd": 4, "data": {"mode":' + str(mode) + '}}')

def mode_sub():
    global mode
    mode = mode - 1
    if mode < 0:
        mode = 6

    ws.send('{"cmd": 4, "data": {"mode":' + str(mode) + '}}')

def trgger_dir_message():
    global l2val
    global r2val
    global stickval

    speed = 0
    # get which trigger is pressed more
    if l2val > r2val:
        speed = l2val
        direction = -1
    else:
        speed = r2val
        direction = 1

    # get the direction of the stick
    data = {
        'cmd' : 1,
        'data' :  {
            'speed' : speed,
            'thrust' : direction,
            'direction' : -stickval
        }
    }

    raw_data = json.dumps(data)
    ws.send(raw_data)
    


controller.right_trigger.on_change(r2_callback)
controller.left_trigger.on_change(l2_callback)
controller.left_stick_x.on_change(stick_callback)
controller.right_stick_x.on_change(lstickX_callback)
controller.right_stick_y.on_change(lstickY_callback)
controller.btn_cross.on_down(horn_on)
controller.btn_cross.on_up(horn_off)
controller.btn_right.on_down(mode_add)
controller.btn_left.on_down(mode_sub)


# enable/connect the device
controller.activate()

while is_running:
    sleep(0.050)
    trgger_dir_message()
    camera_trigger()

# disable/disconnect controller device
controller.deactivate()