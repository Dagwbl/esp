import requests
import json

def request_api(**kwargs):
    response = requests.request(**kwargs)
    # print(type(response.json()))
    # print(response.json())
    response = response.json()
    # print(json.JSONDecoder(response.json()))
    print(response["rows"][1][1])


if __name__ == '__main__':
    # print("Starting request\n")
    request_body = {
        "method": "get",
        "url": "http://127.0.0.1/esp32/api/index.php",
        "data": {
            "model": "DHT11",
            "sensor_id": "4",
        },
        # TODO verify switch information and quality.
    }
    request_api(**request_body)

