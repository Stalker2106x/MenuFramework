#ifndef NATIVERENDERER_HPP_
#define NATIVERENDERER_HPP_

#include "GraphicsRenderer.hh"
#if defined(_MSC_VER) || defined(__MINGW32__)
	#include <windows.h>
#elif defined(__GNUC__) && !defined(__MINGW32__)
    #include <unistd.h>
#endif

class NativeRenderer : public GraphicsRenderer
{
public:
	NativeRenderer()
	: GraphicsRenderer()
	{
	}

	~NativeRenderer()
	{
	}

	virtual void clear()
	{
#if defined(_MSC_VER) || defined(__MINGW32__)
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
#elif defined(__GNUC__) && !defined(__MINGW32__)
    std::cout << "\033[2J\033[1;1H"; //AINSI Sequence
#endif
	_cursor = Point(0,0);
  }

	virtual void draw(const std::string &str, int x, int y)
	{
		while (_cursor.y < y)
		{
			std::cout << "\n";
            _cursor.y++;
            _cursor.x = 0;
		}
        while (_cursor.x < x)
		{
			std::cout << " ";
            _cursor.x++;
		}
        if (!str.empty())
        {
            std::cout << str;
            _cursor.x += str.length();
        }
        std::cout << std::flush;
	}

private:
	Point _cursor;
};

#endif /* !NATIVERENDERER_HPP_ */
