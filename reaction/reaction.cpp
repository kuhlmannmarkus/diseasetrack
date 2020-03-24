#include "reaction.h"

Reaction::Reaction() {}

Reaction::~Reaction() {}

const std::string Reaction::identify() { return std::string("Reaction"); }

void Reaction::init() { return; }

void Reaction::pause() { return; }

void Reaction::run() { return; }

void Reaction::AcceptRunModeOrder(int _in) { return; }

void Reaction::AcceptClusters(std::tuple<std::string,std::vector<std::string>> _clusters){
  std::vector<std::string> cluster_to_tangle;
  cluster_to_tangle.clear();
  Crypto *crypto = new Crypto();
  for(unsigned int i = 0; i < std::get<1>(_clusters).size(); i++){
    std::string debase64 = base64_decode(std::get<1>(_clusters).at(i));
    std::string result = crypto->decrypt(debase64);
    cluster_to_tangle.push_back(result);
    //logMessage(result, "INFO");
  }
  boost::property_tree::ptree message;
  boost::property_tree::ptree clusterarray;
  message.put("Command", "SendMessage");
  for(unsigned int i = 0; i < cluster_to_tangle.size(); i++){
    boost::property_tree::ptree temp;
    temp.put("", cluster_to_tangle.at(i));
    clusterarray.push_back(std::make_pair("", temp));
  }
  boost::property_tree::ptree ntruarray;
  
  boost::property_tree::ptree arraypart;
  arraypart.add_child("Clusters", clusterarray);
  message.add_child("MessageBody", arraypart);
  message.add_child("NTRUPubKeyArray", ntruarray);
  std::stringstream oss;
    write_json(oss, message);
    //logMessage(oss.str(), "INFO");
    std::vector<std::string> ntrukeys;
    
    WC *wc = new WC();
    wc->pushMessage(oss.str(), ntrukeys);
    delete(wc);
  return;
}
