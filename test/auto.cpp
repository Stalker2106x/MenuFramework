#include "MenuFramework.hpp"
#include "term-native/NativeInput.hpp"
#include "term-native/NativeRenderer.hpp"

int main(int argc, char **argv)
{
	Localization::langLocation = "../test/";
	Localization::load("en-US");
	Menu::setRenderer(std::make_shared<NativeRenderer>());
	Menu::setInputManager(std::make_shared<NativeInput>());
	std::string menuDocument = "<Menu OnLoad='InitScript'>"
														 " <Text>Menu Example</Text>"
														 " <Text>Look: <Lang Id='ThisLangDoesNotExist'/></Text>"
														 " <Text>Look: <Lang Id='ThisOneDoes'/></Text>"
														 " <Sep/>"
														 " <Button Type='Intern' Target=''>ButtonTest</Button>"
														 "<Script></Script>"
														 " <Select Id='SelectTest'>"
														 "  <Option Value='1'>Option1</Option>"
														 "  <Option Value='2'>Option2</Option>"
														 " </Select>"
														 " <Input Id='InputTest'/>"
														 "</Menu>"
														 "<Script Id='InitScript'><![CDATA["
														 "	 alert(\"Hello, i am an alert\")"
														 "	 tostring(5)"
														 "	 getCursor()"
														 "	 getInputData(\"InputTest\")"
														 "	 setInputData(\"InputTest\", \"This is a test\")"
														 "	 getSelectValue(\"SelectTest\")"
														 "	 setSelectValue(\"SelectTest\", \"2\")"
														 "   addMenuItem(0, \"<Text>This is the new title</Text>\")"
														 "]]></Script>";
  Menu::init();
  StyleSheet::active->load("../test/style.json");
  Menu::goTo("", menuDocument, DataSource::Document);
	//Menu::active->setClickCallback([](std::shared_ptr<MenuItem> it) { });
  Menu::run();
	std::this_thread::sleep_for(std::chrono::seconds(1));
  //Menu::popUp("", menuDocument, DataSource::Document);
	//std::this_thread::sleep_for(std::chrono::seconds(1));
  //Menu::goTo("Test", "./menu.xml");
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	Menu::quit();
  return (0);
}