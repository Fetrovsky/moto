#include "bmp.h"

#include <cstdint>


namespace encoding
{
    struct __attribute__((packed)) BMP_File_Header
    {
        char Magic[2];
        uint32_t Full_Size;
        uint16_t MBZ_1;
        uint16_t MBZ_2;
        uint32_t Bitmap_Offset;

        BMP_File_Header(uint32_t full_size, uint32_t offset):
            Magic{'B', 'M'},
            Full_Size(full_size),
            MBZ_1(0),
            MBZ_2(0),
            Bitmap_Offset(offset)
        {}
    };

    struct __attribute__((packed)) BMP_Header_Win4x
    {
        uint32_t Header_Size;
        int32_t  Image_Width;
        int32_t  Image_Height;
        uint16_t Color_Planes;
        uint16_t Bits_Per_Pixel;
        uint32_t Compression_Methods;
        uint32_t Bitmap_Size_In_Bytes;
        int32_t  Horizontal_Pixels_Per_Meter;
        int32_t  Vertical_Pixels_Per_Meter;
        uint32_t Colors_Used;
        uint32_t Colors_Important;

        BMP_Header_Win4x(uint32_t width, uint32_t height):
            Header_Size(sizeof(*this)),
            Image_Width(width),
            Image_Height(height),
            Color_Planes(1),
            Bits_Per_Pixel(32),
            Compression_Methods(0),
            Bitmap_Size_In_Bytes(0),  // Not needed for uncompressed bitmaps.
            Horizontal_Pixels_Per_Meter(0),
            Vertical_Pixels_Per_Meter(0),
            Colors_Used(0),
            Colors_Important(0)
        {}
    };

    namespace {
        char magic[2] = {'B', 'M'};

        template<typename T>
        void
        write(
                std::ostream& stream,
                T const& field,
                size_t size = sizeof(T))
        {
            stream.write(reinterpret_cast<char const*>(&field), size);
        }
    }  // namespace

    void BMP::Stream_To(std::ostream& stream) const
    {
        size_t header_size = sizeof(BMP_File_Header) + sizeof(BMP_Header_Win4x);
        size_t image_byte_count = _image.Size() * _image.Value_Size();
        size_t full_size = header_size + image_byte_count;

        BMP_File_Header file_header(full_size, header_size);
        BMP_Header_Win4x bmp_header(_image.Size_X(), _image.Size_Y());

        write(stream, file_header);
        write(stream, bmp_header);

        int64_t height = _image.Size_Y();
        int64_t width_bytes = _image.Size_X() * _image.Value_Size();

        for (int64_t y = --height; y >= 0; --y)
            write(stream, _image[y][0], width_bytes);
    }

    void BMP::Stream_From(std::istream& stream)
    {
    }
}  // namespace encoding

