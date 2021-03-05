#include "image_provider/image_provider.h"
#include "image_provider/mapnik_c_api.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 1; i < argc; ++i)
        std::cout << argv[i] << "\n";

    std::string xml_dir(argv[1]);
    // double bbox[4] = argv[2];

    std::string font_dir = "/map/const/fonts";
    std::string trgt_img = "/map/sjy_test/test.png";
    // std::string xml_dir = "/map/sjy_test/25770551472ea94b214de0575322228e97188a17.xml";

    // std::string font_dir = "/Users/jingyusu/projects/datamap-map/const/fonts";
    // std::string trgt_img = "/Users/jingyusu/Desktop/test.png";
    // std::string xml_dir = "/Users/jingyusu/Desktop/15770551472ea94b214de0575322228e97188a17.xml";
    // double bbox[4] = {13529488.098648008, 3659472.8978689983, 13530101.233294496, 3660085.70353947};
    double bbox[4] = {13529488.05499435, 3660084.1121049374, 13530101.189715622, 3660696.920278768};
    ImageProvider image_provider = ImageProvider(font_dir);
    image_provider.render_area(xml_dir, bbox, trgt_img);
}
