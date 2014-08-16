from unittest import TestCase

from ..utils import get_image_sequence, download_image_sequence


class TestUtils(TestCase):

    def setUp(self):
        self.sequence_id = 'zp_ni0km3m9vjI1jSSQuYA'

    def test_get_image_sequence(self):
        result = get_image_sequence(self.sequence_id)
        self.assertIsNotNone(result)

    def test_download_image_sequence(self):
        download_image_sequence(self.sequence_id)

        # TODO.