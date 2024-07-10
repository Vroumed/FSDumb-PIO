

data = {'cmd': 5, 'data': {'matrixLeft': [0, 1022, 2046, 4095, 2040, 255, 510, 1020], 'matrixRight': [32704, 8160, 4080, 32752, 16368, 8176, 32752, 2040]}}


import rickroll
import json
import websocket

uri = "ws://192.168.157.137/ws"
rickroll.ws = websocket.WebSocket()
rickroll.ws.connect(uri)


rickroll.ws.send(json.dumps(data))


rickroll.main('urss.mid')