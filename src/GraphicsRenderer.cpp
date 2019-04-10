#include "GraphicsRenderer.hh"

GraphicsRenderer::GraphicsRenderer(int x, int y, int fontsize, int spacing)
  : _position(x, y), _fontsize(fontsize), _spacing(spacing)
{
  _cursor = Point(_position.x, _position.y);
}

GraphicsRenderer::~GraphicsRenderer()
{
}

void GraphicsRenderer::print(std::string str)
{
  size_t nl = str.find("\n");

  while (nl != std::string::npos)
  {
    draw(str.substr(0, nl-1), _cursor.x, lineToY(_cursor.y));
    lineBreak();
    str.erase(0, nl + 1);
    nl = str.find("\n");
  }
  if (str.length() > 0)
  {
    draw(str, _cursor.x, lineToY(_cursor.y));
    _cursor.x += str.length();
  }
}

void GraphicsRenderer::lineBreak()
{
  _cursor.x = _position.x;
  _cursor.y++;
}

void GraphicsRenderer::clearScreen()
{
  clear();
  _cursor = _position;
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

GraphicsRenderer &operator<<(GraphicsRenderer &renderer, const std::string &str)
{
  return (renderer << str.c_str());
}

GraphicsRenderer &operator<<(GraphicsRenderer &renderer, int data)
{
  renderer.print(std::to_string(data));
  return (renderer);
}

GraphicsRenderer &resetAttrs(GraphicsRenderer &renderer)
{
	renderer.resetAttrs();
  return (renderer);
}