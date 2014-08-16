#! /usr/bin/env python
from setuptools import setup


with open('README.md') as f:
    long_description = f.read()

setup(name='Mapillary Image Sequence Downloader',
      version='0.1',
      author='David Ok',
      description='A command-line downloader of image sequences from Mapillary',
      long_description=long_description,
      author_email='david.ok8@gmail.com',
      py_modules=['image_sequence'],  # debug this...
      entry_points={
          'console_scripts': [
              'image_seq_downloader_app = image_sequence.main:main',
          ]
      }
)