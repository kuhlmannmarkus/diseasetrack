#include "crypto.h"

std::string string_to_hex(const std::string &input) {
  static const char hex_digits[] = "0123456789ABCDEF";

  std::string output;
  output.reserve(input.length() * 2);
  for (unsigned char c : input) {
    output.push_back(hex_digits[c >> 4]);
    output.push_back(hex_digits[c & 15]);
  }
  return output;
}

Crypto::Crypto() {
  std::ifstream infile("./keys.dat");
  std::string line;
  std::getline(infile, line);
  std::string sk_s = line;
  std::string sk_debase_s = base64_decode(sk_s);
  std::string line2;
  std::getline(infile, line2);
  infile.close();
  std::string pk_s = line2;
  std::string pk_debase_s = base64_decode(pk_s);
  std::copy(sk_debase_s.begin(), sk_debase_s.end(), m_sk);
  m_sk[sk_debase_s.length()] = 0;
  std::copy(pk_debase_s.begin(), pk_debase_s.end(), m_pk);
  m_pk[pk_debase_s.length()] = 0;
}

Crypto::~Crypto() {}

std::string Crypto::encrypt(std::string _in, std::string _pubkey) {
  std::string result = "";
  unsigned char recipient_pk[crypto_box_PUBLICKEYBYTES + 1];
  std::string recipient_pk_s_debase = base64_decode(_pubkey);
  std::copy(recipient_pk_s_debase.begin(), recipient_pk_s_debase.end(),
            recipient_pk);
  recipient_pk[recipient_pk_s_debase.length()] = 0;
  unsigned char ciphertext[crypto_box_SEALBYTES + _in.length()];
  crypto_box_seal(ciphertext,
                  reinterpret_cast<const unsigned char *>(_in.c_str()),
                  _in.length(), recipient_pk);
  for (unsigned int i = 0; i < crypto_box_SEALBYTES + _in.length(); i++) {
    result += ciphertext[i];
  }
  return result;
}

std::string Crypto::decrypt(std::string _in) {
  std::string result = "";
  unsigned char decrypted[_in.length() - crypto_box_SEALBYTES];
  int ret = crypto_box_seal_open(
      decrypted, reinterpret_cast<const unsigned char *>(_in.c_str()),
      _in.length(), m_pk, m_sk);
  if (ret != 0)
    return std::string("");
  for (unsigned int i = 0; i < _in.length() - crypto_box_SEALBYTES; i++) {
    result += decrypted[i];
  }
  return result;
}

std::string Crypto::getPubKey() {
  std::ifstream infile("./keys.dat");
  std::string line;
  std::getline(infile, line);
  line = "";
  std::getline(infile, line);
  infile.close();
  return line;
}

std::string Crypto::hash(std::string _in) {
  std::string digest;
  CryptoPP::SHA3_256 hash;
  // std::cout << "Name: " << hash.AlgorithmName() << std::endl;
  // std::cout << "Digest size: " << hash.DigestSize() << std::endl;
  // std::cout << "Block size: " << hash.BlockSize() << std::endl;
  hash.Update((const byte *)_in.data(), _in.size());
  digest.resize(hash.DigestSize());
  hash.Final((byte *)&digest[0]);
  // return base64_encode(reinterpret_cast<const unsigned char
  // *>(digest.c_str()), digest.length());
  return string_to_hex(digest);
}

std::string Crypto::trunchash(std::string _in) {
  std::string digest;
  CryptoPP::SHA3_256 hash;
  hash.Update((const byte *)_in.data(), _in.size());
  digest.resize(hash.DigestSize() / 2);
  hash.TruncatedFinal((byte *)&digest[0], digest.size());
  return string_to_hex(digest);
}

std::string Crypto::trunchashbase64(std::string _in) {
  std::string digest;
  CryptoPP::SHA3_256 hash;
  hash.Update((const byte *)_in.data(), _in.size());
  digest.resize(hash.DigestSize() / 2);
  hash.TruncatedFinal((byte *)&digest[0], digest.size());
  return base64_encode(reinterpret_cast<const unsigned char *>(digest.c_str()),
                       digest.length());
}
