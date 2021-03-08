#include "image_provider/image_provider.h"
#include "image_provider/mapnik_c_api.h"
#include <iostream>
#include <string>

// typedef struct _mapnik_map_t mapnik_map_t;
// mapnik_map_t *mapnik_map(unsigned int width, unsigned int height);
// int mapnik_map_load_string(mapnik_map_t *m, std::string const &style_conf);
// int mapnik_map_load(mapnik_map_t *m, const char *stylesheet);

ImageProvider::ImageProvider(std::string const &font_dir)
{
    this->font_dir = font_dir;
    // map.reset(mapnik_map(0, 0));
    int status = mapnik_register_font(font_dir);
    std::string dtsrc = "/usr/local/lib/mapnik/input/postgis.input";
    status = mapnik_register_datasource(dtsrc);
    map = mapnik_map(0, 0);
    bbox2d = nullptr;
}

ImageProvider::~ImageProvider()
{
    mapnik_map_free(map);
    if (bbox2d != nullptr)
    {
        mapnik_bbox_free(bbox2d);
    }
    std::cout << "image removed" << std::endl;
}

std::string ImageProvider::get_err_log()
{
    return *mapnik_map_last_error(map);
}

int ImageProvider::render_area(std::string const &xml_config,
                               double *const bbox,
                               std::string const &image_path,
                               int width,
                               int height,
                               float scale_factor,
                               bool xml_string_loading)
{
    int status = 0;
    if (xml_string_loading == true)
    {
        status = mapnik_map_load_string(map, xml_config);
    }
    else
    {
        status = mapnik_map_load(map, xml_config);
        std::cout << "curr status" << status << std::endl;
    }
    // std::cout << "err output:" << *mapnik_map_last_error(map) << std::endl;
    mapnik_map_set_width(map, width);
    mapnik_map_set_height(map, height);
    bbox2d = mapnik_bbox(*bbox, *(bbox + 1), *(bbox + 2), *(bbox + 3));
    mapnik_map_zoom_to_box(map, bbox2d);
    // std::cout << "err output:" << *mapnik_map_last_error(map) << std::endl;
    status = mapnik_map_render_to_file(map, image_path, 1.0, scale_factor, "png256");
    // std::cout << "err output:" << *mapnik_map_last_error(map) << std::endl;
    return status;
}
