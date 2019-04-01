#include "MenuFramework.hpp"
#include <string>

int main(int argc, char **argv)
{
	std::string menuDocument;

	menuDocument += "<Menu>"
                  " <Text>Menu Example</Text>"
                  " <Sep/>"
                  " <Button Type='Intern' Target='alert(\"Stop pressing me!\")'>Alright</Button>"
                  " <Button Type='Intern' Target=''>Nope</Button>"
                  "</Menu>";
  Menu::goTo("", menuDocument, DataSource::Document);
  Menu::run();
  return (0);
}