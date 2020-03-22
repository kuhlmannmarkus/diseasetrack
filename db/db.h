#ifndef DB_H
#define DB_H

#include <mutex>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "dirent.h"

class DB{
public:
  // Methods
  DB();
  ~DB();
  std::vector<std::tuple<std::string,std::string,std::string,std::string>>  getEncounters(std::string);
  void submitEncounters(std::vector<std::tuple<std::string,std::string,std::string,std::string>>);
  // Members
  // Signals
  // Slots
private:
  // Methods
  // Members
  std::mutex mtx;
  // Signals
  // Slots
protected:
  // Methods
  // Members
  // Signals
  // Slots
};

#endif // DB_H
