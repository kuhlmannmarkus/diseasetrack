// TrackCOVIDCluster
#include "web.h"

WS::WS() {}

WS::WS(int _in, std::string _mode, std::string _pubKey, std::string _privKey,
       std::string _caPubKey) {
  m_sec_server = NULL;
  m_server = NULL;
  m_mode = _mode; // TODO There is no mode anymore
  m_pubkey = _pubKey;
  m_privkey = _privKey;
  m_capubkey = _caPubKey;
  m_port = _in;
}

WS::~WS() {
  logMessage("Killing Webserver...", "INFO");
  m_kill_thread = killServer();
  m_kill_thread->join();
  m_server_thread->join();
  delete (m_server_thread);
  delete (m_kill_thread);
  delete (m_server);
  delete (m_sec_server);
}

void WS::AcceptRunModeOrder(int _in) {
  forwardRunModeOrder(_in);
  if (_in == RUNNING)
    run();
  return;
}

void WS::init() { return; }

void WS::pause() { return; }

void WS::run() {
  m_server_thread = startServer();
  return;
}

std::thread *WS::startServer() {
  std::thread *server_thread;
  if (m_pubkey.length() != 0 && m_privkey.length() != 0) {
    m_sec_server = new SimpleWeb::Server<SimpleWeb::HTTPS>(m_pubkey, m_privkey,
                                                           m_capubkey);
    m_sec_server->config.port = m_port;
    m_sec_server->resource["^/json$"]["POST"] =
        [this](std::shared_ptr<SimpleWeb::Server<SimpleWeb::HTTPS>::Response>
                   response,
               std::shared_ptr<SimpleWeb::Server<SimpleWeb::HTTPS>::Request>
                   request) {
          std::string answer;
          try {
            boost::property_tree::ptree pt;
            read_json(request->content, pt);
            std::stringstream oss;
            write_json(oss, pt);
            API *api = new API(oss.str());
            api->ClustersDetected.connect(
                this, &WS::AcceptClustersDetectedForwarding);
            answer = api->evaluate();
            api->ClustersDetected.disconnect(this);
            delete (api);
            *response << "HTTP/1.1 200 OK\r\n"
                      << "Content-Length: " << answer.length() << "\r\n\r\n"
                      << answer;
          } catch (const std::exception &e) {
            answer = "API breach";
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: "
                      << answer.length() << "\r\n\r\n"
                      << answer;
          }
        };
    server_thread = new std::thread([this]() { m_sec_server->start(); });
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } else {
    logMessage("Spinning up Webserver on port: " + std::to_string(m_port),
               "DBG");
    m_server = new SimpleWeb::Server<SimpleWeb::HTTP>();
    m_server->config.port = m_port;

    m_server->resource["^/json$"]["POST"] =
        [this](std::shared_ptr<SimpleWeb::Server<SimpleWeb::HTTP>::Response>
                   response,
               std::shared_ptr<SimpleWeb::Server<SimpleWeb::HTTP>::Request>
                   request) {
          std::string answer;
          try {
            boost::property_tree::ptree pt;
            read_json(request->content, pt);
            std::stringstream oss;
            write_json(oss, pt);
            API *api = new API(oss.str());
            api->ClustersDetected.connect(
                this, &WS::AcceptClustersDetectedForwarding);
            answer = api->evaluate();
            api->ClustersDetected.disconnect(this);
            delete (api);
            *response << "HTTP/1.1 200 OK\r\n"
                      << "Content-Length: " << answer.length() << "\r\n\r\n"
                      << answer;
          } catch (const std::exception &e) {
            answer = "API breach";
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: "
                      << answer.length() << "\r\n\r\n"
                      << answer;
          }
        };
    server_thread = new std::thread([this]() { m_server->start(); });
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return server_thread;
}

std::thread *WS::killServer() {
  std::thread *kill_thread = new std::thread([this]() {
    if (m_sec_server != NULL)
      m_sec_server->stop();
    if (m_server != NULL)
      m_server->stop();
  });
  return kill_thread;
}

const std::string WS::identify() { return std::string("WS"); }

void WS::AcceptClustersDetectedForwarding(
    std::tuple<std::string, std::vector<std::string>> _clusters) {
  ClustersDetected.emit(_clusters);
  return;
}

WC::WC() {}

WC::~WC() {}

void WC::pushMessage(std::string _message) {
  m_client = new SimpleWeb::Client<SimpleWeb::HTTP>("localhost:1234");
  m_client->request("POST", "/json", _message);
  delete (m_client);
  return;
}
