#ifndef MENU_HH_
#define MENU_HH_

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <memory>
#include <functional>
#include "DataFile.hh"
#include "MenuItem.hh"

/*!
 * @brief Base/Generic class for Menu
 */
class Menu
{
public:
  //Generic
  enum ASCIILogo {
    None = -1,
    Game,
    Options,
    Garage,
    Stats,
    Car
  };
  static void setRenderer(GraphicsRenderer &renderer);

  static ASCIILogo convertASCIILogo(std::string art);
  static void printASCIILogo(ASCIILogo art);
  static void alert(std::string str);

  static void setActiveDocument(const std::string &source, const DataSource sourceMode);
  static void setActiveDocument(std::shared_ptr<MenuFile> doc);

  static void goTo(std::string id, std::string source = "", DataSource sourceMode = DataSource::Filesystem);
  static void popUp(std::string id, std::string source = "", DataSource sourceMode = DataSource::Filesystem);
  static bool run();

  static std::shared_ptr<Menu> active;
  static std::shared_ptr<MenuFile> activeDoc;

  //Instance
  Menu(const std::string &id);

  void onLoad();

	void resetCursor();
  void addAlert(std::shared_ptr<MenuItem> menuItem);
  void clearAlerts();
  std::shared_ptr<MenuItem> getItem(const std::string &id);
  void addItem(const xml_node &el, int idx = -1);
  void setClickCallback(std::function<void(std::shared_ptr<MenuItem>)> callback);

  int getCursor();
  void updateCursor(bool add);
  bool update();
  void renderAlerts();
  void render();
  void renderConsole(std::string command);

  static std::shared_ptr<GraphicsRenderer> renderer;
private:
  std::shared_ptr<MenuItem> getHoveredItem();


  std::string _id;
  int _lastInput;
  ASCIILogo _title; //title art
  std::vector<std::shared_ptr<MenuItem>>::iterator _selection;
  std::shared_ptr<std::function<void(std::shared_ptr<MenuItem>)>> _clickCallback;
  std::string _onLoadScript;
  std::deque<std::shared_ptr<MenuItem>> _alerts;
  std::vector<std::shared_ptr<MenuItem>> _items;
};

/*!
 * @brief Acts as a sub window
 */
class MenuDialog
{
public:
  MenuDialog(xml_node &data);

  static void load(const xml_document &doc);

  static std::map<std::string, MenuDialog> dialogs;

  void open();
  void render();

private:
  std::string _data;
  WINDOW *_win;
};

#endif /* !MENU_HH_ */