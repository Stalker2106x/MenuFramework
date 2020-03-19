#include "StyleSheet.hh"

std::shared_ptr<StyleSheet> StyleSheet::active = nullptr;

// StyleUnit

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

// StyleSheet

StyleSheet::StyleSheet()
{
    setDefaults();
}

void StyleSheet::setDefaults(bool clear)
{
    if (clear) _assocs.clear();
    _assocs.insert({
    {"cursor",
        {
            {"char",">"}
        }
    },
    {"separator",
        {
            {"string","======="}
        }
    },
    {"text",
        {
        }
    },
    {"input",
        {
            {"spacing","2"}
        }
    },
    {"select",
        {
            {"spacing","2"},
            {"bracing-left","<"},
            {"bracing-right",">"}
        }
    },
    {"alert",
        {
            {"bracing-left",":::::\n"},
            {"bracing-right","\n:::::\n"}
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
    _assocs.emplace(item.first, StyleUnit(item.second));
  }
}

const StyleUnit &StyleSheet::get(const std::string &group)
{
    return (_assocs.at(group));
}