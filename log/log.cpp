//
// Copyright (c) 2019 Markus Kuhlmann
//
// Refer to the LICENSE file for licensing information
//
#include "log.h"

#if defined(DBGFLAG)
#define DBGMODE true
#endif
#if !defined(DBGFLAG)
#define DBGMODE false
#endif

#define ERR "\033[31mERROR: \033[0m"
#define DBG "\033[33mDEBUG: \033[0m"
#define INFO "\033[32mINFO: \033[0m"
#define UNKOWNSEV "\033[31;43mUNKOWNSEV: \033[0m"

Log::Log() {}

Log::~Log() {}

void Log::AcceptLogMessage(std::string _from, std::string _message,
                           std::string _severity) {
  static std::mutex mtx;
  std::string pfx = "";
  if (_severity == "INFO")
    pfx = INFO;
  else if (_severity == "DBG") {
    pfx = DBG;
    if (!DBGMODE)
      return;
  } else if (_severity == "ERR")
    pfx = ERR;
  else
    pfx = UNKOWNSEV;
  std::time_t tt =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct std::tm *ptm = std::localtime(&tt);
  mtx.lock();
  std::cout << "[" << std::put_time(ptm, "%c") << "] " << pfx << _from << ": "
            << _message << std::endl;
  mtx.unlock();
  return;
}
