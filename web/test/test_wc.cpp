#include "log/log.h"
#include "web/web.h"
#include <ctime>
#include <signal.h>
#include <thread>

sigslot::signal3<std::string, std::string, std::string> LogMessage;
sigslot::signal1<int> RunModeOrder;

const std::string identify() { return std::string("Test_WSS_App"); }

volatile bool killswitch = false;

void signal_handler(int s) {
  if (s == 2) {
    killswitch = true;
    //exit(-1);
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
  LogMessage.connect(log, &Log::AcceptLogMessage);
  WC *wc = new WC();
  LogMessage.emit(identify(), "Starting...", "INFO");
  RunModeOrder.emit(2);
  std::string message = "";
  std::vector<std::string> keys;
  wc->pushMessage(message, keys);
  while (!killswitch) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  delete(wc);
  clock_t end = clock();
  LogMessage.emit(
      identify(),
      "Finished in: " + std::to_string(double(end - begin) / CLOCKS_PER_SEC) +
          "s",
      "INFO");
  LogMessage.disconnect(log);
  delete (log);
  return 0;
}
