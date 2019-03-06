#include "MenuItem.hh"
#include "ScriptEngine.hh"
#include "Menu.hh"
#include "Localization.hh"

//MenuItem

MenuItem::MenuItem(const xml_node &data)
 : _id(data.attribute("Id").value()),
   _label(Localization::substitute(data)),
   _hover(false)
{
}

MenuItem::MenuItem(std::string label)
 : _id(),
 _label(Localization::substitute(label)),
 _hover(false)
{
}

std::shared_ptr<MenuItem> MenuItem::create(const xml_node &data)
{
  std::shared_ptr<MenuItem> el;

  try {
    if (strcmp(data.name(), "Text") == 0) el = std::make_shared<MenuItem>(data);
    else if (strcmp(data.name(), "Sep") == 0) el = std::make_shared<MenuItem>("======");
    else if (strcmp(data.name(), "Button") == 0)
    {
      el = std::make_shared<MenuButton>(data);
    }
    else if (strcmp(data.name(),"Input") == 0)
    {
      el = std::make_shared<MenuInput>(data);
    }
    else if (strcmp(data.name(),"Select") == 0)
    {
      el = std::make_shared<MenuSelect>(data);
    }
    else if (strcmp(data.name(),"Script") == 0)
    {
      el = std::make_shared<MenuScript>(data);
    }
    else if (strcmp(data.name(),"Alert") == 0)
    {
      el = std::make_shared<MenuAlert>(data);
    }
    else throw(std::runtime_error("Error in XML"));
  } catch (std::exception &e) {
    throw (std::runtime_error("Invalid element XML"));
  }
  return (el);
}

void MenuItem::toggleHover()
{
  _hover = !_hover;
}

bool MenuItem::isSelectable()
{
  return (false);
}


const std::string &MenuItem::getId()
{
  return (_id);
}

void MenuItem::select()
{
  throw(std::runtime_error("MenuItem should not be selected"));
}

void MenuItem::render()
{
  if (_hover)
  {
    (*Menu::renderer) << setColor(GraphicsRenderer::Color::Red)
                      << ">";
  }
  (*Menu::renderer) << _label;
  if (_hover) (*Menu::renderer) << resetAttrs;
}

//MenuButton

MenuButton::MenuButton(const xml_node &data)
 : MenuItem(data), _target(data.attribute("Target").value()), _path(data.attribute("Path").value())
{
  if (strcmp(data.attribute("Type").value(), "Goto") == 0) _type = Goto;
  else if (strcmp(data.attribute("Type").value(), "Script") == 0) _type = Script;
  else if (strcmp(data.attribute("Type").value(), "Intern") == 0) _type = Intern;
  else throw (std::runtime_error("Unknown button type"));
}

void MenuButton::select()
{
  switch (_type)
  {
  case Goto:
  {
    Menu::goTo(_target, _path);
    break;
  }
  case Script:
    ScriptEngine::runScript(_target);
    break;
  case Intern:
    ScriptEngine::run(_target);
    break;
  case Cpp:
    _cppCallback();
    break;
  }
}

bool MenuButton::isSelectable()
{
  return (true);
}

void MenuButton::bind(std::function<void(void)> &callback)
{
  _type = Cpp;
  _cppCallback = callback;
}

//MenuInput

MenuInput::MenuInput(const xml_node &data)
 : MenuItem(data)
{

}

void MenuInput::select()
{
  int input;

  Menu::active->render(); //Render once
  //Menu::renderer->setCursor(2); //Block cursor
  //Menu::renderer->setCursorPos(_dataPos); //set cursor at end of input
  while ((input = getch()) != KEY_ENTER && input != '\r' && input != '\n')
  {
    if (_data.length() > 0 && (input == KEY_BACKSPACE || input == '\b')) _data.erase(--_data.end());
    else _data += input;
    Menu::renderer->clearScreen();
    Menu::active->render(); //Request render to update input
    //Menu::renderer->setCursorPos(_dataPos); //set cursor at end of input
  }
  Menu::renderer->clearScreen(); //Clear screen for menu redraw
  //Menu::renderer->setCursor(0); //Disable cursor
}

bool MenuInput::isSelectable()
{
  return (true);
}

std::string MenuInput::getData()
{
  return (_data);
}

void MenuInput::setData(const std::string data)
{
  _data = data;
}

void MenuInput::render()
{
  MenuItem::render();
  (*Menu::renderer) << ": " + _data;
  //_dataPos = Menu::renderer->getCursorPos(); //WEIRD ? should be static
}

//MenuSelect

MenuSelect::MenuSelect(const xml_node &data)
 : MenuItem(data), _cursor(0)
{
  _label = data.attribute("Text").value();
	for (pugi::xml_node el = data.first_child(); el; el = el.next_sibling())
  {
		if (strcmp(el.name(), "Option") == 0 ) _values.push_back(std::make_pair<std::string, std::string>(Localization::substitute(el), el.attribute("Value").value()));
  }
}

void MenuSelect::select()
{
  int input;

  Menu::active->render(); //Render once
  while ((input = getch()) != KEY_ENTER && input != '\r' && input != '\n')
  {
    if (input == KEY_LEFT) --_cursor;
    else if (input == KEY_RIGHT) ++_cursor;
    if (_cursor < 0) _cursor = _values.size()-1;
    if (static_cast<size_t>(_cursor) >= _values.size()) _cursor = 0;
    Menu::renderer->clearScreen();
    Menu::active->render(); //Request render to update input
  }
  Menu::renderer->clearScreen(); //Clear screen for menu redraw
}

bool MenuSelect::isSelectable()
{
  return (true);
}

std::string MenuSelect::getData()
{
  return (_values[_cursor].second);
}

void MenuSelect::setData(const std::string data)
{
  auto value = std::find_if(_values.begin(), _values.end(),
    			[=] (std::pair<std::string, std::string> p) { return (p.second == data); });
  if (value == _values.end()) throw (std::runtime_error("Value: " + data + " does not exist in select"));
  _cursor = value - _values.begin();
}

void MenuSelect::render()
{
  MenuItem::render();
  (*Menu::renderer) << "    <" + _values[_cursor].first + ">";
}


//MenuScript

MenuScript::MenuScript(const xml_node &data)
 : MenuItem(data)
{
}

bool MenuScript::isSelectable()
{
  return (false);
}

void MenuScript::render()
{
  ScriptEngine::run(_label);
}

//MenuAlert

MenuAlert::MenuAlert(const xml_node &data)
 : MenuItem(data)
{
}

bool MenuAlert::isSelectable()
{
  return (false);
}

void MenuAlert::render()
{
  (*Menu::renderer) << "::::\n" << _label << "\n::::\n";
}