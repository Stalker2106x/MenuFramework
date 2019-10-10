#include "StyleSheet.hh"

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

const std::string &StyleSheet::operator[](MenuItem::Type type)
{
    return (_assocs[type]);
}