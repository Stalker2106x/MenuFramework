#ifndef STYLESHEET_HH_
#define STYLESHEET_HH_

#include "MenuItem.hh"

/*
 * Styling properties are defined as string key=value format, stored in map.
 */
class StyleUnit
{
    typedef std::map<std::string, std::string> styleMap_t;
public:
    StyleUnit(const json &data);
    StyleUnit(std::initializer_list<std::pair<const std::string, std::string>> data);

    const std::string &operator[](const std::string &key);

    template <typename T>
    const T get(const std::string &key)
    {
        throw std::runtime_error("Not implemented");
    }
private:
    styleMap_t _style;
};

/*
 * Common styling key strings
 */
#define STYLE_TOP           "top"
#define STYLE_LEFT          "left"
#define STYLE_RIGHT         "right"
#define STYLE_DOWN          "down"
#define STYLE_COLOR         "color"
#define STYLE_BGCOLOR       "bg" STYLE_COLOR
#define STYLE_MARGIN        "margin"
#define STYLE_SPACING       "spacing"
#define STYLE_MARGIN_TOP    STYLE_MARGIN "-" STYLE_TOP
#define STYLE_MARGIN_RIGHT  STYLE_MARGIN "-" STYLE_RIGHT
#define STYLE_MARGIN_DOWN   STYLE_MARGIN "-" STYLE_DOWN
#define STYLE_MARGIN_LEFT   STYLE_MARGIN "-" STYLE_LEFT

/*
 * This class holds style for every non alphanumeric char that is rendered
 */
class StyleSheet {
public:
    StyleSheet();

    void setDefaults(bool clear = true);
    void load(const std::string &path);
    const StyleUnit &get(const std::string &group);
    
    template <typename T>
    const T get(const std::string &group, const std::string &key)
    {
        return (_assocs.at(group).get<T>(key));
    }

    static std::shared_ptr<StyleSheet> active;
protected:
    typedef std::map<std::string, StyleUnit> assocMap_t;
    assocMap_t _assocs;
};

#endif /* !STYLESHEET_HH_ */