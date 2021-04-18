#include "image.hh"

#include <png.h>

namespace utils
{
class ImagePNG : public Image
{
  public:
    void load(const std::string& filename)
    {
        // Open file
        FILE* fp = fopen(filename.c_str(), "rb");
        if (!fp)
            throw std::runtime_error("File " + filename +
                                     " could not be opened.");

        // Check if valid PNG image file
        unsigned char header[8];
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8))
            throw std::runtime_error("File " + filename +
                                     " is not a valid PNG file.");

        png_structp png_write_ptr =
            png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_write_ptr)
            throw std::runtime_error("Call to png_create_read_struct failed.");

        png_infop png_info_ptr = png_create_info_struct(png_write_ptr);
        if (!png_info_ptr)
            throw std::runtime_error("Call to png_create_info_struct failed.");

        if (setjmp(png_jmpbuf(png_write_ptr)))
            throw std::runtime_error("Call to png_jmpbuf failed.");

        png_init_io(png_write_ptr, fp);
        png_set_sig_bytes(png_write_ptr, 8);

        png_read_info(png_write_ptr, png_info_ptr);

        // Retrieve width, height from PNG header
        width_  = png_get_image_width(png_write_ptr, png_info_ptr);
        height_ = png_get_image_height(png_write_ptr, png_info_ptr);
        pixels_ = std::vector<primitives::Color>(width_ * height_);

        if (png_get_color_type(png_write_ptr, png_info_ptr) !=
            PNG_COLOR_TYPE_RGB)
            throw std::runtime_error("Color type of " + filename +
                                     " is not supported.");

        png_set_interlace_handling(png_write_ptr);
        png_read_update_info(png_write_ptr, png_info_ptr);

        if (setjmp(png_jmpbuf(png_write_ptr)))
            throw std::runtime_error("Call to png_jmpbuf failed.");

        // Allocate buffer containing image content
        png_bytep* data = (png_bytep*)malloc(sizeof(png_bytep) * height_);
        for (uint y = 0; y < height_; y++)
            data[y] = (png_byte*)malloc(
                png_get_rowbytes(png_write_ptr, png_info_ptr));

        // Read image content
        png_read_image(png_write_ptr, data);
        fclose(fp);

        // Store image content
        for (uint y = 0; y < height_; y++)
        {
            png_byte* row = data[y];
            for (uint x = 0; x < width_; x++)
            {
                png_byte* color = &(row[x * 3]);
                pixel(x, y) = primitives::Color(color[0], color[1], color[2]);
            }
        }

        // Free memory
        for (uint y = 0; y < height_; y++)
            free(data[y]);
        free(data);
    }

    void save(const std::string& filename)
    {
        // Create file
        FILE* fp = fopen(filename.c_str(), "wb");
        if (!fp)
            throw std::runtime_error("File " + filename +
                                     " could not be opened.");

        // Allocate libpng write and info struct
        png_structp png_write_ptr =
            png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_write_ptr)
            throw std::runtime_error("Call to png_create_write_struct failed.");

        png_infop png_png_info_ptr = png_create_info_struct(png_write_ptr);
        if (!png_png_info_ptr)
            throw std::runtime_error("Call to png_create_info_struct failed.");

        if (setjmp(png_jmpbuf(png_write_ptr)))
            throw std::runtime_error("Call to png_jmpbuf failed.");

        png_init_io(png_write_ptr, fp);

        if (setjmp(png_jmpbuf(png_write_ptr)))
            throw std::runtime_error("Call to png_jmpbuf failed.");

        // Write PNG header
        png_set_IHDR(png_write_ptr,
                     png_png_info_ptr,
                     width_,
                     height_,
                     8, // bit depth
                     PNG_COLOR_TYPE_RGB,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE,
                     PNG_FILTER_TYPE_BASE);
        png_write_info(png_write_ptr, png_png_info_ptr);

        if (setjmp(png_jmpbuf(png_write_ptr)))
            throw std::runtime_error("Call to png_jmpbuf failed.");

        // Allocate buffer to store image content
        png_bytep* data = (png_bytep*)malloc(sizeof(png_bytep) * height_);
        for (uint y = 0; y < height_; y++)
            data[y] = (png_byte*)malloc(
                png_get_rowbytes(png_write_ptr, png_png_info_ptr));

        // Write image content
        for (uint y = 0; y < height_; y++)
        {
            png_byte* row = data[y];
            for (uint x = 0; x < width_; x++)
            {
                const auto color = pixel(x, y);
                png_byte*  ptr   = &(row[x * 3]);
                ptr[0]           = color.r;
                ptr[1]           = color.g;
                ptr[2]           = color.b;
            }
        }

        png_write_image(png_write_ptr, data);

        if (setjmp(png_jmpbuf(png_write_ptr)))
            throw std::runtime_error("Call to png_jmpbuf failed.");

        png_write_end(png_write_ptr, NULL);
        fclose(fp);

        // Free memory
        for (uint y = 0; y < height_; y++)
            free(data[y]);
        free(data);
    }
};

} // namespace utils