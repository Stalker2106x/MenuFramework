#ifndef GRAPHICSRENDERER_HH_
#define GRAPHICSRENDERER_HH_

#include <string>
#include <functional>

struct Point {
  int x;
  int y;

  Point (int x_ = 0, int y_ = 0)
   : x(x_), y(y_)
  {
  }
};

class GraphicsRenderer
{
public:
  enum Color {
    White,
    Black,
    Yellow,
	  Red,
	  Blue,
	  Green,
	  Magenta,
	  Cyan
  };

  GraphicsRenderer(int x, int y, int fontsize = 1, int spacing = 0);
  virtual ~GraphicsRenderer();

  virtual void clear() = 0;
  virtual void draw(const std::string &str, int x, int y) = 0;
  virtual void setColor(GraphicsRenderer::Color color) = 0;
  virtual void resetAttrs() = 0;

  void clearScreen();
  void print(std::string str);

  int lineToY(int line);

  GraphicsRenderer &operator<<(std::function<GraphicsRenderer&(GraphicsRenderer &term)>);
  friend GraphicsRenderer &operator<<(GraphicsRenderer &term, const char *str);
  friend GraphicsRenderer &operator<<(GraphicsRenderer &term, const std::string str);
  friend GraphicsRenderer &operator<<(GraphicsRenderer &term, int data);

protected:
  Point _cursor;
  Point _position;
  int _fontsize;
  int _spacing;
};

GraphicsRenderer &operator<<(GraphicsRenderer &term, const char *str);
GraphicsRenderer &operator<<(GraphicsRenderer &term, const std::string str);
GraphicsRenderer &operator<<(GraphicsRenderer &term, int data);

class setColor
{
public:
    explicit setColor(GraphicsRenderer::Color c): color(c) {}

    friend GraphicsRenderer& operator<<(GraphicsRenderer& renderer, const setColor& colorManip)
    {
	    renderer.setColor(colorManip.color);
      return (renderer);
    }

    GraphicsRenderer::Color color;
};

GraphicsRenderer &resetAttrs(GraphicsRenderer &renderer);

#endif /* !GRAPHICSRENDERER_HH_ */