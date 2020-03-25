import nacl.utils
from nacl.public import PrivateKey, SealedBox

print("We are now generating your keys!")

skserver = PrivateKey.generate()
pkserver = skserver.public_key

print("Server private key: " + skserver.encode(encoder=nacl.encoding.Base64Encoder))
print("Server public key: " + pkserver.encode(encoder=nacl.encoding.Base64Encoder))

f = open("keys.dat", "w")
f.write(skserver.encode(encoder=nacl.encoding.Base64Encoder))
f.write("\n")
f.write(pkserver.encode(encoder=nacl.encoding.Base64Encoder))
f.close()

print("Done! Please find your keys in base64 in keys.dat!")
