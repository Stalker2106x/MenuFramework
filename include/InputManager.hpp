#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

#include "export.h"

struct MF_API Key {
  enum Code {
    None = -1,
    Up,
    Left,
    Right,
    Down,
    Enter,
    Backspace,
    ASCII,
    F11
  };

  Key(Code c = Code::None)
  : code(c)
  {
  }

  Code code;
  char value;
};

class MF_API InputManager
{
public:

  InputManager() {};
  virtual ~InputManager() {};

  virtual Key getInput() = 0;

protected:

};

#endif /* !INPUTMANAGER_HPP_ */