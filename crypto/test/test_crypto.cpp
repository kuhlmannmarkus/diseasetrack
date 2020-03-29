#include "crypto/crypto.h"
#include "log/log.h"
#include <ctime>
#include <signal.h>
#include <thread>
#include "base64.h"

sigslot::signal3<std::string, std::string, std::string> LogMessage;

const std::string identify() { return std::string("Test_Crypto_App"); }

void signal_handler(int s) {
  if (s == 2) {
    exit(-1);
  }
}

int main(int argc, const char *argv[]) {
  clock_t begin = clock();
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = signal_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
  Log *log = new Log();
  Crypto *crypto = new Crypto();
  LogMessage.connect(log, &Log::AcceptLogMessage);
  LogMessage.emit(identify(), "Starting...", "INFO");
  
  clock_t end = clock();
  LogMessage.emit(
      identify(),
      "Finished in: " + std::to_string(double(end - begin) / CLOCKS_PER_SEC) +
          "s",
      "INFO");
  std::string message = "Yoda says, do or do not. There is no try!";
  LogMessage.emit(identify(), "Message: " + message, "INFO");
  LogMessage.emit(identify(), "Hash: " + crypto->hash(message), "INFO");
  LogMessage.emit(identify(), "Base64 encoded message: " + base64_encode(reinterpret_cast<const unsigned char *>(message.c_str()), message.length()), "INFO");
  std::string crypt = crypto->encrypt(message, crypto->getPubKey());
  LogMessage.emit(identify(), "Encrypted message in base64: " + base64_encode(reinterpret_cast<const unsigned char *>(crypt.c_str()), crypt.length()), "INFO");
  delete(crypto);
  LogMessage.disconnect(log);
  delete (log);
  return 0;
}
