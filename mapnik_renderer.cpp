#include "image_provider.h"
#include <string>

int main(int argc, char *argv[])
{
    std::string font_dir = "/Users/jingyusu/projects/datamap-map/const/fonts";
    std::string xml_dir = "/Users/jingyusu/Desktop/25770551472ea94b214de0575322228e97188a17.xml";
    std::string trgt_img = "/Users/jingyusu/Desktop/test.png";

    double bbox[4] = {121.520098071722, 31.1896992447087, 121.578816448752, 31.2421016436171};
    ImageProvider image_provider = ImageProvider(font_dir);
    image_provider.render_area(xml_dir, bbox, trgt_img);
}