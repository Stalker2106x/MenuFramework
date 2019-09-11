#ifndef SCRIPTENGINE_HH_
#define SCRIPTENGINE_HH_

#include <map>
#include "export.h"
#include "DataFile.hh"
namespace sol {
  class state;
  template <bool b>
  class basic_reference;
  using reference = basic_reference<false>;
  template <typename base_t>
  struct basic_environment;
  using environment = basic_environment<reference>;
}
class Menu;
class GraphicsRenderer;
class InputManager;

struct MF_API ScriptEngine
{
  static void init();
  static void reset();

  static void runScript(const std::string &scriptId);
  static void run(const std::string &script);

  static void exposeFunction(const std::string &name, std::function<void(void)> fptr);
  static void exposeCpp();

  static void loadScripts(const xml_document &doc);

  static void console(Menu &currentMenu, std::shared_ptr<InputManager> inputmgr, std::shared_ptr<GraphicsRenderer> renderer);

  static sol::state engine;
  static sol::environment scriptEnv;
  static std::map<std::string, std::string> scripts;
  static std::map<std::string, std::string> environment;
};



#endif /* !SCRIPTENGINE_HH_ */