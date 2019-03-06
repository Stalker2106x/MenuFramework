#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

class InputManager
{
public:
  enum Keys {
    None = -1,
    KEY_UP,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_F11
  };

  InputManager();
  virtual ~InputManager();

  virtual Keys getInput() = 0;

protected:

};

#endif /* !INPUTMANAGER_HPP_ */