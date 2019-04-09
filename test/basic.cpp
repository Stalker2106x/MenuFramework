#include "MenuFramework.hpp"
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char **argv)
{
	std::string menuDocument;

	menuDocument += "<Menu>"
                  " <Text>Menu Example</Text>"
                  " <Sep/>"
                  " <Button Type='Intern' Target='exit()'>ExitTest</Button>"
                  "</Menu>";
  Menu::goTo("", menuDocument, DataSource::Document);
  Menu::run();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  Menu::active->selectCursor();
  Menu::unload();
  return (0);
}