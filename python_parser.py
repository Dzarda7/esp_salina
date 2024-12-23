import requests
import json

def get_json_from_url(url):
    try:
        response = requests.get(url)
        response.raise_for_status()
        data = response.json()
        return data
    except requests.exceptions.RequestException as e:
        print(f"Error fetching data from {url}: {e}")
    except json.JSONDecodeError as e:
        print(f"Error parsing JSON data: {e}")

def get_departures_from_stop(stop_name):
    url = f"https://www.idsjmk.cz/api/departures/busstop-by-name?busStopName={stop_name}"
    data = get_json_from_url(url)
    # calculate how many characters are in data dictionary
    print(len(str(data)))

    one_way = []
    for departure in data["stops"][0]["signs"][0]["departures"]:
        one_way.append((departure["link"], departure["destinationStop"], departure["time"]))

    other_way = []
    for departure in data["stops"][0]["signs"][1]["departures"]:
        other_way.append((departure["link"], departure["destinationStop"], departure["time"]))

    return one_way, other_way

if __name__ == "__main__":
    one_way, other_way = get_departures_from_stop("Kartouzská")

    print("Number    Destination    Time")
    for i in range(len(one_way)):
        print(f"{one_way[i][0]}    {one_way[i][1]}    {one_way[i][2]}")

    print("Number    Destination    Time")
    for i in range(len(other_way)):
        print(f"{other_way[i][0]}    {other_way[i][1]}    {other_way[i][2]}")