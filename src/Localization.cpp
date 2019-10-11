#include "Localization.hh"

std::map<std::string, std::string> Localization::data;
std::string Localization::langLocation = "./";

/*
 * @brief Removes all loaded translation from memory
 */
void Localization::clear()
{
  data.clear();
}

/*
 * @brief Search inside any string and subsitutes any <Lang> tag to its translation
 */
std::string Localization::substitute(std::string data)
{
  size_t beg = 0;
  std::string str;

  while ((beg = data.find("<Lang")) != std::string::npos)
  {
    size_t end = 0;
    str += data.substr(0, beg);
    if ((end = data.find("/>")) != std::string::npos)
    {
      xml_node unit = MenuFile::stringToXML(data.substr(beg, end));
      std::string id = unit.attribute("Id").value();
      str += Localization::data.at(id);
    }
    data.erase(0, (end == 0 ? beg : end));
  }
  return (str);
}

/*
 * @brief Search inside any XML element and subsitutes any <Lang> tag to its translation
 */
std::string Localization::substitute(xml_node data)
{
  std::string str;

  if (strcmp(data.name(), "Lang") == 0)
  {
    try {
      str += Localization::data.at(data.attribute("Id").value());
    } catch(...) {
      str += data.attribute("Id").value();
    }
  }
	for (pugi::xml_node el = data.first_child(); el; el = el.next_sibling())
  {
    if (strcmp(el.name(), "Lang") == 0)
    {
      try {
        str += Localization::data.at(el.attribute("Id").value());
      } catch(...) {
        str += el.attribute("Id").value();
      }
    }
    else if (el.value()) str += el.value();
  }
  return (str);
}

/*
 * @brief loads lang key=value pairs from json file
 */
void Localization::load(std::string langCode)
{
  DataFile langFile(langLocation+langCode+".json");
  json lang;

  if (!langFile.load()) throw (std::runtime_error("Error on lang loading"));
  clear();
  lang = langFile.getData();
  for (auto &unit : lang["data"].get<json::object_t>())
  {
    data.emplace(unit.first, unit.second.get<std::string>());
  }
}