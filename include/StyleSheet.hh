#ifndef STYLESHEET_HH_
#define STYLESHEET_HH_

#include "MenuItem.hh"

class StyleSheet {
public:
    StyleSheet();

    void setDefaults(bool clear = true);
    void load(const DataFile &source);
    const std::string &operator[](MenuItem::Type type);
    
protected:
    std::map<size_t, std::string> _assocs;
};

#endif /* !STYLESHEET_HH_ */