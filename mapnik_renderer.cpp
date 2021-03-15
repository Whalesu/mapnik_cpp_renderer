#include <boost/program_options.hpp>
#include <boost/format.hpp>

using namespace boost;
namespace po = boost::program_options;

#include "image_provider/image_provider_file.h"
#include "image_provider/image_provider_stream.h"
#include "image_provider/mapnik_c_api.h"
#include "utils/util.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string FONTDIR = "FONTDIR";

std::shared_ptr<ImageProvider> make_provider(const string &font_dir, const string &output_type)
{
    if (output_type == "file")
    {
        return std::shared_ptr<ImageProvider>(new ImageProviderFile(font_dir));
    }
    else if (output_type == "stream")
    {
        return std::shared_ptr<ImageProvider>(new ImageProviderStream(font_dir));
    }
    else
    {
        throw std::logic_error(str(boost::format("Output type: %1% is not supported") % output_type));
    }
}

int main(int argc, char *argv[])
{
    po::options_description map_input("mapnik_render options");
    vector<double> bbox;
    string trgt_img = "";
    string xml_info = "";
    int res = 0;
    bool is_xml_string = false;
    string output_type = "";
    imgresult img_res = make_pair("", 0);

    try
    {
        map_input.add_options()
        ("help,h", "show available options")
        ("xml_dir", po::value<string>(), "imported config")
        ("trgt_img, img", po::value<string>(&trgt_img)->default_value("/Users/jingyusu/Desktop/test.png"), "export filepath")
        ("bbox", po::value<vector<double>>(&bbox)->multitoken()->composing(), "bbox of tile")
        ("height", po::value<int>()->default_value(256), "tile height in pixel")
        ("width", po::value<int>()->default_value(256), "tile width in pixel")
        ("scale_factor", po::value<float>()->default_value(1.0), "scale factor")
        ("xml_string", po::value<bool>(), "optional xml config passed as string")
        ("output_type", po::value<string>(&output_type)->default_value("file"), "output type, support file and stream");
    }
    catch (std::exception &ex)
    {
        cerr << ex.what() << '\n';
        return -1;
    }

    po::variables_map input;
    po::store(po::parse_command_line(argc, argv, map_input), input);
    po::notify(input);

    if (input.count("help"))
    {
        cout << map_input << endl;
        return 0;
    }

    if (!(input.count("xml_dir") ^ input.count("xml_string")))
    {
        cerr << "either xml_dir or xml_string is required" << '\n';
        return -1;
    }
    else if (input.count("xml_dir"))
    {
        xml_info = input["xml_dir"].as<string>();
    }
    else
    {
        xml_info = input["xml_string"].as<string>();
        is_xml_string = true;
    }

    if (bbox.size() != 4)
    {
        cerr << "wrong size of bbox, should be 4: " << bbox.size() << '\n';
        return -1;
    }
    /*------- image processing -------*/
    const std::string font_dir = get_env_var(FONTDIR);
    res = ImageProvider::register_resources();
    std::shared_ptr<ImageProvider> image_provider = make_provider(font_dir, output_type);
    if (image_provider == nullptr)
        return -1;

    if (res == 0)
    {
        img_res = image_provider ->render_area(
                      xml_info, bbox.data(), trgt_img, input["width"].as<int>(),
                      input["height"].as<int>(), input["scale_factor"].as<float>(), is_xml_string
                  );
        res = img_res.second;
        cout<<img_res.first;
    }
    if (res != 0)
        cerr << image_provider->get_err_log()<< '\n';
    return res;
}
