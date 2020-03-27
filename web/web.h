//TrackCOVIDCluster
#ifndef WEB_H
#define WEB_H

#include "api/api.h"
#include "common/defaults.h"
#include "diseasetrackobject/diseasetrackobject.h"

#include "server_http.hpp"
#include "server_https.hpp"
#include "client_http.hpp"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <future>

#include <chrono>
#include <functional>
#include <sstream>
#include <thread>
#include <ctime>
#include <iomanip>

class WS : public DiseaseTrackObject {
public:
  // Methods
  WS();
  WS(int _in, std::string _mode, std::string _pubKey = std::string(),
     std::string _privKey = std::string(),
     std::string _caPubKey = std::string());
  ~WS();
  // Members
  // Signals
  sigslot::signal1<std::tuple<std::string, std::vector<std::string>>>
      MessageSendRequest;
  sigslot::signal1<std::tuple<std::string,std::vector<std::string>>> ClustersDetected;
  // Slots
  virtual void AcceptRunModeOrder(int);
  void AcceptMessageSendRequestForwarding(
      std::tuple<std::string, std::vector<std::string>>);
  void AcceptNewTrustedChannelAddedForwarding(std::string);
  void AcceptNewTrustedEndpointAddedForwarding(std::string);
  void AcceptClustersDetectedForwarding(std::tuple<std::string,std::vector<std::string>>);

private:
  // Methods
  virtual void init();
  virtual void pause();
  virtual void run();
  std::thread *startServer();
  std::thread *killServer();
  virtual const std::string identify();
  // Members
  std::string m_mode;
  std::string m_pubkey;
  std::string m_privkey;
  std::string m_capubkey;
  int m_port;
  SimpleWeb::Server<SimpleWeb::HTTPS> *m_sec_server;
  SimpleWeb::Server<SimpleWeb::HTTP> *m_server;
  std::thread *m_server_thread;
  std::thread *m_kill_thread;
  // Signals
  // Slots
protected:
  // Methods
  // Members
  // Signals
  // Slots
};

class WC {
public:
  // Methods
  WC();
  ~WC();
  void pushMessage(std::string, std::vector<std::string>);
  // Members
  // Signals
  // Slots

private:
  // Methods
  // Members
  SimpleWeb::Client<SimpleWeb::HTTPS> *m_sec_client;
  SimpleWeb::Client<SimpleWeb::HTTP> *m_client;
  // Signals
  // Slots
protected:
  // Methods
  // Members
  // Signals
  // Slots
};

#endif // WEB_H
