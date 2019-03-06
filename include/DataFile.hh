#ifndef DATAFILE_HH_
#define DATAFILE_HH_

#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <pugixml.hpp>
using xml_document = pugi::xml_document;
using xml_node = pugi::xml_node;

enum DataSource {
  Filesystem,
  Document
};

/*!
 * @brief JSON File I/O serialization class
 */
class DataFile
{
public:
  DataFile(const std::string &source, const DataSource sourceMode = Filesystem);


  static bool erase(const std::string &name);
  static bool rename(const std::string &oldName, const std::string &newName);
  static std::vector<std::string> getFolderContents(std::string path, std::string filter = "", bool truncateFilter = false);

  bool load();
  bool save();
  bool save(const json &data);

  bool rename(const std::string &name);
  void setData(const json &data);
  const json &getData();

private:
  std::string _source;
  DataSource _sourceMode;
  json _data;
};

/*!
 * @brief XML File I/O serialization class
 */
class MenuFile
{
public:
  MenuFile(const std::string &source, const DataSource sourceMode = Filesystem);

  bool load();
  bool save();
  bool save(const xml_document &data);

  const xml_document &getData();
  void setData(const xml_document &data);

  static xml_node stringToXML(const std::string &data);

private:
  std::string _source;
  DataSource _sourceMode;
  xml_document _data;
};

#endif /* !DATAFILE_HH_ */