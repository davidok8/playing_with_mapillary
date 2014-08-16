import json
import os
import requests
import urllib


BASE_MAPILLARY_URL = 'http://api.mapillary.com/'
BASE_SEQUENCE_URL = 'v1/im/sequence'


def get_image_sequence(sequence_key, geo_json=None):
    """
    Returns the image sequence in a JSON object given a sequence key.

    Parameters
    ----------
    sequence_key: str
        a sequence ID.
    geo_json: bool
        a boolean which if set to True will return a GeoJSON object.

    Returns
    -------
    json_object: JSON object or GeoJSON object.
        a JSON object corresponding to the image sequence.

    """

    url = BASE_MAPILLARY_URL + BASE_SEQUENCE_URL
    params = {'skey': sequence_key}
    if geo_json:
        params['geojson'] = geo_json

    get_content = requests.get(url, params=params).content
    return json.loads(get_content)


def download_image_sequence(sequence_key, output_directory=None):
    """
    Download the image sequence in the format 1024 pixels.
    TODO: it may be worth to have a more generic API but for now it just
    does the job.

    Parameters
    ----------
    sequence_key: str
        a sequence ID.
    geo_json: bool
        a boolean which if set to True will return a GeoJSON object.

    Returns
    -------
    json_object: JSON object or GeoJSON object.
        a JSON object corresponding to the image sequence.

    """

    image_sequences = get_image_sequence(sequence_key)
    image_urls = [
        image['map_image_versions'][2]['url'] for image in image_sequences
    ]

    if not output_directory:
        output_directory = sequence_key
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    for index, image_url in enumerate(image_urls):
        filename = os.path.join(output_directory, str(index) + '.jpg')
        urllib.urlretrieve(image_url, filename)