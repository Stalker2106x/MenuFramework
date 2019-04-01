#ifndef NATIVEINPUT_HPP_
#define NATIVEINPUT_HPP_

#include <iostream>
#include "InputManager.hpp"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

class NativeInput : public InputManager
{
public:
    NativeInput()
  : InputManager()
  {
  }

  ~NativeInput()
  {
  }

  InputManager::Keys getInput()
  {
    switch (getchar())
    {
      case KEY_LEFT:
        return (InputManager::Keys::Left);
      case KEY_RIGHT:
        return (InputManager::Keys::Right);
      case KEY_UP:
        return (InputManager::Keys::Up);
      case KEY_DOWN:
        return (InputManager::Keys::Down);
      case '\n':
      case '\r':
        return (InputManager::Keys::Enter);
      case '\b':
        return (InputManager::Keys::Backspace);
      default:
        return (InputManager::Keys::None);
    }
    return (InputManager::Keys::None);
  }
};

#endif /* !NATIVEINPUT_HPP_ */