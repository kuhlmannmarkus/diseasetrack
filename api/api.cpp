//
// Copyright (c) 2019 Markus Kuhlmann
//
// Refer to the LICENSE file for licensing information
//

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
  result.push_back(std::make_pair("request", _req));
  return result;
}

boost::property_tree::ptree getAnswerSkeletonForBadRequest() {
  boost::property_tree::ptree result;
  result.put("AppName",
             boost::dll::program_location().filename().string());
  result.put("API version", "0.1");
  boost::property_tree::ptree answer;
  boost::property_tree::ptree status;
  status.put("", "Malformed request");
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
  if (command_s.compare("Some command") == 0) {
    boost::property_tree::ptree res = getAnswerSkeleton(pt);
    boost::property_tree::ptree answer;
    boost::property_tree::ptree status;
    boost::property_tree::ptree answervalue;
    status.put("", "Success");
    answervalue.put("", "Some value");
    answer.push_back(std::make_pair("Status", status));
    answer.add_child("Some key", answervalue);
    res.push_back(std::make_pair("Answer", answer));
    std::stringstream oss;
    write_json(oss, res);
    result = oss.str();
    return result;
  } else if (command_s.compare("Some other command") == 0) {
    
  } 
}

const std::string API::identify() { return std::string("API"); }
