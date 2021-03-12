#pragma once
#ifndef IMAGE_PROVIDER_IMAGE_PROVIDER_STREAM_H
#define IMAGE_PROVIDER_IMAGE_PROVIDER_STREAM_H

#include "image_provider/mapnik_c_api.h"
#include "image_provider/image_provider.h"
#include <string>

class ImageProviderStream: public ImageProvider
{
public:
    explicit ImageProviderStream(std::string const &font_dir);
    ~ImageProviderStream();
    imgresult render_area(std::string const &xml_config,
                          double *const bbox,
                          std::string const &image_path = "",
                          int width = 256,
                          int height = 256,
                          float scale_factor = 1.0,
                          float scale = 0.0,
                          bool xml_string_loading = false);
    virtual std::string get_err_log();

private:
    mapnik_image_t * image;
    mapnik_image_blob_t * output;
};
#endif