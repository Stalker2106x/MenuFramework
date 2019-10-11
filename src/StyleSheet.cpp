#include "StyleSheet.hh"

StyleUnit::StyleUnit()
{

}

const std::string &StyleUnit::operator[](const std::string &key)
{
    return (_style[key]);
}

template <>
const int &StyleUnit::get<int>(const std::string &key)
{
    return (std::stoi(_style[key]));
}

template <>
const float &StyleUnit::get<float>(const std::string &key)
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
    //implement defaults
}

void load(const DataFile &source)
{

}

const StyleUnit &StyleSheet::operator[](MenuItem::Type type)
{
    return (_assocs[type]);
}