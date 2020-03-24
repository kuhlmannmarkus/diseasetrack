#include "crypto/crypto.h"
#include "log/log.h"
#include <ctime>
#include <signal.h>
#include <thread>

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
  
  delete(crypto);
  LogMessage.disconnect(log);
  delete (log);
  return 0;
}
