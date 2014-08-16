// ========================================================================== //
// This file is part of DO++, a basic set of libraries in C++ for computer 
// vision.
//
// Copyright (C) 2013 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public 
// License v. 2.0. If a copy of the MPL was not distributed with this file, 
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

//! @file
//! \brief Master header file of the ImageProcessing module.

#ifndef DO_IMAGEPROCESSING_HPP
#define DO_IMAGEPROCESSING_HPP

#ifdef _OPENMP
# include <omp.h>
#endif
#include <DO/Core.hpp>

#include <vector>
#include <exception>

// Basic image processing functions.
#include "ImageProcessing/LinearFiltering.hpp"
#include "ImageProcessing/Deriche.hpp"
// Gradient, Laplacian, Hessian matrix, norm, orientation, second moment matrix.
#include "ImageProcessing/Differential.hpp"
#include "ImageProcessing/Determinant.hpp"
#include "ImageProcessing/Norm.hpp"
#include "ImageProcessing/Orientation.hpp"
#include "ImageProcessing/SecondMomentMatrix.hpp"
// Interpolation (bilinear, trilinear)
#include "ImageProcessing/Interpolation.hpp"
// Reduce, enlarge, downscale, upscale,
#include "ImageProcessing/Scaling.hpp"
#include "ImageProcessing/Warp.hpp"
// Data structures and functions for feature detections.
#include "ImageProcessing/ImagePyramid.hpp"
#include "ImageProcessing/Extrema.hpp"
#include "ImageProcessing/DoG.hpp"

/*!
  \defgroup ImageProcessing Image Processing
  \brief The Image Processing module is a header-only library. 
  It covers basic image processing features such as:
  - Linear filtering (both separable and non-separable)
  - Gaussian blurring
  - Deriche IIR filters
  - Image enlarging/reducing functions
  - Interpolation
  - Differential calculus (gradient, Hessian matrix, divergence, Laplacian)
    based on central finite differentiation.
  - Second-moment matrix
  - Image Pyramid data structure
  - Difference of Gaussians computation
  - Local extremum localization, including localization in scale-space.
 */


#endif /* DO_IMAGEPROCESSING_HPP */