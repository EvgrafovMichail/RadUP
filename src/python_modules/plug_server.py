from typing import Any

from fastapi import FastAPI


plug = FastAPI()


@plug.post('/received_image')
async def post_received_image(
    request_body: dict[str, Any]
):
    print(
        f'image id: {request_body["image_id"]};',
        f'path to image: {request_body["path_to_image"]};',
        f'timestamp: {request_body["timestamp"]}',
        sep='\n'
    )
