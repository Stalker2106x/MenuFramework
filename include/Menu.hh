#ifndef MENU_HH_
#define MENU_HH_

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include "export.h"
#include "GraphicsRenderer.hh"
#include "InputManager.hpp"
#include "DataFile.hh"
#include "MenuItem.hh"
#include "StyleSheet.hh"

/*!
 * @brief Base/Generic class for Menu
 */
class MF_API Menu
{
public:
  //Generic
  static void setRenderer(std::shared_ptr<GraphicsRenderer> renderer);
  static void setInputManager(std::shared_ptr<InputManager> inputmgr);
  static void unload();

  static void alert(std::string str);

  static void setActiveDocument(std::shared_ptr<MenuFile> doc);

  static void goTo(std::string id, std::string source = "", DataSource sourceMode = DataSource::Filesystem);
  static void popUp(std::string id, std::string source = "", DataSource sourceMode = DataSource::Filesystem);
  
  static void quit();
  static void run();

  static void selectCursor();

  static std::shared_ptr<Menu> active;
  static std::shared_ptr<MenuFile> activeDoc;

private:
  static bool _quit;
  static std::mutex _mtx;
  static std::unique_ptr<std::thread> _instance;
  static std::shared_ptr<GraphicsRenderer> _renderer;
  static std::shared_ptr<InputManager> _inputmgr;

public:
  //Instance
  Menu(const std::string &id);

  void onLoad();

  void resetCursor();
  void addAlert(std::shared_ptr<MenuItem> menuItem);
  void clearAlerts();
  std::shared_ptr<MenuItem> getHoveredItem();
  std::shared_ptr<MenuItem> getItem(const std::string &id);
  void addItem(const xml_node &el, int idx = -1);
  void setClickCallback(std::function<void(std::shared_ptr<MenuItem>)> callback);

  int getCursor();
  void updateCursor(bool add);
  bool update();
  void renderAlerts();
  void render();
  void renderConsole(std::string command);

private:
  std::string _id;
  Key _lastInput;
  StyleSheet _style;
  std::vector<std::shared_ptr<MenuItem>>::const_iterator _selection;
  std::shared_ptr<std::function<void(std::shared_ptr<MenuItem>)>> _clickCallback;
  std::string _onLoadScript;
  std::deque<std::shared_ptr<MenuItem>> _alerts;
  std::vector<std::shared_ptr<MenuItem>> _items;
};

#endif /* !MENU_HH_ */