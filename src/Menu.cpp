#include "Menu.hh"
#include "ScriptEngine.hh"
#include "Game.hh"
#include "Collection.hpp"
#include "Terminal.hh"
#include <fstream>

//Menu

std::shared_ptr<Menu> Menu::active = nullptr;
std::shared_ptr<MenuFile> Menu::activeDoc = nullptr;
std::shared_ptr<GraphicsRenderer> Menu::renderer = nullptr;

Menu::Menu(const std::string &id)
 : _id(id), _lastInput(0), _title(None), _clickCallback(nullptr)
{
	xml_node menu;

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
	if (menu.attribute("Title")) _title = Menu::convertASCIILogo(menu.attribute("Title").value());
	if (menu.attribute("OnLoad")) _onLoadScript = menu.attribute("OnLoad").value();
}

void Menu::setRenderer(GraphicsRenderer &renderer)
{
	Menu::renderer = std::shared_ptr<GraphicsRenderer>(&renderer);
}

void Menu::onLoad()
{
	if (!_onLoadScript.empty()) ScriptEngine::runScript(_onLoadScript);
}

void Menu::setActiveDocument(const std::string &source, const DataSource sourceMode)
{
	setActiveDocument(std::make_shared<MenuFile>(source, sourceMode));
}

void Menu::setActiveDocument(std::shared_ptr<MenuFile> doc)
{
	activeDoc = doc;
	if (!activeDoc->load()) throw(std::runtime_error("Error on loading Menu XML"));
	ScriptEngine::loadScripts(activeDoc->getData());
	MenuDialog::load(activeDoc->getData());
}

void Menu::setClickCallback(std::function<void(std::shared_ptr<MenuItem>)> callback)
{
	_clickCallback = std::make_shared<std::function<void(std::shared_ptr<MenuItem>)>>(callback);
}

bool Menu::update()
{
	_lastInput = getch();
	if(_lastInput == KEY_UP) updateCursor(false);
	else if(_lastInput == KEY_DOWN) updateCursor(true);
	else if(_lastInput == KEY_ENTER || _lastInput == '\n' || _lastInput == '\r') (*_selection)->select();
	else if (_lastInput == KEY_F(11)) ScriptEngine::console(*this);
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
	for (size_t i = 0; i <_alerts.size(); i++) _alerts[i]->render();
}

void Menu::render()
{
	renderer->clearScreen();
	renderAlerts();
	if (_title != -1)
	{
		printASCIILogo(_title);
		(*renderer) << "\n";
	}
	for (size_t i = 0; i < _items.size(); i++)
	{
		_items[i]->render();
		(*renderer) << "\n";
	}
}

bool Menu::run()
{
	bool quit = false;
	while (!quit)
	{
		active->render();
		while (!active->update())
		{
			if (active->_clickCallback != nullptr)
			{
				(*active->_clickCallback)(active->getHoveredItem());
			}
		}
	}
	return (true);
}

std::shared_ptr<MenuItem> Menu::getHoveredItem()
{
	return (*_selection);
}

void Menu::renderConsole(std::string command)
{
	renderer->clearScreen();
	for (size_t i = 0; i <_alerts.size(); i++) _alerts[i]->render();
  (*renderer) << "> " << command;
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
	else Terminal::windows.at("main").print(str+"\n");
}

Menu::ASCIILogo Menu::convertASCIILogo(std::string art)
{
	if (art == "Game") return (ASCIILogo::Game);
	else if (art == "Options") return (Options);
	else if (art == "Garage") return (Garage);
	else if (art == "Stats") return (Stats);
	else if (art == "Car") return (Car);
	else return (None);
}

