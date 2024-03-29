
// Formatted with: astyle  --style=google --pad-oper --add-brackets

#include <mapnik/agg_renderer.hpp>
#include <mapnik/color.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/debug.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/version.hpp>

#if MAPNIK_VERSION < 300000
#error Mapnik 3 is needed
#endif

#include "image_provider/mapnik_c_api.h"

#include <stdlib.h>
#include <string>

typedef mapnik::image_rgba8 mapnik_rgba_image;

static std::string *register_err;

inline void mapnik_register_reset_last_error()
{
    if (register_err)
    {
        delete register_err;
        register_err = nullptr;
    }
}

int mapnik_register_datasource(std::string const &path)
{
    mapnik_register_reset_last_error();
    try
    {
        mapnik::datasource_cache::instance().register_datasource(path);
    }
    catch (std::exception const &ex)
    {
        register_err = new std::string(ex.what());
        return -1;
    }
    return 0;
}

int mapnik_register_datasources(std::string const &dir, bool recurse)
{
    mapnik_register_reset_last_error();
    try
    {
        mapnik::datasource_cache::instance().register_datasources(dir, recurse);
    }
    catch (std::exception const &ex)
    {
        register_err = new std::string(ex.what());
        return -1;
    }
    return 0;
}

int mapnik_register_font(std::string const &path)
{
    mapnik_register_reset_last_error();
    try
    {
        mapnik::freetype_engine::register_font(path);
    }
    catch (std::exception const &ex)
    {
        register_err = new std::string(ex.what());
        return -1;
    }
    return 0;
}

int mapnik_register_fonts(std::string const &dir, bool recurse)
{
    mapnik_register_reset_last_error();
    try
    {
        mapnik::freetype_engine::register_fonts(dir, recurse);
    }
    catch (std::exception const &ex)
    {
        register_err = new std::string(ex.what());
        return -1;
    }
    return 0;
}

std::string mapnik_register_last_error()
{
    if (register_err)
    {
        return *register_err;
    }
    return "";
}

void mapnik_logging_set_severity(int level)
{
    mapnik::logger::severity_type severity;
    switch (level)
    {
    case MAPNIK_DEBUG:
        severity = mapnik::logger::debug;
        break;
    case MAPNIK_WARN:
        severity = mapnik::logger::warn;
        break;
    case MAPNIK_ERROR:
        severity = mapnik::logger::error;
        break;
    default:
        severity = mapnik::logger::none;
        break;
    }
    mapnik::logger::instance().set_severity(severity);
}

typedef struct _mapnik_map_t
{
    mapnik::Map *m;
    std::string *err;
} mapnik_map_t;

mapnik_map_t *mapnik_map(unsigned width, unsigned height)
{
    mapnik_map_t *map = new mapnik_map_t;
    map->m = new mapnik::Map(width, height);
    map->err = nullptr;
    return map;
}

void mapnik_map_free(mapnik_map_t *m)
{
    if (m)
    {
        if (m->m)
        {
            delete m->m;
        }
        if (m->err)
        {
            delete m->err;
        }
        delete m;
    }
}

inline void mapnik_map_reset_last_error(mapnik_map_t *m)
{
    if (m && m->err)
    {
        delete m->err;
        m->err = nullptr;
    }
}

std::string mapnik_map_get_srs(mapnik_map_t *m)
{
    if (m && m->m)
    {
        return m->m->srs();
    }
    return NULL;
}

int mapnik_map_set_srs(mapnik_map_t *m, std::string srs)
{
    if (m)
    {
        m->m->set_srs(srs);
        return 0;
    }
    return -1;
}

double mapnik_map_get_scale_denominator(mapnik_map_t *m)
{
    if (m && m->m)
    {
        return m->m->scale_denominator();
    }
    return 0.0;
}

int mapnik_map_load(mapnik_map_t *m, std::string const &stylesheet)
{
    mapnik_map_reset_last_error(m);
    if (m && m->m)
    {
        try
        {
            mapnik::load_map(*m->m, stylesheet);
        }
        catch (std::exception const &ex)
        {
            m->err = new std::string(ex.what());
            return -1;
        }
        return 0;
    }
    return -1;
}

