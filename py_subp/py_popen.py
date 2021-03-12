from subprocess import PIPE
from subprocess import Popen
from subprocess import TimeoutExpired

TIME_LIMIT = 5
EXEC_BIN = '/Users/jingyusu/selfInterest/mapnik_cpp_renderer/mapnik_renderer'


def invoke_cpp_bin_popen(xml_dir,
                         bbox,
                         trgt_img,
                         scale_factor=1.0,
                         height=256,
                         width=256):
    args = [
        EXEC_BIN, '--xml_dir', xml_dir, '--bbox',
        *[f'{coord}' for coord in bbox], '--trgt_img', trgt_img,
        '--scale_factor',
        str(scale_factor), '--height',
        str(height), '--width',
        str(width)
    ]

    env = {
        'FONTDIR': '/Users/jingyusu/projects/datamap-map/const/fonts',
        'INPUT_PLUGIN_DIR': '/usr/local/lib/mapnik/input'
    }
    outs = ''
    errs = ''
    rc = 0
    with Popen(args, env=env, stdout=PIPE, stderr=PIPE,
               encoding='utf8') as proc:
        try:
            outs, errs = proc.communicate(timeout=TIME_LIMIT)
            rc = proc.returncode
        except TimeoutExpired as err:
            proc.kill()
            errs = str(err)
            print("progress time out: ", errs)
    print('stdout:', outs)
    print('stderr:', errs)
    print('returncode:', rc)


if __name__ == '__main__':
    xml_dir = '/Users/jingyusu/Desktop/15770551472ea94b214de0575322228e97188a17.xml'
    bbox = [
        13529488.05499435, 3659472.8978689983, 13530101.189715622,
        3660085.70353947
    ]
    trgt_img = '/Users/jingyusu/Desktop/test_1577_bl.png'
    scale_factor = 2.0
    height = 512
    width = 512
    invoke_cpp_bin_popen(xml_dir, bbox, trgt_img, scale_factor, height, width)
