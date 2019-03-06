#ifndef SCRIPTENGINE_HH_
#define SCRIPTENGINE_HH_

#include <map>
#include <sol.hpp>
#include "DataFile.hh"
#include "Terminal.hh"

class Menu;

struct ScriptEngine
{
  static void init();
  static void reset();

  static void runScript(const std::string &scriptId);
  static void run(const std::string &script);
  static void exposeCpp();
  static void exposeCollections();

  static void loadScripts(const xml_document &doc);

  static void console(Menu &currentMenu);

  static sol::state engine;
  static sol::environment scriptEnv;
  static std::map<std::string, std::string> scripts;
  static std::map<std::string, std::string> environment;
};



#endif /* !SCRIPTENGINE_HH_ */