from typing import Any

from fastapi import FastAPI


plug = FastAPI()


@plug.post('/received_image')
async def post_received_image(
    request_body: dict[str, Any]
):
    print(
        f'radar image info: {request_body["radarImage"]};',
        f'round view file info: {request_body["roundViewFile"]};',
        f'radio tact id: {request_body["radioTactId"]}',
        sep='\n'
    )
