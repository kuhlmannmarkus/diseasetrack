import socket
import time
import json
import psutil
import requests
import nacl.utils
from nacl.public import PrivateKey, SealedBox

pk = "VH0QivWjENisAcrZGSWkYFsgrcsdaAPuSiXdb9puyUM="

pkserver = nacl.public.PublicKey(pk, encoder=nacl.encoding.Base64Encoder)

print("Server PK: " + pkserver.encode(encoder=nacl.encoding.Base64Encoder))

sksender = PrivateKey.generate()
pksender = sksender.public_key

print("Sender SK: " + sksender.encode(encoder=nacl.encoding.Base64Encoder))
print("Sender PK: " + pksender.encode(encoder=nacl.encoding.Base64Encoder))

cookie1 = pksender.encode(encoder=nacl.encoding.Base64Encoder) + " VH0QivWjENisAcrZGSWkYFsgrcsdaAPuSiXdb9puyUM= " + "50.739004,7.154621 " +"1584894668"
cookie2 = pksender.encode(encoder=nacl.encoding.Base64Encoder) + " VH0QivWjENisAcrZGSWkYFsgrcsdaAPuSiXdb9puyUM= " + "50.739004,7.154621 " +"1584894669"
bob_box = SealedBox(pkserver)
encrypted = bob_box.encrypt(cookie1, encoder=nacl.encoding.Base64Encoder)
encrypted2 = bob_box.encrypt(cookie2, encoder=nacl.encoding.Base64Encoder)
print(encrypted.decode('utf-8'))
print("Enc: " + encrypted)

url = 'https://api.trackcovidcluster.de:12345/json'

myobj = '{"Command":"ClusterSubmission"}'

x = {
    "Command": "ClusterSubmission",
    "Clusters": [encrypted, encrypted2],
    "UUID": pksender.encode(encoder=nacl.encoding.Base64Encoder)
}
y = json.dumps(x)

z = requests.post(url, data = y)
print(z.text)

