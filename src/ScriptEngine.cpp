#include "Localization.hh"
#include "ScriptEngine.hh"
#include "Menu.hh"

std::map<std::string, std::string> ScriptEngine::scripts = std::map<std::string, std::string>();
std::map<std::string, std::string> ScriptEngine::environment = std::map<std::string, std::string>();
sol::state ScriptEngine::engine;
sol::environment ScriptEngine::scriptEnv;

void ScriptEngine::init()
{
  engine.open_libraries(sol::lib::base);
}

void ScriptEngine::reset()
{
  engine.collect_garbage();
  scriptEnv = sol::environment(ScriptEngine::engine, sol::create);
  exposeCpp();
}

void ScriptEngine::runScript(const std::string &scriptId)
{
  if (scriptId == "") throw (std::runtime_error("No script given"));
  run(scripts[scriptId]);
}

void ScriptEngine::run(const std::string &script)
{
  try {
    engine.script(script, scriptEnv);
  } catch (std::exception &e) {
    Menu::alert(std::string(e.what()));
  }
}

void ScriptEngine::exposeCpp()
{
  //General
  scriptEnv.set_function("print", [=] (std::string x) { (*Menu::renderer) << x; });
  scriptEnv.set_function("tostring", [=] (int x) { return (std::to_string(x)); });
  scriptEnv.set_function("toint", [=] (std::string str) { return (atoi(str.c_str())); });
  scriptEnv.set_function("clearScreen", [] () { (*Menu::renderer).clearScreen(); });
  //scriptEnv.set_function("pause", [] () { getch(); });
  scriptEnv.set_function("exit", [] () { exit(0); });
  scriptEnv.set_function("alert", [=] (std::string msg) { Menu::alert(msg); });
  scriptEnv.set_function("goTo", [=] (std::string id) { Menu::goTo(id); });
  scriptEnv.set_function("goToPath", [=] (std::string id, std::string path) { Menu::goTo(id, path); });
  scriptEnv.set_function("setEnv", [=] (std::string key, std::string value) { environment[key] = value; });
  scriptEnv.set_function("getEnv", [=] (std::string key) { return (environment[key]); });
  //Menu helper
  scriptEnv.set_function("addMenuItem", [=] (int idx, std::string xml) {
    MenuFile menu(xml, DataSource::Document);

    if (!menu.load()) Menu::alert("Invalid XML injection");
    Menu::active->addItem(menu.getData().first_child(), idx);
  });
  scriptEnv.set_function("getCursor", [] () { return (Menu::active->getCursor()); });
  scriptEnv.set_function("getInputData", [=] (std::string id) {
    std::shared_ptr<MenuInput> in = std::dynamic_pointer_cast<MenuInput>(Menu::active->getItem(id));
    return (in->getData());
  });
  scriptEnv.set_function("setInputData", [=] (std::string id, std::string value) {
    std::shared_ptr<MenuInput> in = std::dynamic_pointer_cast<MenuInput>(Menu::active->getItem(id));
    in->setData(value);
  });
  scriptEnv.set_function("getSelectValue", [=] (std::string id) {
    std::shared_ptr<MenuSelect> sel = std::dynamic_pointer_cast<MenuSelect>(Menu::active->getItem(id));
    return (sel->getData());
  });
  scriptEnv.set_function("setSelectValue", [=] (std::string id, std::string value) {
    std::shared_ptr<MenuSelect> sel = std::dynamic_pointer_cast<MenuSelect>(Menu::active->getItem(id));
    sel->setData(value);
  });
}

void ScriptEngine::loadScripts(const xml_document &doc)
{
  scripts.clear();
	for (pugi::xml_node el = doc.first_child(); el; el = el.next_sibling())
	{
    if (strcmp(el.name(), "Script") == 0)
    {
      scripts.emplace(el.attribute("Id").value(), el.first_child().value());
    }
	}
}

void ScriptEngine::console(Menu &currentMenu)
{
  int input;
  std::string command;

  currentMenu.renderConsole(command); //Renders console
  while ((input = Menu::inputmgr->getInput()) != InputManager::Keys::KEY_F11)
  {
    Menu::renderer->clearScreen();
    if (input == InputManager::Keys::KEY_ENTER)
    {
      run(command);
      command.clear();
    }
    else if (command.length() > 0 && (input == InputManager::Keys::KEY_BACKSPACE)) command.erase(--command.end());
    else command += input;
    currentMenu.renderConsole(command);
  }
  Menu::renderer->clearScreen();
}