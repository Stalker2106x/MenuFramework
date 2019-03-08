#ifndef POINT_HPP_
#define POINT_HPP_

struct MF_API Point {
  int x;
  int y;

  Point (int x_ = 0, int y_ = 0)
   : x(x_), y(y_)
  {
  }
};

#endif /* !POINT_HPP_ */