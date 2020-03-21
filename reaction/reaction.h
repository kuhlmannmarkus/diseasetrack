#ifndef REACTION_H
#define REACTION_H

#include "common/defaults.h"
#include "diseasetrackobject/diseasetrackobject.h"

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class Reaction : public DiseaseTrackObject {
public:
  // Methods
  Reaction();
  ~Reaction();
  // Members
  // Signals
  // Slots
  virtual void AcceptRunModeOrder(int);

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
