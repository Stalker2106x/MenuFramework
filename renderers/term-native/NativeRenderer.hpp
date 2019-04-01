#ifndef NATIVERENDERER_HPP_
#define NATIVERENDERER_HPP_

#include "GraphicsRenderer.hh"
#ifdef __GNUC__
	#include <unistd.h>
	#include <term.h>
#endif
#ifdef _MSC_VER
	#include <windows.h>
#endif

class NativeRenderer : public GraphicsRenderer
{
public:
	NativeRenderer()
	: GraphicsRenderer(0, 0), _cursor(0, 0)
	{
	}

	~NativeRenderer()
	{
	}

  virtual void setColor(GraphicsRenderer::Color color)
	{
		std::cout << ">";
	}

	virtual void resetAttrs()
	{
	}

	virtual void clear()
	{
#ifdef __GNUC__
  	if (!cur_term)
    {
			int result;
			setupterm(NULL, STDOUT_FILENO, &result);
			if (result <= 0) return;
    }
  	putp(tigetstr("clear"));
#elif _MSC_VER
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR) ' ',
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
#endif
	_cursor = Point(0,0);
  }

	virtual void draw(const std::string &str, int x, int y)
	{
		while (_cursor.y < y)
		{
			std::cout << "\n";
            _cursor.y++;
		}
        if (!str.empty())
        {
            std::cout << str << "\n";
            if (str.back() == '\n') _cursor.x = 0;
            else
            {
                size_t pos = str.find('\n');
                _cursor.x = (pos == std::string::npos ? str.length() : str.length() - pos);
            }
        }
		_cursor.y += std::count(str.begin(), str.end(), '\n') + 1;
	}

private:
	Point _cursor;
};

#endif /* !NATIVERENDERER_HPP_ */