from subprocess import PIPE
from subprocess import Popen
from subprocess import TimeoutExpired
import os
from io import BytesIO

TIME_LIMIT = 5
EXEC_BIN = '/Users/jingyusu/selfInterest/mapnik_cpp_renderer/mapnik_renderer'


def save_stream_output(data: (bytes, BytesIO), image_name):
    image_directory = '/Users/jingyusu/Desktop/stream_output'
    image_path = '/Users/jingyusu/Desktop/stream_output/' + image_name
    os.makedirs(image_directory, exist_ok=True)
    with open(image_path, 'wb') as f:
        if isinstance(data, bytes):
            f.write(data)
        else:
            f.write(data.getbuffer())


def invoke_cpp_bin_popen(xml_config,
                         bbox,
                         scale_factor=1.0,
                         height=256,
                         width=256,
                         trgt_img=None,
                         xml_string_loading=True):
    args = [
        EXEC_BIN, '--xml_config', xml_config, '--bbox',
        *[f'{coord}' for coord in bbox], '--scale_factor',
        str(scale_factor), '--height',
        str(height), '--width',
        str(width), '--is_xml_string',
        str(xml_string_loading)
    ]

    args += ['--trgt_img', trgt_img
             ] if trgt_img else ['--output_type', 'stream']

    env = {
        'FONTDIR': '/Users/jingyusu/projects/datamap-map/const/fonts',
        'INPUT_PLUGIN_DIR': '/usr/local/lib/mapnik/input'
    }
    outs = ''
    errs = ''
    rc = 0
    with Popen(args, env=env, stdout=PIPE, stderr=PIPE, encoding=None) as proc:
        try:
            outs, errs = proc.communicate(timeout=TIME_LIMIT)
            rc = proc.returncode
        except TimeoutExpired as err:
            proc.kill()
            errs = str(err)
            print("progress time out: ", errs)

    return outs, errs, rc


if __name__ == '__main__':
    xml_dir = '/Users/jingyusu/Desktop/15770551472ea94b214de0575322228e97188a17.xml'
    # xml_dir = '/Users/jingyusu/Desktop/map_test/render_config/88/433471/93ae71e7e220cb6438e0c150bac455a5b1087e5b.xml'
    bbox = [
        13529488.05499435, 3659472.8978689983, 13530101.189715622,
        3660085.70353947
    ]
    trgt_img = '/Users/jingyusu/Desktop/test_1577_bl.png'
    # trgt_img = None
    xml_string_loading = True

    if xml_string_loading:
        with open(xml_dir, 'r') as config_file:
            xml_dir = config_file.read()

    scale_factor = 2.0
    height = 512
    width = 512
    if trgt_img:
        outs, errs, rc = invoke_cpp_bin_popen(
            xml_dir,
            bbox,
            scale_factor,
            height,
            width,
            trgt_img,
            xml_string_loading=xml_string_loading)
    else:
        outs, errs, rc = invoke_cpp_bin_popen(
            xml_dir,
            bbox,
            scale_factor,
            height,
            width,
            xml_string_loading=xml_string_loading)
        print('stdout:', outs)
        if outs and rc == 0:
            print('stdout:', outs)
            save_stream_output(outs, 'test_1577_blstream.png')
        else:
            print('stderr:', errs)

        print('returncode:', rc)
