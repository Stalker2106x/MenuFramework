#include "DataFile.hh"
#ifdef __GNUC__
#include <dirent.h>
#endif
#ifdef _MSC_VER
#include "dirent/dirent.h"
#endif

//JSON

DataFile::DataFile(const std::string &source, const DataSource sourceMode)
 : _source(source), _sourceMode(sourceMode)
{

}

bool DataFile::load()
{
  switch(_sourceMode)
  {
  case Filesystem:
  {
    std::ifstream file;

    file.open(_source, std::ifstream::in);
    if (!file.good()) return (false);
    _data = json::parse(file);
    file.close();
    break;
  }
  case Document:
    _data = json::parse(_source);
    break;
  }
  return (true);
}

bool DataFile::save()
{
  return (save(_data));
}

bool DataFile::save(const json &data)
{
  std::ofstream file;

  file.open(_source, std::ofstream::out | std::ofstream::trunc);
  if (!file.good()) return (false);
  file << data.dump();
  file.close();
  return (true);
}

void DataFile::setData(const json &data)
{
  _source.clear();
  _data = data;
}

bool DataFile::erase(const std::string &name)
{
  return (remove(name.c_str()) == 0);
}

bool DataFile::rename(const std::string &name)
{
  if (_sourceMode != Filesystem) return (false);
  return (!std::rename(_source.c_str(), name.c_str()));
}

const json &DataFile::getData()
{
  return (_data);
}

bool DataFile::rename(const std::string &oldName, const std::string &newName)
{
  std::string path = "./Data/Saves/";
  return (std::rename((path+oldName+".json").c_str(), (path+newName+".json").c_str()) == 0);
}

std::vector<std::string> DataFile::getFolderContents(std::string path, std::string filter, bool truncateFilter)
{
	std::vector<std::string> files;
	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir(path.c_str())) == NULL)
	{
			throw (std::runtime_error("Error opening " + path));
	}
	while ((dirp = readdir(dp)) != NULL)
	{
		std::string file = std::string(dirp->d_name);
    size_t pos = file.find(filter);
    if (filter == "" || (pos != std::string::npos && (pos + filter.length()) == file.length()))
    {
      if (truncateFilter) files.push_back(file.substr(0, file.find(filter)));
      else files.push_back(file);
    }
	}
	closedir(dp);
	return (files);
}


//XML

MenuFile::MenuFile(const std::string &source, const DataSource sourceMode)
 : _source(source), _sourceMode(sourceMode)
{

}

bool MenuFile::load()
{
  switch(_sourceMode)
  {
  case Filesystem:
    _data.load_file(_source.c_str());
  break;
  case Document:
    _data.load_string(_source.c_str());
  break;
  }
  return (true);
}

bool MenuFile::save()
{
  return (save(_data));
}

bool MenuFile::save(const xml_document &data)
{
  _data.reset(data);
  _data.save_file(_source.c_str());
  return (true);
}

const xml_document &MenuFile::getData()
{
  return (_data);
}

void MenuFile::setData(const xml_document &data)
{
  _source.clear();
  _data.reset(data);
}

xml_node MenuFile::stringToXML(const std::string &data)
{
  xml_document doc;
  doc.load_string(data.c_str());
  return (doc.root());
}