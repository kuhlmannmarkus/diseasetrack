import socket
import time
import json
import requests

url = 'https://api.trackcovidcluster.de:12345/json'

x = {
    "Command": "UUIDPoll",
}

y = json.dumps(x)
print(y)
z = requests.post(url, data = y)
print(z.text)
