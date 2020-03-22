#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <signal.h>
#include <thread>

#include "log/log.h"
#include "web/web.h"
#include "reaction/reaction.h"

#include "common/defaults.h"

sigslot::signal3<std::string, std::string, std::string> LogMessage;
sigslot::signal1<int> SendRunModeOrder;
sigslot::signal1<std::vector<std::string>> SendZMQPatterns;

static volatile bool killswitch = false;

const std::string identify() { return std::string("DaemonApp"); }

void signal_handler(int s) {
  std::cout << "Caught Signal: " << s << std::endl;
  if (s == 2) {
    killswitch = true;
  }
}

int main(int argc, const char *argv[]) {
  Log *log = new Log();
  LogMessage.connect(log, &Log::AcceptLogMessage);
  LogMessage.emit(identify(), "Starting...", "INFO");
  WS *ws = new WS(12345, "test");
  ws->LogMessage.connect(log, &Log::AcceptLogMessage);
  SendRunModeOrder.connect(dynamic_cast<DiseaseTrackObject *>(ws),
                           &DiseaseTrackObject::AcceptRunModeOrder);
  Reaction *reaction = new Reaction();
  reaction->LogMessage.connect(log, &Log::AcceptLogMessage);
  ws->ClustersDetected.connect(reaction,
                           &Reaction::AcceptClusters);
  SendRunModeOrder.emit(2);
  LogMessage.emit(identify(), "Started", "INFO");
  while (!killswitch) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  LogMessage.emit(identify(), "Shutting down...", "INFO");
  SendRunModeOrder.emit(0);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  LogMessage.emit(identify(), "Finished.", "INFO");
  SendRunModeOrder.disconnect(ws);
  ws->LogMessage.disconnect(log);
  ws->ClustersDetected.disconnect(reaction);
  reaction->LogMessage.disconnect(log);
  delete(reaction);
  delete (ws);
  LogMessage.disconnect(log);
  delete (log);
  return 0;
}
