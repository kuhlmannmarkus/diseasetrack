#include "db.h"

DB::DB() {}

DB::~DB() {}

std::vector<std::tuple<std::string,std::string,std::string,std::string>> DB::getEncounters(std::string _ID){
  std::vector<std::tuple<std::string,std::string,std::string,std::string>> result;
  result.clear();
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ("./storage")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      printf ("%s\n", ent->d_name);
    }
    closedir (dir);
  } else {
    /* could not open directory */
    perror ("");
  }
  return result;
}

void DB::submitEncounters(std::vector<std::tuple<std::string,std::string,std::string,std::string>> _encounters){
  for(unsigned int i = 0; i < _encounters.size(); i++){
    std::string path = "storage/" + std::get<3>(_encounters.at(i)) + ".db";
    std::ofstream outfile;
    mtx.lock();
    outfile.open(path.c_str(), std::ios_base::app);
    outfile << std::get<0>(_encounters.at(i));
    outfile << " ";
    outfile << std::get<1>(_encounters.at(i));
    outfile << " ";
    outfile << std::get<2>(_encounters.at(i));
    outfile << std::endl;
    outfile.close();
    mtx.unlock();
  }
  return;
}
