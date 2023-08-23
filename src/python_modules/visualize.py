try:
    import datetime
    import sys
    import os

    import requests

    from utils import (
        parse_data,
        create_image,
    )


    def main() -> None:
        path_to_radar_data = os.path.join(
            '..', 'tmp', f'~radar_data{sys.argv[1]}.json'
        )

        path_to_image_folder = os.path.join('..', 'images')
        path_to_image = os.path.join(
            path_to_image_folder, f'image_{sys.argv[-1]}.png'
        )

        if not os.path.exists(path_to_image_folder):
            os.mkdir(path_to_image_folder)

        radar_data = parse_data(path_to_radar_data)
        create_image(radar_data, path_to_image)

        request_body = {
            'image_id': sys.argv[-1],
            'path_to_image': path_to_image,
            'timestamp': str(datetime.datetime.now())
        }

        url = "http://127.0.0.1:8000/received_image"
        requests.post(url, json=request_body)


    if __name__ == '__main__':
        main()

except (KeyboardInterrupt, Exception):
    pass
