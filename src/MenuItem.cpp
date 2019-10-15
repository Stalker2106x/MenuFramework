#include "MenuItem.hh"
#include "ScriptEngine.hh"
#include "Menu.hh"
#include "Localization.hh"

//MenuItem

MenuItem::MenuItem(const xml_node &data, bool selectable)
 : _id(data.attribute("Id").value()), _selectable(selectable),
   _label(Localization::substitute(data)),
   _hover(false)
{
}

MenuItem::MenuItem(std::string label, bool selectable)
 : _id(), _selectable(selectable),
 _label(Localization::substitute(label)),
 _hover(false)
{
}

std::shared_ptr<MenuItem> MenuItem::create(const xml_node &data)
{
  std::shared_ptr<MenuItem> el;

  try {
    if (strcmp(data.name(), "Text") == 0) el = std::make_shared<MenuItem>(data);
    else if (strcmp(data.name(), "Sep") == 0) el = std::make_shared<MenuItem>(StyleSheet::active->get<std::string>("separator", "string"));
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
  return (_selectable);
}


const std::string &MenuItem::getId()
{
  return (_id);
}

void MenuItem::select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer)
{
  throw(std::runtime_error("MenuItem should not be selected"));
}

void MenuItem::render(std::shared_ptr<GraphicsRenderer> renderer)
{
  if (_hover)
  {
    (*renderer) << setColor(GraphicsRenderer::Color::Red)
                << StyleSheet::active->get<std::string>("cursor", "char");
  }
  (*renderer) << _label;
  if (_hover) (*renderer) << resetAttrs;
}

//MenuButton

MenuButton::MenuButton(const xml_node &data)
 : MenuItem(data, true), _target(data.attribute("Target").value()), _path(data.attribute("Path").value())
{
  //Data
  if (strcmp(data.attribute("Type").value(), "Goto") == 0) _actionType = Goto;
  else if (strcmp(data.attribute("Type").value(), "Script") == 0) _actionType = Script;
  else if (strcmp(data.attribute("Type").value(), "Intern") == 0) _actionType = Intern;
  else throw (std::runtime_error("Unknown button type"));
  //Style
  StyleUnit styling = StyleSheet::active->get("button");
}

void MenuButton::select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer)
{
  switch (_actionType)
  {
  case Goto:
    Menu::goTo(_target, _path);
    break;
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

void MenuButton::bind(std::function<void(void)> &callback)
{
  _actionType = Cpp;
  _cppCallback = callback;
}

//MenuInput

MenuInput::MenuInput(const xml_node &data)
 : MenuItem(data, true)
{
  //Style
  StyleUnit styling = StyleSheet::active->get("input");
  _spacing = styling.get<int>("spacing");
}

void MenuInput::select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer)
{
  Key input;

  Menu::active->render(); //Render once
  while ((input = inputmgr->getInput()).code != Key::Code::Enter)
  {
    if (_data.length() > 0 && (input.code == Key::Code::Backspace)) _data.erase(--_data.end());
    else _data += input.value;
    renderer->clearScreen();
    Menu::active->render(); //Request render to update input
  }
  renderer->clearScreen(); //Clear screen for menu redraw
}

std::string MenuInput::getData()
{
  return (_data);
}

void MenuInput::setData(const std::string data)
{
  _data = data;
}

void MenuInput::render(std::shared_ptr<GraphicsRenderer> renderer)
{
  MenuItem::render(renderer);
  (*renderer) << std::string(_spacing, ' ') + _data;
  //_dataPos = Menu::renderer->getCursorPos(); //WEIRD ? should be static
}

//MenuSelect

MenuSelect::MenuSelect(const xml_node &data)
 : MenuItem(data, true), _cursor(0)
{
  //Data
  _label = data.attribute("Text").value();
	for (pugi::xml_node el = data.first_child(); el; el = el.next_sibling())
  {
		if (strcmp(el.name(), "Option") == 0 ) _values.push_back(std::make_pair<std::string, std::string>(Localization::substitute(el), el.attribute("Value").value()));
  }
  //Style
  StyleUnit styling = StyleSheet::active->get("select");
  _spacing = styling.get<int>("spacing");
  _bracing = std::make_pair(styling.get<std::string>("bracing-left"), styling.get<std::string>("bracing-right"));
}

void MenuSelect::select(std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer)
{
  Key::Code input;

  Menu::active->render(); //Render once
  while ((input = inputmgr->getInput().code) != Key::Code::Enter)
  {
    if (input == Key::Code::Left) --_cursor;
    else if (input == Key::Code::Right) ++_cursor;
    if (_cursor < 0) _cursor = _values.size()-1;
    if (static_cast<size_t>(_cursor) >= _values.size()) _cursor = 0;
    renderer->clearScreen();
    Menu::active->render(); //Request render to update input
  }
  renderer->clearScreen(); //Clear screen for menu redraw
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

void MenuSelect::render(std::shared_ptr<GraphicsRenderer> renderer)
{
  MenuItem::render(renderer);
  (*renderer) << std::string(_spacing, ' ') << _bracing.first << _values[_cursor].first << _bracing.second;
}


//MenuScript

MenuScript::MenuScript(const xml_node &data)
 : MenuItem(data, false)
{
}

void MenuScript::render(std::shared_ptr<GraphicsRenderer> renderer)
{
  ScriptEngine::run(_label);
}

//MenuAlert

MenuAlert::MenuAlert(const xml_node &data)
 : MenuItem(data, false)
{
  //Style
  StyleUnit styling = StyleSheet::active->get("alert");
  _bracing = std::make_pair(styling.get<std::string>("bracing-left"), styling.get<std::string>("bracing-right"));
}

void MenuAlert::render(std::shared_ptr<GraphicsRenderer> renderer)
{
  (*renderer) << _bracing.first << _label << _bracing.second;
}