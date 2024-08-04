


import rickroll
import json
import websocket

uri = "ws://192.168.1.22/ws"
rickroll.ws = websocket.WebSocket()
rickroll.api_key = "KQCW0LVI"
rickroll.ws.connect(uri)

# authentification
data = {"type": 0, "api_key": rickroll.api_key}
print(json.dumps(data))
exit()

rickroll.ws.send(json.dumps(data))

rickroll.main('urss.mid')