#include "image_provider/image_provider_file.h"
#include "image_provider/mapnik_c_api.h"
#include <string>

ImageProviderFile::ImageProviderFile(std::string const &font_dir): ImageProvider(font_dir)
{
}

ImageProviderFile::~ImageProviderFile()
{
}

imgresult ImageProviderFile::render_area(
    std::string const &xml_config,
    double *const bbox,
    std::string const &image_path,
    int width,
    int height,
    float scale_factor,
    float scale,
    bool xml_string_loading)
{
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
    return std::make_pair(image_path, status);
}
