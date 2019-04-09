#include "MenuFramework.hpp"
#include <string>

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
  													 " <Script><![CDATA["
														 "  alert(\"I will show everytime something happens\")"
														 " ]]</Script>"
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
														 "   exit()"
														 "]]></Script>";
  Menu::goTo("", menuDocument, DataSource::Document);
  Menu::run();
  Menu::unload();
  return (0);
}