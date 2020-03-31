import socket
import time
import json
import psutil
import requests
import nacl.utils
from nacl.public import PrivateKey

sksender = PrivateKey.generate()
pksender = sksender.public_key

url = 'https://api.trackcovidcluster.de:12345/json'

x = {
    "Command": "StatePoll",
    "UUID": "dA07FmQ0ByvEiCK/6WvHnN27V78VKuHNgzPf3NFiHRk=",
}

y = json.dumps(x)
print(y)
z = requests.post(url, data = y)
print(z.text)
