#ifndef IMAGE_H__INCLUDED
#define IMAGE_H__INCLUDED

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <vector>


namespace image {
    template<typename Value_Type, int dimensions = 2>
    class Value_Matrix;

    template<typename Value_Type>
    class Basic_Image;

    using Image = Basic_Image<uint32_t>;
}

using image::Image;

template<typename Value_Type, int dimensions>
class image::Value_Matrix
{
    public:
        typedef Value_Type value_type;

    private:
        std::vector<Value_Type> _flat_array;
        std::array<size_t, dimensions> _sizes;

        template<size_t D = dimensions>
        size_t _compute_size(size_t const* indices) const
        {
            if constexpr(D == 0)
            {
                return 0;
            } else {
                return
                    std::accumulate(
                        &indices[0], &indices[D], 1,
                        [](size_t a, size_t x) { return a * x; });
            }
        }

        template<size_t D = dimensions>
        size_t _flatten_indices(size_t const* indices) const
        {
            if constexpr(D == 0)
            {
                return 0;
            } else {
                return (
                    (indices[0] * _compute_size<dimensions - 1>(&_sizes[1])) +
                    (_flatten_indices<D - 1>(&indices[1])));
            }
        }

    public:
        Value_Matrix(std::array<size_t, dimensions> const& sizes):
            _flat_array(_compute_size(&sizes[0]), Value_Type()),
            _sizes(sizes)
        {}

        Value_Type const& Get(std::array<size_t, dimensions> const& sizes) const
        { return _flat_array[_flatten_indices(&sizes[0])]; }

        Value_Type& Get(std::array<size_t, dimensions> const& sizes)
        { return _flat_array[_flatten_indices(&sizes[0])]; }

        size_t Get_Size(size_t dimension) const
        { return _sizes[dimension]; }

        size_t Get_Flat_Size() const
        { return _flat_array.size(); }
};

template<typename Value_Type>
class image::Basic_Image
{
    public:
        typedef Value_Type value_type;

    private:
        Value_Matrix<Value_Type> _rep;

    public:
        Basic_Image(size_t size_x, size_t size_y):
            _rep({size_y, size_x})
        {}

        size_t Size_X() const
        { return _rep.Get_Size(1); }

        size_t Size_Y() const
        { return _rep.Get_Size(0); }

        size_t Size() const
        { return _rep.Get_Flat_Size(); }

        size_t Value_Size() const
        { return sizeof(Value_Type); }

        Value_Type const* operator[](size_t row_index) const
        { return &_rep.Get({row_index, 0}); }

        Value_Type* operator[](size_t row_index)
        { return &_rep.Get({row_index, 0}); }
};

#endif  // IMAGE_H__INCLUDED

