#ifndef LOCALIZATION_HH_
#define LOCALIZATION_HH_

#include <map>
#include "export.h"
#include "DataFile.hh"

struct MF_API Localization
{
  static void clear();
  static std::string substitute(std::string data);
  static std::string substitute(xml_node data);
  static void load(std::string langCode);

  static std::map<std::string, std::string> data;
  static std::string langLocation;
};

#endif /* !LOCALIZATION_HH_ */