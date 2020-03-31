import socket
import json
import requests

url = 'https://api.trackcovidcluster.de:12345/json'

print(socket.gethostbyname("api.trackcovidcluster.de"))

x = {
    "Command": "Bad Command"
}
y = json.dumps(x)

z = requests.post(url, data = y)

print(z.text)

