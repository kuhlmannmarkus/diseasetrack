//TrackCOVIDCluster
#include "api.h"
#include <iostream>
#include <sstream>

template <typename T>
std::vector<T> as_vector(boost::property_tree::ptree const &pt,
                         boost::property_tree::ptree::key_type const &key) {
  std::vector<T> r;
  for (auto &item : pt.get_child(key))
    r.push_back(item.second.get_value<T>());
  return r;
}

boost::property_tree::ptree
getAnswerSkeleton(boost::property_tree::ptree _req) {
  boost::property_tree::ptree result;
  result.put("AppName",
             boost::dll::program_location().filename().string());
  result.put("API version", "0.1");
  result.push_back(std::make_pair("Request", _req));
  return result;
}

boost::property_tree::ptree getAnswerSkeletonForBadRequest() {
  boost::property_tree::ptree result;
  result.put("AppName",
             boost::dll::program_location().filename().string());
  result.put("API version", "0.1");
  boost::property_tree::ptree answer;
  boost::property_tree::ptree status;
  status.put("", "Malformed Request");
  answer.push_back(std::make_pair("Status", status));
  result.push_back(std::make_pair("Answer", answer));
  return result;
}

API::API() {}

API::~API() {}

API::API(std::string _data) { m_received_data = _data; }

std::string API::evaluate() {
  std::string result = "API breach";
  std::string command_s;
  std::stringstream data_stream(m_received_data);
  boost::property_tree::ptree pt;
  try {
    read_json(data_stream, pt);
    boost::property_tree::ptree &command = pt.get_child("Command");
    command_s = command.get_value<std::string>();
  } catch (const boost::property_tree::ptree_error &e) {
    boost::property_tree::ptree res = getAnswerSkeletonForBadRequest();
    std::stringstream oss;
    write_json(oss, res);
    result = oss.str();
    return result;
  }
  if (command_s.compare("ClusterSubmission") == 0){
    std::cout << "doing it" << std::endl;
    boost::property_tree::ptree res = getAnswerSkeleton(pt);
    boost::property_tree::ptree answer;
    boost::property_tree::ptree status;
    std::vector<std::string> clusters =
        as_vector<std::string>(pt, "Clusters");
    std::cout << "size of cluters: " << clusters.size() << std::endl;
    boost::property_tree::ptree &pk = pt.get_child("UUID");
    std::string senderpk =  pk.get_value<std::string>();
    std::cout << "UUID: " << senderpk << std::endl;
    ClustersDetected.emit(std::make_tuple(senderpk, clusters));
    status.put("", "Success");
    answer.push_back(std::make_pair("Status", status));
    res.push_back(std::make_pair("Answer", answer));
    std::stringstream oss;
    std::cout << "Still alive" << std::endl;
    write_json(oss, res);
    std::cout << "Im here" << std::endl;
    return oss.str();;
  }
  else if (command_s.compare("SubmitWhereabouts") == 0){
    boost::property_tree::ptree &pk_tree = pt.get_child("UUID");
    std::string pk = pk_tree.get_value<std::string>();
    DB *db = new DB("./");
    std::vector<int> res2 = db->obscure(pk);
    //delete(db);
    std::cout << "DB in API:" << res2.size() << std::endl;
    boost::property_tree::ptree res = getAnswerSkeleton(pt);
    boost::property_tree::ptree answer;
    boost::property_tree::ptree status;
    boost::property_tree::ptree answervalue;
    if(res2.size() != 0){
      for(unsigned int i = 0; i < res2.size(); i++){
        boost::property_tree::ptree temp;
        temp.put("", res2.at(i));
        answervalue.push_back(std::make_pair("", temp));
      }
      answer.add_child("Encounters", answervalue);
    }
    std::stringstream streamforenc;
    write_json(streamforenc, answervalue);
    std::string stringtoenc = streamforenc.str();
    std::cout << "STRING HERE: " <<  stringtoenc << std::endl;;
    status.put("", "Success");
    answer.push_back(std::make_pair("Status", status));
    res.push_back(std::make_pair("Answer", answer));
    std::stringstream oss;
    write_json(oss, res);
    result = oss.str();
    delete(db);
    return result;

   }
   else if (command_s.compare("StatePoll") == 0){
    boost::property_tree::ptree &pk_tree = pt.get_child("UUID");
    std::string pk = pk_tree.get_value<std::string>();
    DB *db = new DB("./");
    std::vector<int> res2 = db->obscure(pk);
    delete(db);
    for(unsigned int z = 0; z < res2.size(); z++){
      std::cout << "z: " << z << " cont: " << res2.at(z) << std::endl;
    }
    std::cout << "DB in API:" << res2.size() << std::endl;
    boost::property_tree::ptree res = getAnswerSkeleton(pt);
    boost::property_tree::ptree answer;
    boost::property_tree::ptree status;
    boost::property_tree::ptree answervalue;
    if(res2.size() != 0){  
      for(unsigned int i = 0; i < res2.size(); i++){
	try {
	  std::cout << "i: " << i << " cont: " << res2.at(i) << std::endl;
          boost::property_tree::ptree temp;
          temp.put("", res2.at(i));
          answervalue.push_back(std::make_pair("", temp));
	}
	catch (const boost::property_tree::ptree_error &e) {
	  std::cout << "there was some error " << e.what() << std::endl;
	}
      }
      answer.add_child("Encounters", answervalue);
    }
    std::stringstream streamforenc;
    write_json(streamforenc, answervalue);
    std::string stringtoenc = streamforenc.str();
    //logMessage("STRING HERE: " + stringtoenc, "ERR");
    std::cout << "STRING HERE: " <<  stringtoenc << std::endl;
    status.put("", "Success");
    answer.push_back(std::make_pair("Status", status));
    res.push_back(std::make_pair("Answer", answer));
    std::stringstream oss;
    write_json(oss, res);
    result = oss.str();
    return result;

   }
  else if (command_s.compare("RequestServerPubKey") == 0){
    boost::property_tree::ptree res = getAnswerSkeleton(pt);
    boost::property_tree::ptree answer;
    boost::property_tree::ptree status;
    boost::property_tree::ptree answervalue;
    status.put("", "Success");
    Crypto *crypto = new Crypto();
    std::string pkey = crypto->getPubKey();
    delete(crypto);
    answervalue.put("", pkey);
    answer.push_back(std::make_pair("Status", status));
    answer.add_child("ServerPubKey", answervalue);
    res.push_back(std::make_pair("Answer", answer));
    std::stringstream oss;
    write_json(oss, res);
    result = oss.str();
    return result;

   }
   else{
     boost::property_tree::ptree res = getAnswerSkeletonForBadRequest();
     std::stringstream oss;
     write_json(oss, res);
     result = oss.str();
     return result;
   }  
}

const std::string API::identify() { return std::string("API"); }
