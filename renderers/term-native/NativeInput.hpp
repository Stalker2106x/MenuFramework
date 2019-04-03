#ifndef NATIVEINPUT_HPP_
#define NATIVEINPUT_HPP_

#include <iostream>
#include "InputManager.hpp"
#if defined(_MSC_VER) || defined(__MINGW32__)
    #include <windows.h>
#elif defined(__GNUC__) && !defined(__MINGW32__)
    #include <termios.h>
    #include <unistd.h>
    #define KEY_LEFT    75
    #define KEY_RIGHT   77
    #define KEY_UP      72
    #define KEY_DOWN    80
#endif


class NativeInput : public InputManager
{
public:
    NativeInput()
  : InputManager()
  {
#if defined(__GNUC__) && !defined(__MINGW32__)
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
  }

  ~NativeInput()
  {
  }

  InputManager::Keys getInput()
  {
#if defined(_MSC_VER) || defined(__MINGW32__)
      HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
      DWORD Events = 0;     // Event count
      DWORD EventsRead = 0; // Events read from console

      while (Events == 0)
      {
          GetNumberOfConsoleInputEvents(hStdIn, &Events);
          if (Events != 0)
          {
              INPUT_RECORD *eventBuffer = new INPUT_RECORD[Events]; // create event buffer the size of how many Events

              ReadConsoleInput(hStdIn, eventBuffer, Events, &EventsRead);
              for (DWORD i = 0; i < EventsRead; ++i)
              {
                  if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) // check if event[i] is a key event && if so is a press not a release
                  {
                      switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode)
                      {
                      case VK_LEFT:
                          return (InputManager::Keys::Left);
                      case VK_RIGHT:
                          return (InputManager::Keys::Right);
                      case VK_UP:
                          return (InputManager::Keys::Up);
                      case VK_DOWN:
                          return (InputManager::Keys::Down);
                      case VK_RETURN:
                          return (InputManager::Keys::Enter);
                      case VK_DELETE:
                          return (InputManager::Keys::Backspace);
                      default:
                          return (InputManager::Keys::None);
                      }
                  }
              } // end EventsRead loop
              delete eventBuffer;
          }
      }
#elif defined(__GNUC__) && !defined(__MINGW32__)
    int buffer;

    read(STDIN_FILENO, &buffer, 1);
    switch (buffer)
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
    }
#endif
      return (InputManager::Keys::None);
  }

};

#endif /* !NATIVEINPUT_HPP_ */
