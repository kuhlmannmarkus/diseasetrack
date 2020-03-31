#ifndef CRYPTO_H
#define CRYPTO_H

#include "base64.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"
#include <sodium.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Crypto {
public:
  Crypto();
  ~Crypto();
  std::string encrypt(std::string, std::string);
  std::string decrypt(std::string);
  std::string getPubKey();
  std::string hash(std::string);
  std::string trunchash(std::string);
  std::string trunchashbase64(std::string);

private:
  // unsigned char m_sk[crypto_box_SECRETKEYBYTES];
  // unsigned char m_pk[crypto_box_PUBLICKEYBYTES];
  unsigned char m_sk[32];
  unsigned char m_pk[33];
};

#endif // CRYPTO_H
