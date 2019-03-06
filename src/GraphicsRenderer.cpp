#include "GraphicsRenderer.hh"

GraphicsRenderer::GraphicsRenderer(int x, int y, int fontsize, int spacing)
  : _position(x, y), _fontsize(fontsize), _spacing(spacing)
{
}

GraphicsRenderer::~GraphicsRenderer()
{
}

void GraphicsRenderer::print(std::string str)
{
  if (str.find("\n") == std::string::npos)
  {
    draw(str, _cursor.x, lineToY(_cursor.y));
    return;
  }
  for (size_t i = str.find("\n"); i != std::string::npos; i++)
  {
    draw(str.substr(0, i), _cursor.x, lineToY(_cursor.y));
    _cursor.y++;
    if (i < str.length() - 1)
    {
      i++;
      str.erase(i, str.length() - i);
    }
    else return;
  }
}

void GraphicsRenderer::clearScreen()
{
  clear();
  _cursor = Point(0, 0);
}

int GraphicsRenderer::lineToY(int line)
{
  return (_position.y + (line * (_fontsize + _spacing)));
}

GraphicsRenderer &GraphicsRenderer::operator<<(std::function<GraphicsRenderer &(GraphicsRenderer &renderer)> f)
{
	return (f(*this));
}

// I/O Streaming

GraphicsRenderer &operator<<(GraphicsRenderer &renderer, const char *str)
{
  renderer.print(str);
  return (renderer);
}

GraphicsRenderer &operator<<(GraphicsRenderer &renderer, const std::string str)
{
  return (renderer << str.c_str());
}

GraphicsRenderer &operator<<(GraphicsRenderer &renderer, int data)
{
  return (renderer << std::to_string(data));
}

GraphicsRenderer &resetAttrs(GraphicsRenderer &renderer)
{
	renderer.resetAttrs();
  return (renderer);
}