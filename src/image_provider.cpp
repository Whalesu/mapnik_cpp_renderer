#include "image_provider/image_provider.h"
#include "image_provider/mapnik_c_api.h"
#include "utils/util.h"
#include <iostream>
#include <string>

std::string const ImageProvider::INPUT_PLUGIN_DIR = "INPUT_PLUGIN_DIR";
std::string const ImageProvider::FONTDIR = "FONTDIR";

ImageProvider::ImageProvider(std::string const &font_dir): font_dir(font_dir), status(0), map(mapnik_map(0,0)), bbox2d(nullptr)
{
}

ImageProvider::~ImageProvider()
{
    mapnik_register_reset_last_error();
    mapnik_map_free(map);
    if (bbox2d != nullptr)
    {
        mapnik_bbox_free(bbox2d);
    }
}

int ImageProvider::register_resources()
{
    int reg_status = 0;
    reg_status = mapnik_register_fonts(get_env_var(ImageProvider::FONTDIR)) || mapnik_register_datasources(get_env_var(ImageProvider::INPUT_PLUGIN_DIR));
    return reg_status;
}

std::string ImageProvider::get_err_log()
{
    if (!(mapnik_register_last_error().empty()))
    {
        return mapnik_register_last_error();
    }
    else if (mapnik_map_last_error(map) != nullptr)
    {
        return *mapnik_map_last_error(map);
    }
    else
    {
        return "";
    }
}

int ImageProvider::render_area(std::string const &xml_config,
                               double *const bbox,
                               std::string const &image_path,
                               int width,
                               int height,
                               float scale_factor,
                               float scale,
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
    }
    mapnik_map_set_width(map, width);
    mapnik_map_set_height(map, height);
    bbox2d = mapnik_bbox(*bbox, *(bbox + 1), *(bbox + 2), *(bbox + 3));
    mapnik_map_zoom_to_box(map, bbox2d);
    if (status == 0)
        status = mapnik_map_render_to_file(map, image_path, scale, scale_factor, "png256");
    return status;
}
