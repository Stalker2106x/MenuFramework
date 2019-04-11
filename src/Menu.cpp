#include <fstream>
#include "Menu.hh"
#include "ScriptEngine.hh"
#include "Point.hpp"
//Native _renderer
#include "term-native/NativeInput.hpp"
#include "term-native/NativeRenderer.hpp"

//Menu

std::mutex Menu::_mtx;
std::shared_ptr<Menu> Menu::active = nullptr;
std::shared_ptr<MenuFile> Menu::activeDoc = nullptr;
std::unique_ptr<std::thread> Menu::_instance = nullptr;
std::shared_ptr<GraphicsRenderer> Menu::_renderer = std::make_shared<NativeRenderer>();
std::shared_ptr<InputManager> Menu::_inputmgr = std::make_shared<NativeInput>();
bool Menu::_quit = false;

Menu::Menu(const std::string &id)
 : _id(id), _lastInput(Key::Code::None), _clickCallback(nullptr)
{
	xml_node menu;

	_selection = _items.begin(); //Initialize selection
	ScriptEngine::reset(); //Clears lua state
	for (pugi::xml_node el = activeDoc->getData().first_child(); el; el = el.next_sibling())
	{
		if (strcmp(el.name(), "Menu") == 0 && el.attribute("Id").value() == id) menu = el;
	}
	if (!menu) throw(std::runtime_error("Menu not found"));
	for (pugi::xml_node el = menu.first_child(); el; el = el.next_sibling())
	{
		addItem(el);
	}
	resetCursor(); //Reset selection
	if (menu.attribute("OnLoad")) _onLoadScript = menu.attribute("OnLoad").value();
}

void Menu::setRenderer(std::shared_ptr<GraphicsRenderer> renderer)
{
	Menu::_renderer = renderer;
}

void Menu::setInputManager(std::shared_ptr<InputManager> inputmgr)
{
	Menu::_inputmgr = inputmgr;
}

void Menu::selectCursor()
{
	(*active->_selection)->select(_inputmgr, _renderer);
}

void Menu::quit()
{
	Menu::_quit = true;
	Menu::_instance->join();
	Menu::_instance = nullptr;
}

void Menu::unload()
{
	std::lock_guard<std::mutex> lock(_mtx);

	Menu::active = nullptr;
	Menu::activeDoc = nullptr;
	Menu::_renderer = nullptr;
	Menu::_inputmgr = nullptr;
}

void Menu::onLoad()
{
	if (!_onLoadScript.empty()) ScriptEngine::runScript(_onLoadScript);
}

void Menu::setActiveDocument(std::shared_ptr<MenuFile> doc)
{
	activeDoc = doc;
	if (!activeDoc->load()) throw(std::runtime_error("Error on loading Menu XML"));
	ScriptEngine::loadScripts(activeDoc->getData());
}

void Menu::setClickCallback(std::function<void(std::shared_ptr<MenuItem>)> callback)
{
	_clickCallback = std::make_shared<std::function<void(std::shared_ptr<MenuItem>)>>(callback);
}

bool Menu::update()
{
	Key input = _inputmgr->getInput();

	if(input.code == Key::Code::Up) updateCursor(false);
	else if(input.code == Key::Code::Down) updateCursor(true);
	else if(input.code == Key::Code::Enter) (*_selection)->select(_inputmgr, _renderer);
	else if (input.code == Key::Code::F11) ScriptEngine::console(*this, _inputmgr, _renderer);
	else return (false);
	return (true);
}

int Menu::getCursor()
{
	size_t cursor = 0;
	for (auto it = _items.begin(); it != _items.end() && it != _selection; it++)
	{
		if ((*it)->isSelectable()) cursor++;
	}
	if (cursor >= _items.size()) return (-1);
	return (cursor);
}

void Menu::resetCursor()
{
	_selection = _items.begin();
	while (_selection != _items.end() && !((*_selection)->isSelectable())) _selection++;
	if (_selection != _items.end()) (*_selection)->toggleHover();
}

