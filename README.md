
# MenuFramework

[![Travis CI](https://travis-ci.org/Stalker2106x/MenuFramework.svg?branch=master)](https://travis-ci.org/Stalker2106x/MenuFramework)
![](https://img.shields.io/badge/Modern-C++14-f39f37.svg)
[![Coverage Status](https://coveralls.io/repos/github/Stalker2106x/MenuFramework/badge.svg?branch=master)](https://coveralls.io/github/Stalker2106x/MenuFramework?branch=master)
[![Coverity Status](https://scan.coverity.com/projects/18141/badge.svg)](https://scan.coverity.com/projects/stalker2106x-menuframework)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/f71ea5beeab44b0abfff01b52f426460)](https://www.codacy.com/app/Stalker2106x/MenuFramework?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Stalker2106x/MenuFramework&amp;utm_campaign=Badge_Grade)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Stalker2106x/MenuFramework.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Stalker2106x/MenuFramework/context:cpp)
![](https://img.shields.io/github/license/stalker2106x/MenuFramework.svg)
[![Average time to resolve an issue](https://isitmaintained.com/badge/resolution/stalker2106x/MenuFramework.svg)](http://isitmaintained.com/project/stalker2106x/MenuFramework "Average time to resolve an issue")

## About

MenuFramework is a rich extendable XML menu engine with LUA integrated script engine (C++ exposition capabilities)
It gives an easy way to add interactive interfaces to your program without hassle
Code is cross-platform, tested and compiled on Windows, Linux and MacOS !

![](/test/demo.gif)

### Contributors

	- Stabax
	- Stalker2106

## How to Build ###

### Dependencies ###

	- Nlhomann's json
	- Pugixml
	- Lua
	- Sol2

You need to grab dependencies if they are not installed on your machine (which is probably the case),
you can do so with the following command-line:

    git submodule update --init --recursive

The project itself is based on CMake, which means you have to generate the build tools for it.
You can generate Makefiles, Visual studio solutions, etc with the following commands (when at repository root)

    mkdir build && cd build
    cmake ..

That procedure should generate the correct build tools based on what your cmake is configured for. You can then just build the library (statically is preferred) and link against it!
The library has been tested on *C++14* compliant *MinGW, GCC, and MSBuild*.

# Menu system

## Getting started

The menu framework is component-driven, the core components are the GraphicsRenderer, and the InputManager.
They allow to use any rendering system to render Menus, and any user input mechanism to interact with user interface.

Menu framework uses a specific XML markup for menu, which can be supplied either through C++ Strings, or Filesystem.
A basic example of a program that shows a menu is the following:

 	  std::string menuDocument;
    
	  menuDocument += "<Menu>"
                    " <Text>Menu Example</Text>"
                    " <Sep/>"
                    " <Button Type='Intern' Target='alert(\"Stop pressing me!\")'>Press me!</Button>"
                    " <Button Type='Intern' Target=''>Do nothing</Button>"
                    "</Menu>";
    Menu::goTo("", menuDocument, DataSource::Document);
    Menu::run();

By default, the framework uses a NativeRenderer and NativeInput, which uses respectively system standard output and input (raw mode).
If you want, you can provide implementation for both of them through the following methods:

    static void Menu::setRenderer(std::shared_ptr<GraphicsRenderer> renderer);
    static void Menu::setInputManager(std::shared_ptr<InputManager> inputmgr);

To do so, just create your own renderer and inputmanager, inherit from GraphicsRenderer and InputManager classes, and implement pure virtual functions. Examples can be found [here](renderers/)

## Menu XML elements

All of the following tag/attributes are writable in MenuFramework markup, and will generate the corresponding widget, based on the position in the hierarchy of the XML Document.

<table style="width:100%;">
  <tr>
    <th>Tag</th>
    <th>Attribute</th>
    <th>Description</th>
  </tr>
  <!-- Generic -->
  <tr>
    <td rowspan="2">All</td>
    <td colspan="2">Every element implements these attributes</td>
  </tr>
  <tr>
    <td>Id</td>
    <td>Sets id of element for interaction in script</td>
  </tr>
  <!-- Menu -->
  <tr>
    <td rowspan="2">Menu</td>
    <td colspan="2">Container for menu items</td>
  </tr>
  <tr>
    <td>OnLoad</td>
    <td>scriptId to execute when creating menu</td>
  </tr>
  <!-- Text -->
  <tr>
    <td rowspan="1">Text</td>
    <td colspan="2">Non selectable entry of characters</td>
  </tr>
  <!-- Lang -->
  <tr>
    <td rowspan="1">Lang</td>
    <td colspan="2">Load lang unit with same Id from locale file</td>
  </tr>
  <!-- Sep -->
  <tr>
    <td>Sep</td>
    <td colspan="2">Non selectable horizontal separator</td>
  </tr>
  <!-- Button -->
  <tr>
    <td rowspan="4">Button</td>
    <td colspan="2">Clickable entry</td>
  </tr>
  <tr>
    <td>Type</td>
    <td>Can be any of: { Intern, Goto, Script }</td>
  </tr>
  <tr>
    <td>Target</td>
    <td>Source, function or scriptId to execute</td>
  </tr>
  <tr>
    <td>Path</td>
    <td>Path where the source is located if extern</td>
  </tr>
  <!-- Input -->
  <tr>
    <td rowspan="1">Input</td>
    <td colspan="2">Field to input characters</td>
  </tr>
  <!-- Select -->
  <tr>
    <td rowspan="1">Select</td>
    <td colspan="2">Multi-value selector</td>
  </tr>
  <!-- Option -->
  <tr>
    <td rowspan="2">Option</td>
    <td colspan="2">Selectable value option for select</td>
  </tr>
  <tr>
    <td>Value</td>
    <td>Sets value corresponding to text</td>
  </tr>
  <!-- Script -->
  <tr>
    <td rowspan="2">Script</td>
    <td colspan="2">Lua script. Can be defined outside of a menu to be called by Id, or inside Menu, will be called when parsed.</td>
  </tr>
  <!-- Alert -->
  <tr>
    <td rowspan="2">Alert</td>
    <td colspan="2">Menu embedded alert message</td>
  </tr>
</table>

## Examples

This is an example of a basic menu:

    <Menu Id="ExampleMenu">
      <Text>Example label</Text>
      <Sep/>
      <Button Type='Intern' Target='alert("amazing")'>Print!</Button>
    </Menu>

an advanced menu example :

    <Menu Id="NewGame">
      <Text>Game Creation Menu</Text>
      <Select Text="Select difficulty">
        <Option Value="0"><Lang Id="difficulty.easy"/></Option>
        <Option Value="1"><Lang Id="difficulty.medium"/></Option>
      </Select>
      <Select Id="Locale" Text="Locale">
        <Option Value="en-US">en-US</Option>
        <Option Value="fr-FR">fr-FR</Option>
      </Select>
      <Input Id="Profile"><Lang Id="global.name"/></Input>
      <Button Type="Intern" Target='alert("ok")'>Create Profile</Button>
      <Button Type="Goto" Target="Home">Back</Button>
    </Menu>

## Script Engine (lua)

You may want to have custom behaviour inside your menus, thats why the framework implements its own script engine. The language of the script is lua, which allow very simple learning for easy customization, and exposes C++ Data easily (C++ binding is explained in next chapter)

## Script C++ Binding

If you need helper lua functions, the following are implemented:

<table style="width:100%;">
  <tr>
    <th>Function</th>
    <th>Description</th>
  </tr>
  <!-- Generic -->
  <tr>
    <td>print(string)</td>
    <td>print given string to terminal at current cursor position</td>
  </tr>
  <tr>
    <td>alert(string)</td>
    <td>add an alert with given string to current menu</td>
  </tr>
  <tr>
    <td>tostring(int)</td>
    <td>converts given integer and returns it in string form</td>
  </tr>
  <tr>
    <td>getCursor()</td>
    <td>returns integer position of cursor (Only hovereable items count)</td>
  </tr>
  <tr>
    <td>getInputData(id)</td>
    <td>returns string contained in input of given string Id (id)</td>
  </tr>
  <tr>
    <td>setInputData(id, value)</td>
    <td>sets string data (value) in input of given string Id (id)</td>
  </tr>
  <tr>
    <td>getSelectValue(id)</td>
    <td>returns string data (value) of selected option in select of given string Id (id)</td>
  </tr>
  <tr>
    <td>setSelectValue(id, value)</td>
    <td>sets option data (value) to select inside select of given string Id (id)</td>
  </tr>
  <tr>
    <td>addMenuItem(pos, xml)</td>
    <td>add Item in menu at position (pos) with XML Data (xml)</td>
  </tr>
</table>

## Adding lua to your menus

This is an example of a script that prints a menu that lists items and allows to select one:

    <Menu Id="Test" OnLoad="TestOnLoad">
      <Text>Select an item</Text>
      <Sep/>
      <!-- Items will be inserted here -->
      <Sep/>
      <Button Type="Intern" Target="exit()">Quit</Button>
    </Menu>
    <Script Id="TestOnLoad"><![CDATA[
      i = 0
      while (i < 5) do
        addMenuItem(2 + i, "<Button Type='Intern' Target='alert(\"selected: "..i.." !\")'>Item "..i.."</Button>")
        i = i + 1
      end
    ]]></Script>

Will output:

    Select an item

    Item 1
    Item 2
    Item 3
    Item 4

## Internationalization

By default, the menu framework does not uses the "locale" system. You can load any locale when starting the program, using the static "load" function in localization where langCode is the locale JSON file name.

    Localization::load("en-US"); //Loads english locale

Now, the program will look inside the current working directory for a file named "./en-US.json", which will contains a JSON formatted translated units like so:

    {
      "localization": "English",
      "data": {
        "any.key": "Translated value"
      }
    }

You can change the folder where the localization looks for by altering the following variable

    Localization::langLocation = "./my/localization/folder/";

You can insert "Localized" string in your menus through the "Lang" XML element, which will load the corresponding text with Id in the locale file. For example, the following menu will show the previous "any.key" defined unit.

    <Menu>
      <Lang Id="any.key">
    </Menu>