#include "StyleSheet.hh"

StyleUnit::StyleUnit(const json &data)
{
  for (auto &unit : data.get<json::object_t>())
  {
    _style.emplace(unit.first, unit.second.get<std::string>());
  }
}

StyleUnit::StyleUnit(std::initializer_list<std::pair<const std::string, std::string>> data)
{
    _style.insert(data);
}

const std::string &StyleUnit::operator[](const std::string &key)
{
    return (_style[key]);
}

template <>
const int StyleUnit::get<int>(const std::string &key)
{
    return (std::stoi(_style[key]));
}

template <>
const float StyleUnit::get<float>(const std::string &key)
{
    return (std::stof(_style[key]));
}

StyleSheet::StyleSheet()
{
    setDefaults();
}

void StyleSheet::setDefaults(bool clear)
{
    if (clear) _assocs.clear();
    _assocs.insert({
    {MenuItem::Type::Text,
        {
            {"",""}
        }
    },
    {MenuItem::Type::Separator,
        {
            {"skin","-=-"}
        }
    },
    {MenuItem::Type::Button,
        {
        }
    },
    {MenuItem::Type::Alert,
        {
            {"skin","::"}
        }
    },
    {MenuItem::Type::Select,
        {
            {"left-select","<"},
            {"right-select",">"}
        }
    }
    });
}

void StyleSheet::load(const std::string &path)
{
  DataFile styleFile(path);
  json style;

  if (!styleFile.load()) throw (std::runtime_error("Error on stylesheet import"));
  style = styleFile.getData();
  for (auto &item : style.get<json::object_t>())
  {
    //_assocs.emplace(/* item typename to enum */, StyleUnit(item.second));
  }

}

const StyleUnit &StyleSheet::operator[](MenuItem::Type type)
{
    return (_assocs.at(type));
}