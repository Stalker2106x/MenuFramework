#ifndef MENUITEM_HH_
#define MENUITEM_HH_

#include <memory>
#include <string>
#include "export.h"
#include "Point.hpp"
#include "GraphicsRenderer.hh"
#include "InputManager.hpp"
#include "DataFile.hh"

#define TARGET_SEPARATOR  ":"

/*!
 * @brief Holds menu entry data
 */
class MF_API MenuItem
{
public:

  /*!
  * @brief Holds all existing item types
  */
  enum Type {
    Unknown = 0,
    Button,
    Select,
    Script,
    Alert
  };

  MenuItem(const xml_node &data, bool selectable = false);
  MenuItem(std::string label, bool selectable = false);

  static std::shared_ptr<MenuItem> create(const xml_node &data);

  void toggleHover();

  const std::string &getId();
  bool isSelectable();
  virtual void select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer);
  virtual void render(std::shared_ptr<GraphicsRenderer> renderer);

protected:
  bool _selectable;
  std::string _id;
  std::string _label;
  bool _hover;
};

/*!
 * @brief Acts as a button
 */
class MenuButton : public MenuItem
{
public:
  enum ActionType {
    Goto,
    Script,
    Intern,
    Cpp
  };
  MenuButton(const xml_node &data);

  virtual void select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer);
  void bind(std::function<void(void)> &callback);

private:
  ActionType _actionType;
  std::string _target, _path;
  std::function<void(void)> _cppCallback;
};

/*!
 * @brief Acts as a text input
 */
class MenuInput : public MenuItem
{
public:
  MenuInput(const xml_node &data);

  virtual void select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer);
  std::string getData();
  void setData(const std::string data);
  virtual void render(std::shared_ptr<GraphicsRenderer> renderer);

private:
  std::string _data;
  Point _dataPos;
};

/*!
 * @brief Acts as a multi-value selector
 */
class MenuSelect : public MenuItem
{
public:
  MenuSelect(const xml_node &data);

  virtual void select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer);
  std::string getData();
  void setData(const std::string data);
  virtual void render(std::shared_ptr<GraphicsRenderer> renderer);

private:
  std::vector<std::pair<std::string, std::string>> _values;
  int _cursor;
};

/*!
 * @brief Acts as a nested script
 */
class MenuScript : public MenuItem
{
public:
  MenuScript(const xml_node &data);

  virtual void render(std::shared_ptr<GraphicsRenderer> renderer);
};

/*!
 * @brief Acts as an alert message
 */
class MenuAlert : public MenuItem
{
public:
  MenuAlert(const xml_node &data);

  virtual void render(std::shared_ptr<GraphicsRenderer> renderer);
};


#endif /* !MENUITEM_HH_ */