int mapnik_map_load_string(mapnik_map_t *m, std::string const &style_conf)
{
    mapnik_map_reset_last_error(m);
    if (m && m->m)
    {
        try
        {
            mapnik::load_map_string(*m->m, style_conf);
        }
        catch (std::exception const &ex)
        {
            m->err = new std::string(ex.what());
            return -1;
        }
        return 0;
    }
    return -1;
}

void mapnik_apply_layer_off_hack(mapnik_map_t *m)
{
    // Note: Since Mapnik 3 all layers with status="off" are not loaded and cannot
    // be activated by a custom LayerSelector. As a workaround, all layers with names
    // starting with '__OFF__' are disabled on load and the '__OFF__' prefix is removed
    // from the layer name.
    if (m && m->m)
    {
        for (size_t idx = 0; idx < m->m->layer_count(); idx++)
        {
            mapnik::layer &layer = m->m->get_layer(idx);
            if (layer.name().rfind("__OFF__", 0) == 0)
            {
                layer.set_active(false);
                layer.set_name(layer.name().substr(7));
            }
        }
    }
}

int mapnik_map_zoom_all(mapnik_map_t *m)
{
    mapnik_map_reset_last_error(m);
    if (m && m->m)
    {
        try
        {
            m->m->zoom_all();
        }
        catch (std::exception const &ex)
        {
            m->err = new std::string(ex.what());
            return -1;
        }
        return 0;
    }
    return -1;
}

void mapnik_map_resize(mapnik_map_t *m, unsigned int width, unsigned int height)
{
    if (m && m->m)
    {
        m->m->resize(width, height);
    }
}

MAPNIKCAPICALL void mapnik_map_set_buffer_size(mapnik_map_t *m, int buffer_size)
{
    m->m->set_buffer_size(buffer_size);
}

std::string *mapnik_map_last_error(mapnik_map_t *m)
{
    if (m && m->err)
    {
        return m->err;
    }
    return nullptr;
}

struct _mapnik_bbox_t
{
    mapnik::box2d<double> b;
};

mapnik_bbox_t *mapnik_bbox(double minx, double miny, double maxx, double maxy)
{
    mapnik_bbox_t *b = new mapnik_bbox_t;
    b->b = mapnik::box2d<double>(minx, miny, maxx, maxy);
    return b;
}

void mapnik_bbox_free(mapnik_bbox_t *b)
{
    if (b)
    {
        delete b;
    }
}

void mapnik_map_zoom_to_box(mapnik_map_t *m, mapnik_bbox_t *b)
{
    if (m && m->m && b)
    {
        m->m->zoom_to_box(b->b);
    }
}

struct _mapnik_image_t
{
    mapnik_rgba_image *i;
    std::string *err;
};

inline void mapnik_image_reset_last_error(mapnik_image_t *i)
{
    if (i && i->err)
    {
        delete i->err;
        i->err = nullptr;
    }
}

void mapnik_image_free(mapnik_image_t *i)
{
    if (i)
    {
        if (i->i)
        {
            delete i->i;
        }
        if (i->err)
        {
            delete i->err;
        }
        delete i;
    }
}

std::string mapnik_image_last_error(mapnik_image_t *i)
{
    if (i && i->err)
    {
        return *(i->err);
    }
    return "";
}

mapnik_image_t *mapnik_map_render_to_image(mapnik_map_t *m, float scale, float scale_factor)
{
    mapnik_map_reset_last_error(m);
    mapnik_rgba_image *im = new mapnik_rgba_image(m->m->width(), m->m->height());
    if (m && m->m)
    {
        try
        {
            mapnik::agg_renderer<mapnik_rgba_image> ren(*m->m, *im, static_cast<double>(scale_factor));
            if (scale > 0.0)
            {
                ren.apply(static_cast<double>(scale));
            }
            else
            {
                ren.apply();
            }
        }
        catch (std::exception const &ex)
        {
            delete im;
            m->err = new std::string(ex.what());
            return NULL;
        }
    }
    mapnik_image_t *i = new mapnik_image_t;
    i->i = im;
    i->err = nullptr;
    return i;
}

