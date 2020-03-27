#ifndef CRYPTO_H
#define CRYPTO_H

#include "base64.h"
#include <sodium.h>
#include "cryptopp/cryptlib.h"
#include "cryptopp/keccak.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Crypto{
public:
  Crypto();
  ~Crypto();
  std::string encrypt(std::string, std::string);
  std::string decrypt(std::string);
  std::string getPubKey();
  std::string hash(std::string);
private:
   unsigned char m_sk[crypto_box_SECRETKEYBYTES];
   unsigned char m_pk[crypto_box_PUBLICKEYBYTES];
};

#endif // CRYPTO_H
