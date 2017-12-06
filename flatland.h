#ifndef FLATLAND_H__INCLUDED
#define FLATLAND_H__INCLUDED

#include "image.h"

namespace flatland
{
    struct Point
    {
        float X;
        float Y;
    };

    struct Size
    {
        float Width;
        float Height;
    };

    inline
    bool operator<(Point a, Point b)
    {
        return ((a.Y < b.Y) || ((a.Y == b.Y) && (a.X < b.X)));
    }

    struct Triangle
    {
        Point Points[3];
        uint32_t Color;

        Triangle(Point points[3], uint32_t color):
            Triangle(points[0], points[1], points[2], color)
        {}

        Triangle(Point p0, Point p1, Point p2, uint32_t color):
            Points{p0, p1, p2},
            Color(color)
        {
            std::sort(&Points[0], &Points[3]);
        }
    };

    class Viewport
    {
        private:
            Image& _view;
            Point _bottom_left;
            Size _scale;

            int32_t translate_x(float x)
            { return int32_t((x - _bottom_left.X) / _scale.Width); }

            int32_t translate_y(float y)
            { return _view.Size_Y() - int32_t((_bottom_left.Y + y) / _scale.Height); }

            void draw_horizontal(float begin, float end, float y, uint32_t Color);
            void draw_basic(Point p0, Point p1, Point p2, uint32_t Color);
            void draw(Point p0, Point p1, Point p2, uint32_t Color);

        public:
            Viewport(Image& view):
                _view(view),
                _bottom_left({0.0, 0.0}),
                _scale({1.0, 1.0})
            {}

            void Draw(Triangle const& triangle)
            {
                draw(
                        triangle.Points[0],
                        triangle.Points[1],
                        triangle.Points[2],
                        triangle.Color);
            }
    };

    class World
    {
        private:
            std::vector<Triangle> _triangles;

        public:
            World():
                _triangles()
            {
                _triangles.reserve(1e6);
            }

            void Add(Triangle const& t)
            {
                _triangles.push_back(t);
            }

            void Draw(Viewport& viewport)
            {
                for_each(
                        _triangles.begin(), _triangles.end(),
                        [&viewport](Triangle const& t)
                        { viewport.Draw(t); });
            }
    };
}  // namespace Flatland

#endif  // FLATLAND_H__INCLUDED

