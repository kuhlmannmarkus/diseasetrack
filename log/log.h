//
// Copyright (c) 2019 Markus Kuhlmann
//
// Refer to the LICENSE file for licensing information
//
#ifndef LOG_H
#define LOG_H

#include "sigslot.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>

class Log : public sigslot::has_slots<> {
public:
  // Methods
  Log();
  ~Log();
  // Members
  // Signals
  // Slots
  void AcceptLogMessage(std::string, std::string, std::string);

private:
  // Methods
  // Members
  // Signals
  // Slots
protected:
  // Methods
  // Members
  // Signals
  // Slots
};

#endif // LOG_H
