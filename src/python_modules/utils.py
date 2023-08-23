import json

import numpy as np
import cv2 as cv


def parse_data(path_to_radar_data: str) -> np.ndarray:

    with open(path_to_radar_data, 'r') as file:
        radar_data = json.load(file)
        radar_data = np.array(radar_data, dtype=np.uint8)

    return radar_data[:, :1000]


def create_image(radar_data: np.ndarray, path_to_image: str) -> None:
    report_amount = 3600
    report_size = 1000

    image = np.zeros(shape=(2 * report_size, 2 * report_size))

    angles = np.linspace(0, 2 * np.pi, report_amount)
    distances = np.arange(report_size)

    distances_grid, angles_grid = np.meshgrid(distances, angles)

    index_row = np.floor(
        distances_grid * np.sin(angles_grid) + report_size
    ).astype(int)
    index_col = np.floor(
        distances_grid * np.cos(angles_grid) + report_size
    ).astype(int)

    image[index_row.flatten(), index_col.flatten()] = radar_data.flatten()

    circle_mask = cv.circle(
        image.copy(), (report_size - 1, report_size - 1),
        report_size - 1, 255, -1
    )

    image = np.ones_like(circle_mask) * 255 - circle_mask + image
    cv.imwrite(path_to_image, image)
