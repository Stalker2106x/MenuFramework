#include "MenuFramework.hpp"
#include "term-native/NativeInput.hpp"
#include "term-native/NativeRenderer.hpp"

int main(int argc, char **argv)
{
	std::string menuDocument = "<Menu OnLoad='InitScript'>"
														 " <Text>Menu Example</Text>"
														 " <Sep/>"
														 " <Button Type='Intern' Target=''>ButtonTest</Button>"
  													 " <Select Id='SelectTest'>"
  													 "  <Option Value='1'>Option1</Option>"
  													 "  <Option Value='2'>Option2</Option>"
  													 " </Select>"
  													 " <Input Id='InputTest'/>"
														 "</Menu>";
  Menu::init();
  Menu::goTo("", menuDocument, DataSource::Document);
  Menu::run();
	while (1);
  return (0);
}