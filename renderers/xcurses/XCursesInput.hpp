#ifndef INPUT_HH_
#define INPUT_HH_

#include <InputManager.hpp>

class Input : public InputManager
{
public:
  Input::Input()
  : InputManager()
  {

  }

  Input::~Input()
  {

  }

  virtual InputManager::Keys Input::getInput()
  {
    switch (getch())
    {
      case KEY_LEFT:
        return (InputManager::Keys::Left);
        break;
      case KEY_RIGHT:
        return (InputManager::Keys::Right);
        break;
      case KEY_UP:
        return (InputManager::Keys::Up);
        break;
      case KEY_DOWN:
        return (InputManager::Keys::Down);
        break;
      case KEY_ENTER:
      case '\n':
      case '\r':
        return (InputManager::Keys::Enter);
        break;
      case KEY_BACKSPACE:
      case '\b':
        return (InputManager::Keys::Backspace);
        break;
    }
    return (InputManager::Keys::None);
  }

protected:
};

#endif /* !INPUT_HH_ */