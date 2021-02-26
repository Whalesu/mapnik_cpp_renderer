#ifndef IMAGE_PROVIDERS_H
#define IMAGE_PROVIDERS_H

#include <string>
#include "mapnik_c_api.h"

class ImageProvider
{
public:
    explicit ImageProvider(std::string const &font_dir);
    ~ImageProvider();
    int render_area(std::string const &xml_config,
                    double bbox[],
                    std::string const &image_path = "",
                    int width = 256,
                    int height = 256,
                    float scale_factor = 1.0,
                    bool xml_string_loading = false);

private:
    std::string base_path;
    std::string font_dir;
    mapnik_map_t *map;
    mapnik_bbox_t *bbox2d;
    // std::unique_ptr<mapnik_map_t> map;
    // std::unique_ptr<mapnik_bbox_t> bbox2d;
};
#endif