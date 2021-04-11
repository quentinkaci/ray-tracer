#include "image.hh"

#include <png.h>

namespace utils
{
class ImagePNG : public Image
{
  public:
    void load(const std::string& filename)
    {
        unsigned char header[8];

        // Open file and check if png file
        FILE* fp = fopen(filename.c_str(), "rb");
        if (!fp)
            throw "File could not be opened.";

        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8))
            throw "File is not a valid PNG file.";

        png_structp png_ptr =
            png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
            throw "Error: png_create_read_struct failed";

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
            throw "Error: png_create_info_struct failed";

        if (setjmp(png_jmpbuf(png_ptr)))
            throw "Error: Error during init_io";

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width_  = png_get_image_width(png_ptr, info_ptr);
        height_ = png_get_image_height(png_ptr, info_ptr);
        pixels_ = new primitives::Color[width_ * height_];

        if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGB)
            throw "Error: PNG color type not supported.";

        png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

        // Read file content
        if (setjmp(png_jmpbuf(png_ptr)))
            throw "Error: error during read_image png";

        png_bytep* row_pointers =
            (png_bytep*)malloc(sizeof(png_bytep) * height_);
        for (uint y = 0; y < height_; y++)
            row_pointers[y] =
                (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));

        png_read_image(png_ptr, row_pointers);

        fclose(fp);

        for (uint y = 0; y < height_; y++)
        {
            png_byte* row = row_pointers[y];
            for (uint x = 0; x < width_; x++)
            {
                png_byte* ptr = &(row[x * 3]);
                double    r   = ptr[0];
                double    g   = ptr[1];
                double    b   = ptr[2];
                pixel(x, y)   = primitives::Color(r, g, b);
            }
        }

        for (uint y = 0; y < height_; y++)
            free(row_pointers[y]);
        free(row_pointers);
    }

    void save(const std::string& filename)
    {
        // Create file
        FILE* fp = fopen(filename.c_str(), "wb");
        if (!fp)
            throw "Error while writing PNG file.";

        png_structp png_ptr =
            png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
            throw "Error: png_create_write_struct failed.";

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
            throw "Error: png_create_info_struct failed.";

        if (setjmp(png_jmpbuf(png_ptr)))
            throw "Error: error during PNG init_io.";

        png_init_io(png_ptr, fp);

        // Write header
        if (setjmp(png_jmpbuf(png_ptr)))
            throw "Error: error during writing PNG header.";

        png_set_IHDR(png_ptr,
                     info_ptr,
                     width_,
                     height_,
                     8, // bit depth
                     PNG_COLOR_TYPE_RGB,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE,
                     PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);

        // Write content
        if (setjmp(png_jmpbuf(png_ptr)))
            throw "Error: error during writing PNG bytes";

        png_bytep* row_pointers =
            (png_bytep*)malloc(sizeof(png_bytep) * height_);
        for (uint y = 0; y < height_; y++)
            row_pointers[y] =
                (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));

        for (uint y = 0; y < height_; y++)
        {
            png_byte* row = row_pointers[y];
            for (uint x = 0; x < width_; x++)
            {
                auto color = pixel(x, y);

                png_byte* ptr = &(row[x * 3]);
                ptr[0]        = color.r;
                ptr[1]        = color.g;
                ptr[2]        = color.b;
            }
        }

        png_write_image(png_ptr, row_pointers);

        if (setjmp(png_jmpbuf(png_ptr)))
            throw "Error: error during end of write PNG";

        png_write_end(png_ptr, NULL);

        fclose(fp);

        for (uint y = 0; y < height_; y++)
            free(row_pointers[y]);
        free(row_pointers);
    }
};

} // namespace utils