#ifndef BMP_H__INCLUDED
#define BMP_H__INCLUDED

#include <ostream>

#include "image.h"


namespace encoding
{
    class BMP
    {
        private:
            Image& _image;

        public:
            BMP(Image& image):
                _image(image)
            {}

            void Stream_To(std::ostream& stream) const;
            void Stream_From(std::istream& stream);
    };
}  // namespace Encoding

inline
std::ostream& operator<<(std::ostream& stream, encoding::BMP const& bmp)
{
    bmp.Stream_To(stream);
    return stream;
}

inline
std::istream& operator>>(std::istream& stream, encoding::BMP& bmp)
{
    bmp.Stream_From(stream);
    return stream;
}

#endif  // BMP_H_INCLUDED

