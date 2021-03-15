#pragma once
#ifndef IMAGE_PROVIDER_IMAGE_PROVIDER_H
#define IMAGE_PROVIDER_IMAGE_PROVIDER_H

#include "image_provider/mapnik_c_api.h"
#include <string>

class ImageProvider
{
    static const std::string INPUT_PLUGIN_DIR;
    static const std::string FONTDIR;

public:
    explicit ImageProvider(std::string const &font_dir);
    virtual ~ImageProvider();
    virtual imgresult render_area(std::string const &xml_config,
                                  double *const bbox,
                                  std::string const &image_path = "",
                                  int width = 256,
                                  int height = 256,
                                  float scale_factor = 1.0,
                                  float scale = 0.0,
                                  bool xml_string_loading = false) = 0;
    virtual std::string get_err_log();
    static int register_resources();

protected:
    int status;
    mapnik_map_t *map;
    mapnik_bbox_t *bbox2d;

private:
    std::string base_path;
    std::string font_dir;
    // std::unique_ptr<mapnik_map_t> map;
    // std::unique_ptr<mapnik_bbox_t> bbox2d;
};
#endif