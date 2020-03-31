// TrackCOVIDCluster
#ifndef API_H
#define API_H

#include <boost/dll.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "base64.h"
#include "common/defaults.h"
#include "crypto/crypto.h"
#include "db/db.h"
#include "diseasetrackobject/diseasetrackobject.h"

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
  std::string getAnswerForBadRequest();
  // Members
  // Signals
  sigslot::signal1<std::tuple<std::string, std::vector<std::string>>>
      ClustersDetected;
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
