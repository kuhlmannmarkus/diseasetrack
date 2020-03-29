import socket
import time
import json
import psutil
import requests
import nacl.utils
from nacl.public import PrivateKey, SealedBox

pk = "CJnw5cqBJ153OTIka2tqDKWoOvBKqis2M+7zEj07WTM="

pkserver = nacl.public.PublicKey(pk, encoder=nacl.encoding.Base64Encoder)

print("Server PK: " + pkserver.encode(encoder=nacl.encoding.Base64Encoder))

sksender = PrivateKey.generate()
pksender = sksender.public_key

print("Sender SK: " + sksender.encode(encoder=nacl.encoding.Base64Encoder))
print("Sender PK: " + pksender.encode(encoder=nacl.encoding.Base64Encoder))

cookieobject1 = {
  "UUID": "7lAh57Wd+Mj+yDZuM5NXBIg8WgY7ivGAyWdhcik4s3c=",
  "Timestamp": 1585054159,
  "GPSLatitude": 51.489748,
  "GPSLongitude": 7.236259,
  "BLEAttenuation": -30
}

cookieobject2 = {
  "UUID": "DJnw5cqBJ153OTIka2tqDKWoOvBKqis2M+7zEj07WTM=",
  "Timestamp": 1585054160,
  "GPSLatitude": 51.489747,
  "GPSLongitude": 7.236260,
  "BLEAttenuation": -32
}

cookieobject3 = {
  "UUID": "TJnw5cqBJ153OTIka2tqDKWoOvBKqis2M+7zEj07WTM=",
  "Timestamp": 1585054160,
  "GPSLatitude": 51.489747,
  "GPSLongitude": 7.236260,
  "BLEAttenuation": -32
}

cookie1 = json.dumps(cookieobject1)
cookie2 = json.dumps(cookieobject2)
cookie3 = json.dumps(cookieobject3)

crypto_box = SealedBox(pkserver)

encrypted = crypto_box.encrypt(cookie1, encoder=nacl.encoding.Base64Encoder)
encrypted2 = crypto_box.encrypt(cookie2, encoder=nacl.encoding.Base64Encoder)
encrypted3 = crypto_box.encrypt(cookie3, encoder=nacl.encoding.Base64Encoder)
url = 'https://api.trackcovidcluster.de:12345/json'
print(socket.gethostbyname("api.trackcovidcluster.de"))
x = {
    "Command": "ClusterSubmission",
    "Clusters": [encrypted, encrypted2, encrypted3],
    "UUID": pksender.encode(encoder=nacl.encoding.Base64Encoder)
}
y = json.dumps(x)

z = requests.post(url, data = y)
print(z.text)

