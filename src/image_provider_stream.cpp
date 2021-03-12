#include "image_provider/image_provider_stream.h"
#include "image_provider/mapnik_c_api.h"
#include <string>

ImageProviderStream::ImageProviderStream(std::string const &font_dir): ImageProvider(font_dir), image(nullptr), output(nullptr)
{
}

ImageProviderStream::~ImageProviderStream()
{
    if(image != nullptr)
    {
        mapnik_image_free(image);
    }
    if(output != nullptr)
    {
        mapnik_image_blob_free(output);
    }
}

std::string ImageProviderStream::get_err_log()
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
        return mapnik_image_last_error(image);
    }
}


imgresult ImageProviderStream::render_area(
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
    std::string output_str = "";
    if (status == 0)
    {
        image = mapnik_map_render_to_image(map, scale, scale_factor);
        std::string output_str = std::string(mapnik_image_to_blob(image,"png8:m=h:z=1")->ptr);
    }
    return std::make_pair(output_str, status);
}
