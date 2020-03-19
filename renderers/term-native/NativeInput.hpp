#ifndef NATIVEINPUT_HPP_
#define NATIVEINPUT_HPP_

#include <iostream>
#include "InputManager.hpp"
#if defined(_MSC_VER) || defined(__MINGW32__)
    #include <windows.h>
#elif (defined(__GNUC__) && !defined(__MINGW32__)) || defined(__clang__)
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>

    #define KEY_UP 4283163
    #define KEY_RIGHT 4414235
    #define KEY_DOWN 4348699
    #define KEY_LEFT 4479771
    /* #define KEY_ARROW_MASK  0x3
     * #define KEY_LEFT        75
     * #define KEY_RIGHT       77
     * #define KEY_UP          72
     * #define KEY_DOWN        80 */
#endif


class NativeInput : public InputManager
{
public:
    NativeInput()
  : InputManager()
  {
#if (defined(__GNUC__) && !defined(__MINGW32__)) || defined(__clang__)
    tcgetattr(0, &termCfg); // grab old terminal i/o settings
    struct termios raw = termCfg;
    raw.c_lflag &= ~ICANON; /* disable buffered i/o */
    tcsetattr(0, TCSANOW, &raw);
#endif
  }

  ~NativeInput()
  {
#if (defined(__GNUC__) && !defined(__MINGW32__)) || defined(__clang__)
    tcsetattr(0, TCSANOW, &termCfg);
#endif
  }

  Key getInput()
  {
#if defined(_MSC_VER) || defined(__MINGW32__)
      HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
      DWORD Events = 0;     // Event count
      DWORD EventsRead = 0; // Events read from console
      DWORD timeoutTick = GetTickCount() + 3000;

      while (GetTickCount() < timeoutTick)
      {
          GetNumberOfConsoleInputEvents(hStdIn, &Events);
          if (Events != 0)
          {
              Key key;
              INPUT_RECORD *eventBuffer = new INPUT_RECORD[Events];

              ReadConsoleInput(hStdIn, eventBuffer, Events, &EventsRead);
              for (DWORD i = 0; i < EventsRead; ++i)
              {
                  if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown)
                  {
                      switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode)
                      {
                      case VK_LEFT:
                          key.code = Key::Code::Left;
						  break;
                      case VK_RIGHT:
                          key.code = Key::Code::Right;
						  break;
                      case VK_UP:
                          key.code = Key::Code::Up;
						  break;
                      case VK_DOWN:
                          key.code = Key::Code::Down;
						  break;
                      case VK_RETURN:
                          key.code = Key::Code::Enter;
						  break;
                      case VK_BACK:
                          key.code = Key::Code::Backspace;
						  break;
                      default:
                          key.code = Key::Code::None;
						  break;
                      }
                      if (key.code == Key::Code::None
                          && ((eventBuffer[i].Event.KeyEvent.wVirtualKeyCode >= 'A'
                               &&  eventBuffer[i].Event.KeyEvent.wVirtualKeyCode <= 'Z')
                              || (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode >= '0'
                               &&  eventBuffer[i].Event.KeyEvent.wVirtualKeyCode <= '9')
                              || (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_SPACE)))
                      {
                        key.code = Key::Code::ASCII;
                        key.value = eventBuffer[i].Event.KeyEvent.wVirtualKeyCode;
                      }
                      if (key.code != Key::Code::None)
                      {
                          delete[] eventBuffer;
                          return (key);
                      }
                  }
              }
              delete[] eventBuffer;
          }
          else
          {
            Sleep(50);
          }
      }
#elif (defined(__GNUC__) && !defined(__MINGW32__)) || defined(__clang__)
    int buffer = 0;
    Key key;

    struct timeval timeout = {3, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(4, &fds, NULL, NULL, &timeout) > 0)
    {
        read(STDIN_FILENO, &buffer, 4);
        switch (buffer)
        {
        case KEY_LEFT:
            key.code = Key::Code::Left;
            break;
        case KEY_RIGHT:
            key.code = Key::Code::Right;
            break;
        case KEY_UP:
            key.code = Key::Code::Up;
            break;
        case KEY_DOWN:
            key.code = Key::Code::Down;
            break;
        case '\n':
        case '\r':
            key.code = Key::Code::Enter;
            break;
        case '\b':
            key.code = Key::Code::Backspace;
            break;
        default:
            key.code = Key::Code::None;
            break;
        }
        if (key.code == Key::Code::None
            && ((buffer >= 'A' &&  buffer <= 'Z')
                || (buffer >= '0' &&  buffer <= '9')
                || (buffer >= 'a' &&  buffer <= 'z')
                || (buffer == ' ')))
        {
            key.code = Key::Code::ASCII;
            key.value = (char)buffer;
        }
    }
    if (key.code != Key::Code::None)
    {
        return (key);
    }
#endif
    return (Key::Code::None);
  }
private:
#if (defined(__GNUC__) && !defined(__MINGW32__)) || defined(__clang__)
    struct termios termCfg;
#endif
};

#endif /* !NATIVEINPUT_HPP_ */
