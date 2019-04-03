
# MenuFramework

## About

MenuFramework is a full-fledged XML menu engine with LUA integrated script engine (C++ exposition capabilities)

### Contributors

	- Stabax
	- Stalker2106

## How to Build ###

### Dependencies ###

	- Nlhomann's json
	- Pugixml
	- Lua
	- Sol2

You need to grab and build dependencies if they are not installed on your machine (which is probably the case),
you can do so with the following command-lines:

    git submodule init
    git submodule update

The project itself is based on CMake, which means you have to generate the build tools for it.
You can generate Makefiles, Visual studio solutions, etc with the following commands (when at repository root)

    mkdir build && cd build
    cmake ..

Then, it should generate the correct build tools based on what your cmake is configured for.
The library has been tested on *C++14* compliant *MinGW, GCC, and MSBuild*.

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

## Adding menus

You may want to create your own menus. Luckily, the next chapter describes all the elements available to create one.
The menus are defined inside XML documents with a specific syntax and various allowed tags & attributes.

## Menu elements

<table style="width:100%;">
  <tr>
    <th>Tag</th>
    <th>Attribute</th>
    <th>Description</th>
  </tr>
  <!-- Generic -->
  <tr>
    <td rowspan="2">All</td>
    <td colspan="2">Every element implements these methods</td>
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
    <td rowspan="2">Input</td>
    <td colspan="2">Field to input characters</td>
  </tr>
  <!-- Select -->
  <tr>
    <td rowspan="2">Select</td>
    <td colspan="2">multi-value selection item</td>
  </tr>
  <!-- Option -->
  <tr>
    <td rowspan="2">Option</td>
    <td colspan="2">selectable item from select</td>
  </tr>
  <tr>
    <td>Value</td>
    <td>Sets value corresponding to text</td>
  </tr>
  <!-- Script -->
  <tr>
    <td rowspan="2">Script</td>
    <td colspan="2">Lua embedded script called by Id, or position in menu</td>
  </tr>
  <!-- Alert -->
  <tr>
    <td rowspan="2">Alert</td>
    <td colspan="2">Menu embedded alert message</td>
  </tr>
</table>

## Menu example

This is an example of a basic menu written in mf markup:

    <Menu Id="ExampleMenu">
      <Text>Example label</Text>
      <Sep/>
      <Button Type='Intern' Target='alert("amazing")'>Print!</Button>
    </Menu>

This is an advanced menu example :

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

## Write your own scripts

For example, the following script prints a menu that lists items and allows to select one

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