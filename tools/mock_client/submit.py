import socket
import time
import json
import psutil
import requests
import nacl.utils
from nacl.public import PrivateKey, SealedBox
from Crypto.Hash import SHA3_256
import pybase64

pk = "CJnw5cqBJ153OTIka2tqDKWoOvBKqis2M+7zEj07WTM="

pkserver = nacl.public.PublicKey(pk, encoder=nacl.encoding.Base64Encoder)

print("Server PK: " + pkserver.encode(encoder=nacl.encoding.Base64Encoder))

sksender = PrivateKey.generate()
pksender = sksender.public_key

print("Sender SK: " + sksender.encode(encoder=nacl.encoding.Base64Encoder))
print("Sender PK: " + pksender.encode(encoder=nacl.encoding.Base64Encoder))

skencounter1 = PrivateKey.generate()
pkencounter1 = skencounter1.public_key
pkenconter1base = pkencounter1.encode(nacl.encoding.Base64Encoder)
h_obj1 = SHA3_256.new()
h_obj1.update(pkencounter1.encode(nacl.encoding.RawEncoder))
pkencounter1hash = h_obj1.digest()
pkencounter1hashbase64 = pybase64.standard_b64encode(pkencounter1hash)
pkencounter1hashbase64trunc = pybase64.standard_b64encode(pkencounter1hash[0:16])
print("Encounter 1 encoded PK: " + pkenconter1base)
print("Encounter 1 hashed and encoded PK: " + pkencounter1hashbase64)
print("Encounter 1 hashed, truncated  and encoded PK: " + pkencounter1hashbase64trunc)

skencounter2 = PrivateKey.generate()
pkencounter2 = skencounter2.public_key
pkenconter2base = pkencounter2.encode(nacl.encoding.Base64Encoder)
h_obj2 = SHA3_256.new()
h_obj2.update(pkencounter2.encode(nacl.encoding.RawEncoder))
pkencounter2hash = h_obj2.digest()
pkencounter2hashbase64 = pybase64.standard_b64encode(pkencounter2hash)
pkencounter2hashbase64trunc = pybase64.standard_b64encode(pkencounter2hash[0:16])
print("Encounter 2 encoded PK: " + pkenconter2base)
print("Encounter 2 hashed and encoded PK: " + pkencounter2hashbase64)
print("Encounter 2 hashed, truncated  and encoded PK: " + pkencounter2hashbase64trunc)

skencounter3 = PrivateKey.generate()
pkencounter3 = skencounter3.public_key
pkenconter3base = pkencounter3.encode(nacl.encoding.Base64Encoder)
h_obj3 = SHA3_256.new()
h_obj3.update(pkencounter3.encode(nacl.encoding.RawEncoder))
pkencounter3hash = h_obj3.digest()
pkencounter3hashbase64 = pybase64.standard_b64encode(pkencounter3hash)
pkencounter3hashbase64trunc = pybase64.standard_b64encode(pkencounter3hash[0:16])
print("Encounter 3 encoded PK: " + pkenconter3base)
print("Encounter 3 hashed and encoded PK: " + pkencounter3hashbase64)
print("Encounter 3 hashed, truncated  and encoded PK: " + pkencounter3hashbase64trunc)

skencounter3 = PrivateKey.generate()
pkencounter3 = skencounter3.public_key

cookieobject1 = {
  "UUID": pkencounter1hashbase64trunc,
  "Timestamp": int(round(time.time())),
}

cookieobject2 = {
  "UUID": pkencounter2hashbase64trunc,
  "Timestamp": int(round(time.time())),
}

cookieobject3 = {
  "UUID": pkencounter3hashbase64trunc,
  "Timestamp": int(round(time.time())),
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
print(x)
z = requests.post(url, data = y)
print(z.text)

