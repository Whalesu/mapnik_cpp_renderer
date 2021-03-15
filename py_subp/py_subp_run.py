import subprocess


def invoke_cpp_bin():
    res = subprocess.run(
        [
            '/Users/jingyusu/selfInterest/mapnik_cpp_renderer/mapnik_renderer',
            '--xml_dir',
            '/Users/jingyusu/Desktop/15770551472ea94b214de0575322228e97188a17.xml',
            '--bbox', '13529488.05499435', '3659472.8978689983',
            '13530101.189715622', '3660085.70353947', '--trgt_img',
            '/Users/jingyusu/Desktop/test_1577_bl.png', '--scale_factor',
            '2.0', '--height', '512', '--width', '512'
        ],
        env={
            'FONTDIR': '/Users/jingyusu/projects/datamap-map/const/fonts',
            'INPUT_PLUGIN_DIR': '/usr/local/lib/mapnik/input'
        },
        capture_output=True)
    print('returncode: ', res.returncode)
    print('stdout:', res.stdout.decode("utf-8"))
    print('stderr:', res.stderr.decode("utf-8"))


if __name__ == '__main__':
    invoke_cpp_bin()
