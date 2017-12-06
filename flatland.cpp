#include "flatland.h"

#include <algorithm>

#include <iostream>
#define SHOWB(N) (std::cout << #N << ": " << (N? "True": "False") << std::endl)
#define SHOW(N) (std::cout << #N << ": " << N << std::endl)

void flatland::Viewport::draw_horizontal(float begin, float end, float y, uint32_t Color)
{
    auto view_y = translate_y(y);

    if ((view_y < 0) || (view_y >= int32_t(_view.Size_Y())))
    {
        std::cout << std::endl;
        return;  // Vertical trim.  The whole thing goes away.
    }

    // Do translation and horizontal trim.
    auto view_begin = std::max(translate_x(begin), 0);
    auto view_end = std::min(translate_x(end), int32_t(_view.Size_X()) - 1);

    auto row = _view[view_y];
    for (auto it = &row[view_begin]; it <= &row[view_end]; ++it)
        *it = Color;
}

void flatland::Viewport::draw_basic(Point p0, Point p1, Point p2, uint32_t Color)
{
    //
    // Invariant: p0 and p1 have the same value for Y.
    //

    float begin_x_left = p0.X;
    float begin_x_right = p1.X;

    float delta_x_left = p2.X - begin_x_left;
    float delta_x_right = p2.X - begin_x_right;

    float begin_y = p0.Y;
    float end_y = p2.Y;

    float delta_y = end_y - begin_y;
    float inverse_delta_y = 1 / delta_y;

    float inverse_slope_left = delta_x_left * inverse_delta_y;
    float inverse_slope_right = delta_x_right * inverse_delta_y;

    if (delta_y > 0)
    {
        for (float y = 0; y <= delta_y; y+= _scale.Height)
        {
            float begin = inverse_slope_left * y + begin_x_left;
            float end = inverse_slope_right * y + begin_x_right;

            draw_horizontal(begin, end, begin_y + y, Color);
        }
    } else {
        for (float y = 0; y >= delta_y; y-= _scale.Height)
        {
            float begin = inverse_slope_left * y + begin_x_left;
            float end = inverse_slope_right * y + begin_x_right;

            draw_horizontal(begin, end, begin_y + y, Color);
        }
    }
}

void flatland::Viewport::draw(Point p0, Point p1, Point p2, uint32_t Color)
{
    //
    // Case 1: Two points at the top.
    // Technique: Draw top line and adjust endpoints as we move downwards.
    // This case is covered by the Draw_Basic() implementation.
    //

    if (p0.Y == p1.Y)
        return draw_basic(p0, p1, p2, Color);

    //
    // Case 2: Two points at the bottom.
    // Technique: Draw bottom line and adjust endpoints as we move upwards.
    // This case is covered by the Draw_Basic() implementation.
    //

    if (p1.Y == p2.Y)
        return draw_basic(p1, p2, p0, Color);

    //
    // Case 3: No two points share a horizontal.
    // Technique: Draw top line and adjust endpoints as we move downwards.
    //

    float inverse_slope = (p2.X - p0.X) / (p2.Y - p0.Y);
    float delta_y = p1.Y - p0.Y;
    float delta_x = inverse_slope * delta_y;

    Point p1_prime { p0.X + delta_x, p1.Y };

    if (p1_prime.X < p1.X)
        std::swap(p1_prime, p1);

    draw_basic(p1, p1_prime, p0, Color);
    draw_basic(p1, p1_prime, p2, Color);
}

