#include "boost/program_options.hpp"

using namespace boost;
namespace po = boost::program_options;

#include "image_provider/image_provider.h"
#include "image_provider/mapnik_c_api.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string FONTDIR = "FONTDIR";

string get_env_var(string const &key)
{
    char *val = getenv(key.c_str());
    return val == NULL ? "" : string(val);
}

int main(int argc, char *argv[])
{
    po::options_description map_input("mapnik_render options");
    vector<double> bbox;
    string trgt_img = "";
    string xml_info = "";
    bool is_xml_string = false;

    try
    {
        map_input.add_options()("help,h", "show available options")("xml_dir", po::value<string>(), "imported config")("trgt_img, img", po::value<string>(&trgt_img)->default_value("/Users/jingyusu/Desktop/test.png"), "export filepath")("bbox", po::value<vector<double>>(&bbox)->multitoken()->composing(), "bbox of tile")("height", po::value<int>()->default_value(256))("width", po::value<int>()->default_value(256))("scale", po::value<float>()->default_value(1.0))("xml_string", po::value<string>(), "xml config passed as string");
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what() << '\n';
        return -1;
    }

    // std::cout << "Have " << argc << " arguments:" << std::endl;
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
        std::cerr << "either xml_dir or xml_string is required" << '\n';
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
        std::cerr << "wrong size of bbox, should be 4: " << bbox.size() << '\n';
        return -1;
    }
    /*------- image processing -------*/
    const std::string font_dir = get_env_var(FONTDIR);

    // double bbox[4] = {13529488.098648008 3659472.8978689983 13530101.233294496 3660085.70353947};
    // double bbox[4] = {13529488.05499435, 3660084.1121049374, 13530101.189715622, 3660696.920278768};
    ImageProvider image_provider = ImageProvider(font_dir);
    image_provider.render_area(xml_info, bbox.data(), trgt_img, input["width"].as<int>(), input["height"].as<int>(), input["scale"].as<float>(), is_xml_string);
}
