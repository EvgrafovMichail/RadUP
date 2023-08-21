import sys
import os

from utils import (
    parse_data,
    create_image,
)


def main() -> None:
    path_to_radar_data = os.path.join('.', f'radar_data{sys.argv[1]}.json')
    path_to_image_folder = os.path.join('..', 'images')
    path_to_image = os.path.join(
        path_to_image_folder, f'image_{sys.argv[-1]}.png'
    )

    if not os.path.exists(path_to_image_folder):
        os.mkdir(path_to_image_folder)

    radar_data = parse_data(path_to_radar_data)
    create_image(radar_data, path_to_image)



if __name__ == '__main__':
    main()
