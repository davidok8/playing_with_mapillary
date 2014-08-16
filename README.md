Playing With Mapillary
======================

Contents
--------
The repository `playing_with_mapillary` contains 3 folders:

1. [image_sequence_downloader](https://github.com/davidok8/playing_with_mapillary/tree/master/image_sequence_downloader): which contains the Python program used to download the image sequences.
2. [recurring_pattern_detector](https://github.com/davidok8/playing_with_mapillary/tree/master/recurring_pattern_detector): which contains the C++ program to segment the image.
3. [report](https://github.com/davidok8/playing_with_mapillary/tree/master/report): which contains the LaTeX report.

Data and results are stored as follows:

- Datasets are in: [/recurring_pattern_detector/recurring_pattern_detector/datasets](https://github.com/davidok8/playing_with_mapillary/tree/master/recurring_pattern_detector/recurring_pattern_detector/datasets).
- Results are in: [/recurring_pattern_detector/recurring_pattern_detector/results](https://github.com/davidok8/playing_with_mapillary/tree/master/recurring_pattern_detector/recurring_pattern_detector/results).


Build the program
-----------------

### Continuous Integration Status

[![Build
Status](https://travis-ci.org/davidok8/playing_with_mapillary.svg?branch=master)](davidok8/playing_with_mapillary)

Click on the Travis CI badge and scroll to the end of the build history. Once the program is build, we run the program and it shows the computation time to segment each frame of some image sequence.


To build the program, you will need CMake (http://www.cmake.org). We describe it for Linux and Mac OS X.

### Linux

In ubuntu, type the following commands:

```
  # Install cmake
  sudo apt-get install cmake

  # Assuming we are at the root directory of the repository: `playing_with_mapillary`.
  cd recurring_pattern_detector
  mkdir build
  cd build

  # We are the directory: `playing_with_mapillary/recurring_pattern_detector/build/`.
  cmake -DCMAKE_BUILD_TYPE=Release ..
  make
```

The compiled program is `recurring_pattern_detector` and is located in folder: `/recurring_pattern_detector/build/recurring_pattern_detector`.

Since we are at `recurring_pattern_detector/build/`. Run the program:

```
  $ ./recurring_pattern_detector/recurring_pattern_detector
```


### Mac OS X

It is convenient to use the GUI version of CMake from (http://www.cmake.org) and XCode.

In CMake GUI app:

- `where is the source code:` put: `playing_with_mapillary/recurring_pattern_detector/CMakeLists.txt`
- `where to build the binaries:` put: `playing_with_mapillary/recurring_pattern_detector/build`
- Generate an XCode project.
- In XCode, select the program `recurring_pattern_detector` and edit the scheme so that it builds in `Release` mode.
- Run the program

Open the XCode project


What does the program do?
-------------------------

When the program `recurring_pattern_detector` is run:

1. it segments the image sequence [16U02a2VZI6P3Wz2z8QPZA](https://github.com/davidok8/playing_with_mapillary/tree/master/recurring_pattern_detector/recurring_pattern_detector/datasets/16U02a2VZI6P3Wz2z8QPZA)).

2. it stores the segmentation results in [16U02a2VZI6P3Wz2z8QPZA](https://github.com/davidok8/playing_with_mapillary/tree/master/recurring_pattern_detector/recurring_pattern_detector/results/16U02a2VZI6P3Wz2z8QPZA)).

If you encounter any trouble, click on the build status to examine the log: [![Build
Status](https://travis-ci.org/davidok8/playing_with_mapillary.svg?branch=master)](davidok8/playing_with_mapillary)

Read the code for details: [recurring_pattern_detector.cpp](https://github.com/davidok8/playing_with_mapillary/blob/master/recurring_pattern_detector/recurring_pattern_detector/recurring_pattern_detector.cpp)

(The way the project is laid out is not very convenient in Linux but in constrast, in Windows, it is very practical. Sorry.)

