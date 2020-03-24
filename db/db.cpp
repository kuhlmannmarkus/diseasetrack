#include "db.h"
#include <iterator>

//############################################################################################################
DB::DB(std::string _path, std::string _fileEnding) :
  m_path(_path),
  m_fileEnding(_fileEnding)
{}

//############################################################################################################
DB::~DB() {}

//############################################################################################################
std::vector<std::tuple<std::string, std::string, double, double, UINT64>>  DB::getEncounters(std::string _puplicKeyContactPerson)
{
  std::vector<std::tuple<std::string, std::string, double, double, UINT64>> result;

  std::string path = getFilenameOfContact(_puplicKeyContactPerson);

  mtx.lock();

  std::ifstream infile(path.c_str(), std::ios::in);

  // check for file
  if (infile.good())
  {
	std::string str;

	str.assign((std::istreambuf_iterator<char>(infile)),
	std::istreambuf_iterator<char>());

	infile.close();

	//Split lines
	std::vector<std::string> lines = splitString(str, "\n");

	// Fill result
	std::tuple<std::string, std::string, double, double, UINT64> temp;

	for (size_t i = 0; i < lines.size(); i++)
	{ 
		//Split line into string vector
		std::istringstream iss(lines[i]);
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
										 std::istream_iterator<std::string>{} };

		//Set values into tuple
		std::get<0>(temp) = tokens[0]; // contact
		std::get<1>(temp) = tokens[1]; // infected
		std::get<2>(temp) = ::atof(tokens[2].c_str());	// latitude
		std::get<3>(temp) = ::atof(tokens[3].c_str());		// longitude
		std::get<4>(temp) = std::stoull(tokens[4].c_str());	// timestamp

		//Add tuple to result vector
		result.insert(result.begin(), temp);
	}
  }
  else
  {
	/* file does not exist or is already open */
	perror("");
	infile.close();
  }

  mtx.unlock();
  return result;
}

//############################################################################################################
void DB::submitEncounters(std::vector<std::tuple<std::string, std::string, double, double, UINT64>> _encounters)
{
  for(size_t i = 0; i < _encounters.size(); i++){
    std::string path = getFilenameOfContact(std::get<0>(_encounters.at(i)));
    std::ofstream outfile;
    mtx.lock();
    outfile.open(path.c_str(), std::ios_base::app);
    // Write puplicKeyContactPerson
    outfile << std::get<0>(_encounters.at(i));
    outfile << " ";
    // Write puplicKeyInfectedPerson
    outfile << std::get<1>(_encounters.at(i));
    outfile << " ";
    // Write latitude
    outfile << std::get<2>(_encounters.at(i));
	outfile << " ";
    // Write longitude
	outfile << std::get<3>(_encounters.at(i));
	outfile << " ";
    // Write timestamp
	outfile << std::get<4>(_encounters.at(i));

    outfile << std::endl;
    outfile.close();
    mtx.unlock();
  }
  return;
}

//############################################################################################################
std::vector<std::string> DB::splitString(std::string _stringToBeSplitted, std::string _delimeter)
{
  std::vector<std::string> splittedString;
  size_t startIndex = 0;
  size_t endIndex = 0;
  while ((endIndex = _stringToBeSplitted.find(_delimeter, startIndex)) < _stringToBeSplitted.size())
  {
	std::string val = _stringToBeSplitted.substr(startIndex, endIndex - startIndex);
	splittedString.push_back(val);
	startIndex = endIndex + _delimeter.size();
  }
  if (startIndex < _stringToBeSplitted.size())
  {
	std::string val = _stringToBeSplitted.substr(startIndex);
	splittedString.push_back(val);
  }
  return splittedString;
}

//############################################################################################################
std::string DB::getFilenameOfContact(std::string _puplicKeyOfContactPerson)
{
  std::hash<std::string> hasher;

  std::size_t hash = hasher(_puplicKeyOfContactPerson);

  std::string result = m_path.c_str() + std::to_string(hash) + ".db";

  return result;
}
//############################################################################################################
