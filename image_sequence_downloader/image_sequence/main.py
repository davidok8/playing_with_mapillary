import argparse

from utils import download_image_sequence


def parse_command_line_args():
    parser = argparse.ArgumentParser(
        description='Download an image sequence from www.mapillary.com'
    )
    parser.add_argument(
        "sequence_key",
        action='store',
        help="key of the image sequence"
    )
    parser.add_argument(
        "-o", "--output_dir", type=str,
        action='store',
        default=None,
        help="output directory"
    )
    return parser.parse_args()


def main():
    # Process command-line arguments.
    args = parse_command_line_args()

    # Download the image sequence.
    sequence_key = args.sequence_key
    output_dir = args.output_dir
    download_image_sequence(sequence_key, output_dir)


if __name__ == '__main__':
    main()