void Menu::updateCursor(bool add)
{
	auto prevSel = _selection;

	(*_selection)->toggleHover();
	if ((!add && _selection == _items.begin()) || (add && _selection == --_items.end())) //Check for invalidation
	{
		_selection = (add ? _items.begin() : --_items.end());
	}
	else _selection += (add ? 1 : -1);
	while (_selection != prevSel && !((*_selection)->isSelectable())) //Continue browsing if not selectable
	{
		if ((!add && _selection == _items.begin()) || (add && _selection == --_items.end())) //Check for invalidation
		{
			_selection = (add ? _items.begin() : --_items.end());
		}
		else _selection += (add ? 1 : -1);
	}
	(*_selection)->toggleHover();
}

void Menu::renderAlerts()
{
	for (size_t i = 0; i <_alerts.size(); i++) _alerts[i]->render(_renderer);
}

void Menu::render()
{
	_renderer->clearScreen();
	renderAlerts();
	for (size_t i = 0; i < _items.size(); i++)
	{
		_items[i]->render(_renderer);
		_renderer->lineBreak();
	}
}

void Menu::run()
{
	_instance = std::make_unique<std::thread>([=] () {
		while (!_quit)
		{
			std::shared_ptr<Menu> activeMenu;
			{
				std::lock_guard<std::mutex> lock(_mtx);
			 	activeMenu = active;
			}
			activeMenu->render();
			if (activeMenu->update())
			{
				if (activeMenu->_clickCallback != nullptr)
				{
					(*activeMenu->_clickCallback)(active->getHoveredItem());
				}
			}
		}
		unload();
	});
}

std::shared_ptr<MenuItem> Menu::getHoveredItem()
{
	return (*_selection);
}

void Menu::renderConsole(std::string command)
{
	_renderer->clearScreen();
	for (size_t i = 0; i <_alerts.size(); i++) _alerts[i]->render(_renderer);
  (*_renderer) << "> " << command;
}

void Menu::alert(std::string str)
{
	if (Menu::active != nullptr)
	{
		std::string itemXML = "<Alert>" + str + "</Alert>";
		MenuFile menu(itemXML, DataSource::Document);
		if (!menu.load()) throw(std::runtime_error("Error on loading Alert XML"));
		Menu::active->addAlert(MenuItem::create(menu.getData().first_child()));
	}
	else
	{
		_renderer->print(str);
		_renderer->lineBreak();
	}
}

void Menu::addAlert(std::shared_ptr<MenuItem> menuItem)
{
	_alerts.push_back(menuItem);
	if (_alerts.size() > 4) _alerts.pop_front();
	renderAlerts();
}

void Menu::clearAlerts()
{
	_alerts.clear();
}

void Menu::goTo(std::string id, std::string source, const DataSource dataMode)
{
	if (source != "") setActiveDocument(std::make_shared<MenuFile>(source, dataMode));
	active = std::make_shared<Menu>(Menu(id));
	active->onLoad();
}

void Menu::popUp(std::string id, std::string source, const DataSource dataMode)
{
	std::shared_ptr<MenuFile> prevDoc = activeDoc;
	std::string prevId = active->_id;
	goTo(id, source, dataMode);
	while (1)
	{
		active->render();
		active->update();
		if(active->_lastInput.code == Key::Code::Enter) break;
	};
	if (active->_clickCallback != nullptr)
	{
		(*active->_clickCallback)(active->getHoveredItem());
	}
	setActiveDocument(prevDoc);
	goTo(prevId);
}

std::shared_ptr<MenuItem> Menu::getItem(const std::string &id)
{
	auto obj = std::find_if(_items.begin(), _items.end(),
    			[=] (std::shared_ptr<MenuItem> m) { return (m->getId() == id); });
	if (obj == _items.end()) throw (std::runtime_error("Specified item does not exist"));
	return *(obj);
}

void Menu::addItem(const xml_node &el, int idx)
{
	try {
		std::shared_ptr<MenuItem> obj = MenuItem::create(el);
		auto it = (idx == -1 ? _items.end() : _items.begin() + idx);
		if (obj == nullptr) return; //Discard if non-object entity is parsed
		_items.insert(it, obj);
	} catch(std::exception &e) {
		Menu::alert(e.what());
	}
}