#ifndef REACTION_H
#define REACTION_H

#include "common/defaults.h"
#include "diseasetrackobject/diseasetrackobject.h"
#include "base64.h"
#include "web/web.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <sodium.h>
#include "nacl/crypto_box.h"

class Reaction : public DiseaseTrackObject {
public:
  // Methods
  Reaction();
  ~Reaction();
  // Members
  // Signals
  // Slots
  virtual void AcceptRunModeOrder(int);
  void AcceptClusters(std::tuple<std::string,std::vector<std::string>>);
  
private:
  // Methods
  virtual const std::string identify();
  virtual void init();
  virtual void pause();
  virtual void run();
  std::string execute(std::string);
  // Members
  // Signals
  // Slots
protected:
  // Methods
  // Members
  // Signals
  // Slots
};

#endif // REACTION_H
