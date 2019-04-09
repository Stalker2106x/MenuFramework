#include "MenuFramework.hpp"
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char **argv)
{
	std::string menuDocument;

	menuDocument += "<Menu OnLoad='InitScript'>"
		" <Text>Menu Example</Text>"
		" <Sep/>"
		" <Button Type='Intern' Target='exit()'>ExitTest</Button>"
		" <Button Type='Intern' Target='exit()'>SS</Button>"
		"</Menu>"
		"<Script Id='InitScript'><![CDATA["
		"exit()"
		"]]></Script>";
  Menu::goTo("", menuDocument, DataSource::Document);
  Menu::run();
  Menu::unload();
  return (0);
}