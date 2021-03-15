#pragma once
#ifndef IMAGE_PROVIDER_IMAGE_PROVIDER_FILE_H
#define IMAGE_PROVIDER_IMAGE_PROVIDER_FILE_H

#include "image_provider/mapnik_c_api.h"
#include "image_provider/image_provider.h"
#include <string>

class ImageProviderFile: public ImageProvider
{
public:
    ImageProviderFile(std::string const &font_dir);
    ~ImageProviderFile();
    imgresult render_area(std::string const &xml_config,
                          double *const bbox,
                          std::string const &image_path = "",
                          int width = 256,
                          int height = 256,
                          float scale_factor = 1.0,
                          float scale = 0.0,
                          bool xml_string_loading = false) ;
};
#endif