void Menu::printASCIILogo(ASCIILogo art)
{
	switch (art)
	{
	case ASCIILogo::Game:
		(*renderer) << setColor(GraphicsRenderer::Color::Red)
				 				<< " ________                  ________       _____                    \n"
				 				<< " ___  __ \\_____ ______________  __ \\_________(_)__   ______________\n"
				 				<< " __  /_/ /  __ `/  ___/  _ \\_  / / /_  ___/_  /__ | / /  _ \\_  ___/\n"
				 				<< " _  _, _// /_/ // /__ /  __/  /_/ /_  /   _  / __ |/ //  __/  /    \n"
				 				<< " /_/ |_| \\__,_/ \\___/ \\___//_____/ /_/    /_/  _____/ \\___//_/     \n"
				 				<< resetAttrs;
		break;
	case Options:
		(*renderer) << setColor(GraphicsRenderer::Color::Red)
								<< "_______          _____ _____                        \n"
								<< "__  __ \\________ __  /____(_)______ _______ ________\n"
								<< "_  / / /___  __ \\_  __/__  / _  __ \\__  __ \\__  ___/\n"
								<< "/ /_/ / __  /_/ // /_  _  /  / /_/ /_  / / /_(__  ) \n"
								<< "\\____/  _  .___/ \\__/  /_/   \\____/ /_/ /_/ /____/  \n"
								<< "        /_/                                         \n"
								<< resetAttrs;
		break;
	case Garage:
		(*renderer) << setColor(GraphicsRenderer::Color::Red)
								<< "_________                                       \n"
								<< "__  ____/______ _______________ ________ ______ \n"
								<< "_  / __  _  __ `/__  ___/_  __ `/__  __ `/_  _ \\\n"
								<< "/ /_/ /  / /_/ / _  /    / /_/ / _  /_/ / /  __/\n"
								<< "\\____/   \\__,_/  /_/     \\__,_/  _\\__, /  \\___/ \n"
								<< "                                 /____/         \n"
								<< resetAttrs;
		break;
	case Stats:
		(*renderer) << setColor(GraphicsRenderer::Color::Red)
								<< "_____________         _____         \n"
								<< "__  ___/__  /_______ ___  /_________\n"
								<< "_____ \\ _  __/_  __ `/_  __/__  ___/\n"
								<< "____/ / / /_  / /_/ / / /_  _(__  ) \n"
								<< "/____/  \\__/  \\__,_/  \\__/  /____/  \n"
								<< resetAttrs;
		break;
	case Car:
		(*renderer) << setColor(GraphicsRenderer::Color::Red)
								<< "		                      ___..............._\n"
								<< "             __.. ' _'.\"\"\"\"\"\"\\\\\"\"\"\"\"\"\"\"- .`-._\n"
								<< " ______.-'         (_) |      \\\\           ` \\\\`-. _\n"
								<< "/_       --------------'-------\\\\---....______\\\\__`.`  -..___\n"
								<< "| T      _.----._           Xxx|x...           |          _.._`--. _\n"
								<< "| |    .' ..--.. `.         XXX|XXXXXXXXXxx==  |       .'.---..`.     -._\n"
								<< "\\_j   /  /  __  \\  \\        XXX|XXXXXXXXXXX==  |      / /  __  \\ \\        `-.\n"
								<< " _|  |  |  /  \\  |  |       XXX|\"\"'            |     / |  /  \\  | |          |\n"
								<< "|__\\_j  |  \\__/  |  L__________|_______________|_____j |  \\__/  | L__________J\n"
								<< "     `'\\ \\      / ./__________________________________\\ \\      / /___________\\\n"
								<< "        `.`----'.'                                     `.`----'.'\n"
								<< "          `\"\"\"\"'                                         `\"\"\"\"'\n"
								<< resetAttrs;
		break;
	case None:
	default:
		break;
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
	if (source != "") setActiveDocument(source, dataMode);
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
		if(active->_lastInput == KEY_ENTER || active->_lastInput == '\n' || active->_lastInput == '\r') break;
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
	if (_selection != _items.end()) (*_selection)->toggleHover(); //Unselect previous selection
	try {
		std::shared_ptr<MenuItem> obj = MenuItem::create(el);
		auto it = (idx == -1 ? _items.end() : _items.begin() + idx);
		if (obj == nullptr) return; //Discard if non-object entity is parsed
		_items.insert(it, obj);
	} catch(std::exception &e) {
		Menu::alert(e.what());
	}
	resetCursor();
	if (_selection != _items.end()) (*_selection)->toggleHover();
}

//MenuDialog

std::map<std::string, MenuDialog> MenuDialog::dialogs = std::map<std::string, MenuDialog>();

MenuDialog::MenuDialog(xml_node &data)
 : _data(data.first_child().value())
{

}

void MenuDialog::load(const xml_document &doc)
{
	for (pugi::xml_node el = doc.first_child(); el; el = el.next_sibling())
	{
		if (strcmp(el.name(), "Dialog") == 0)
		{
			dialogs.emplace(el.attribute("Id").value(), MenuDialog(el));
		}
	}
}

void MenuDialog::open()
{
	//_win = Terminal::get().addChildWindow(Point(), Point(50, 50));
}

void MenuDialog::render()
{

}