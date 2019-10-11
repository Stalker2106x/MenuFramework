#ifndef STYLESHEET_HH_
#define STYLESHEET_HH_

#include "MenuItem.hh"

/*
 * Styling properties are defined as string key=value format, stored in map.
 */
class StyleUnit
{
public:
    StyleUnit();

    const std::string &operator[](const std::string &key);

    template <typename T>
    const T &get(const std::string &key)
    {
        throw std::runtime_error("Not implemented");
    }
private:
    std::map<std::string, std::string> _style;
};

/*
 * Common styling key strings
 */
#define STYLE_TOP           "top"
#define STYLE_LEFT          "left"
#define STYLE_RIGHT         "right"
#define STYLE_DOWN          "down"
#define STYLE_CHAR          "char"
#define STYLE_COUNT         "count"
#define STYLE_MARGIN        "margin"
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
    void load(const DataFile &source);
    const StyleUnit &operator[](MenuItem::Type type);

protected:
    std::map<size_t, StyleUnit> _assocs;
};

#endif /* !STYLESHEET_HH_ */