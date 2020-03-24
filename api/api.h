#ifndef API_H
#define API_H

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/dll.hpp>

#include "common/defaults.h"
#include "diseasetrackobject/diseasetrackobject.h"
#include "crypto/crypto.h"

#include <algorithm>
#include <fstream>
#include <sstream>

class API : public DiseaseTrackObject {
public:
  // Methods
  API();
  ~API();
  API(std::string);
  std::string evaluate();
  // Members
  // Signals
  sigslot::signal1<std::tuple<std::string,std::vector<std::string>>> ClustersDetected;
  // Slots
private:
  // Methods
  virtual const std::string identify();
  // Members
  std::string m_received_data;
  // Signals
  // Slots
protected:
  // Methods
  // Members
  // Signals
  // Slots
};

#endif // API_H
