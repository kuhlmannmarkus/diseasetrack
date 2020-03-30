//TrackCOVIDCluster
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
  std::vector<std::tuple<std::string, std::string, UINT64>> toDB;
  toDB.clear();
  cluster_to_tangle.clear();
  Crypto *crypto = new Crypto();
  for(unsigned int i = 0; i < std::get<1>(_clusters).size(); i++){
    std::string debase64 = base64_decode(std::get<1>(_clusters).at(i));
    std::string result = crypto->decrypt(debase64);
    boost::property_tree::ptree pt;
    try {
      std::stringstream data_stream(result);
      read_json(data_stream, pt);
      boost::property_tree::ptree &UUID_tree = pt.get_child("UUID");
      std::string UUID = UUID_tree.get_value<std::string>();
      //boost::property_tree::ptree &BLEAttenuation_tree = pt.get_child("BLEAttenuation");
      //float BLEAttenuation = BLEAttenuation_tree.get_value<float>();
      boost::property_tree::ptree &Timestamp_tree = pt.get_child("Timestamp");
      int Timestamp = Timestamp_tree.get_value<int>();
      //boost::property_tree::ptree &GPSLatitude_tree = pt.get_child("GPSLatitude");
      //float GPSLatitude = GPSLatitude_tree.get_value<float>();
      //boost::property_tree::ptree &GPSLongitude_tree = pt.get_child("GPSLongitude");
      //float GPSLongitude = GPSLongitude_tree.get_value<float>();
      std::tuple<std::string, std::string, UINT64> temp = std::make_tuple(UUID, std::get<0>(_clusters), Timestamp);
      toDB.push_back(temp);
      logMessage("UUID: "+ UUID, "INFO");
      //logMessage("GPSLat: "+ std::to_string(GPSLatitude), "INFO");
      //logMessage("GPSLon: "+ std::to_string(GPSLongitude), "INFO");
      logMessage("TS: "+ std::to_string(Timestamp), "INFO");
      //logMessage("BLEAtt: "+ std::to_string(BLEAttenuation), "INFO");
    } catch (const boost::property_tree::ptree_error &e) {
      continue;
    }
    cluster_to_tangle.push_back(result);
    //logMessage(result, "INFO");
  }
  DB *db = new DB("./");
  db->submitEncounters(toDB);
  delete(db);
  boost::property_tree::ptree message;
  boost::property_tree::ptree clusterarray;
  message.put("Command", "SendMessage");
  for(unsigned int i = 0; i < cluster_to_tangle.size(); i++){
    boost::property_tree::ptree temp;
    temp.put("", cluster_to_tangle.at(i));
    clusterarray.push_back(std::make_pair("", temp));
    //DB *db = new DB("./");
    //delete(db);
  }
  boost::property_tree::ptree ntruarray;
  
  boost::property_tree::ptree arraypart;
  boost::property_tree::ptree uuid;
  Crypto *nc = new Crypto();
  uuid.put("", nc->trunchashbase64(base64_decode(std::get<0>(_clusters))));
  delete(nc);
  arraypart.add_child("UUID", uuid);
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
