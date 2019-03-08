#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

class MF_API InputManager
{
public:
  enum Keys {
    None = -1,
    Up,
    Left,
    Right,
    Down,
    Enter,
    Backspace,
    F11
  };

  InputManager() {};
  virtual ~InputManager() {};

  virtual Keys getInput() = 0;

protected:

};

#endif /* !INPUTMANAGER_HPP_ */