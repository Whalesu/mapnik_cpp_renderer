#include "image_provider/image_provider.h"
#include "image_provider/mapnik_c_api.h"
#include "utils/util.h"
#include <iostream>
#include <string>

std::string const ImageProvider::INPUT_PLUGIN_DIR = "INPUT_PLUGIN_DIR";
std::string const ImageProvider::FONTDIR = "FONTDIR";

ImageProvider::ImageProvider(std::string const &font_dir): status(0), font_dir(font_dir), map(mapnik_map(0,0)), bbox2d(nullptr)
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
    int reg_status = mapnik_register_fonts(get_env_var(ImageProvider::FONTDIR)) || mapnik_register_datasources(get_env_var(ImageProvider::INPUT_PLUGIN_DIR));
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
