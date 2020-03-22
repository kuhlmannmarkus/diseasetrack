import socket
import time
import json
import psutil
import requests
import nacl.utils
from nacl.public import PrivateKey, Box

pk = "VH0QivWjENisAcrZGSWkYFsgrcsdaAPuSiXdb9puyUM="

pkserver = nacl.public.PublicKey(pk, encoder=nacl.encoding.Base64Encoder)

print("Server PK: " + pkserver.encode(encoder=nacl.encoding.Base64Encoder))

sksender = PrivateKey.generate()
pksender = sksender.public_key

print("Sender SK: " + sksender.encode(encoder=nacl.encoding.Base64Encoder))
print("Sender PK: " + pksender.encode(encoder=nacl.encoding.Base64Encoder))

cookie1 = pksender.encode(encoder=nacl.encoding.Base64Encoder) + " VH0QivWjENisAcrZGSWkYFsgrcsdaAPuSiXdb9puyUM= " + "ort " +" zeit"

bob_box = Box(sksender, pkserver)
encrypted = bob_box.encrypt(cookie1, encoder=nacl.encoding.Base64Encoder)
print("Enc: " + encrypted)
url = 'http://localhost:12345/json'

myobj = '{"Command":"ClusterSubmission"}'

x = {
  "Command": "ClusterSubmission",
  "Clusters": [encrypted]
}
y = json.dumps(x)

z = requests.post(url, data = y)
print(z.text)

