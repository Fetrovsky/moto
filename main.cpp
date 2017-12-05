#include <fstream>

#include "bmp.h"
#include "image.h"
#include "flatland.h"

using flatland::Point;
using flatland::Triangle;

int main()
{
    Image frame_buffer(1024, 768);
    flatland::Viewport viewport(frame_buffer);
    flatland::World flat_world;

    flat_world.Add(
            Triangle{Point{100, 700}, Point{500, 400}, Point{10, 50}, 0x008000f0});

    flat_world.Draw(viewport);

    std::ofstream stream("my.bmp");
    stream << encoding::BMP(frame_buffer);

    return 0;
}