int mapnik_map_render_to_file(mapnik_map_t *m, std::string const &filepath, float scale, float scale_factor, std::string const &format)
{
    mapnik_map_reset_last_error(m);
    if (m && m->m)
    {
        try
        {
            mapnik_rgba_image buf(m->m->width(), m->m->height());
            mapnik::agg_renderer<mapnik_rgba_image> ren(*m->m, buf, scale_factor);
            if (scale > 0.0)
            {
                ren.apply(scale);
            }
            else
            {
                ren.apply();
            }
            mapnik::save_to_file(buf, filepath, format);
        }
        catch (std::exception const &ex)
        {
            m->err = new std::string(ex.what());
            return -1;
        }
        return 0;
    }
    return -1;
}

void mapnik_image_blob_free(mapnik_image_blob_t *b)
{
    if (b)
    {
        if (b->image_blob)
        {
            delete b->image_blob;
        }
        delete b;
    }
}

mapnik_image_blob_t *mapnik_image_to_blob(mapnik_image_t *i, std::string format)
{
    mapnik_image_reset_last_error(i);
    mapnik_image_blob_t *blob = new mapnik_image_blob_t;
    blob->image_blob = nullptr;
    blob->len = 0;
    if (i && i->i)
    {
        try
        {
            blob->image_blob = new std::string(save_to_string(*(i->i), format));
        }
        catch (std::exception const &ex)
        {
            i->err = new std::string(ex.what());
            delete blob;
            return nullptr;
        }
    }
    return blob;
}

const uint8_t *mapnik_image_to_raw(mapnik_image_t *i, size_t *size)
{
    if (i && i->i)
    {
        *size = i->i->width() * i->i->height() * 4;
        return (uint8_t *)i->i->data();
    }
    return NULL;
}

mapnik_image_t *mapnik_image_from_raw(const uint8_t *raw, int width, int height)
{
    mapnik_image_t *img = new mapnik_image_t;
    img->i = new mapnik_rgba_image(width, height);
    memcpy(img->i->data(), raw, width * height * 4);
    img->err = NULL;
    return img;
}

int mapnik_map_layer_count(mapnik_map_t *m)
{
    if (m && m->m)
    {
        return m->m->layer_count();
    }
    return 0;
}

std::string mapnik_map_layer_name(mapnik_map_t *m, size_t idx)
{
    if (m && m->m)
    {
        mapnik::layer const &layer = m->m->get_layer(idx);
        std::string name = layer.name();
        return name;
    }
    return NULL;
}

int mapnik_map_layer_is_active(mapnik_map_t *m, size_t idx)
{
    if (m && m->m)
    {
        mapnik::layer const &layer = m->m->get_layer(idx);
        return layer.active();
    }
    return 0;
}

void mapnik_map_layer_set_active(mapnik_map_t *m, size_t idx, int active)
{
    if (m && m->m)
    {
        mapnik::layer &layer = m->m->get_layer(idx);
        layer.set_active(active);
    }
}

int mapnik_map_background(mapnik_map_t *m, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
    if (m && m->m)
    {
        boost::optional<mapnik::color> const &bg = m->m->background();
        if (bg)
        {
            mapnik::color c = bg.get();
            *r = c.red();
            *g = c.green();
            *b = c.blue();
            *a = c.alpha();
            return 1;
        }
    }
    return 0;
}

void mapnik_map_set_background(mapnik_map_t *m, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (m && m->m)
    {
        mapnik::color bg(r, g, b, a);
        m->m->set_background(bg);
    }
}

void mapnik_map_set_maximum_extent(mapnik_map_t *m, double x0, double y0, double x1, double y1)
{
    if (m && m->m)
    {
        mapnik::box2d<double> extent(x0, y0, x1, y1);
        m->m->set_maximum_extent(extent);
    }
}

void mapnik_map_reset_maximum_extent(mapnik_map_t *m)
{
    if (m && m->m)
    {
        m->m->reset_maximum_extent();
    }
}

void mapnik_map_set_width(mapnik_map_t *m, int width)
{
    if (m && m->m)
    {
        m->m->set_width(width);
    }
}

void mapnik_map_set_height(mapnik_map_t *m, int height)
{
    if (m && m->m)
    {
        m->m->set_height(height);
    }
}
