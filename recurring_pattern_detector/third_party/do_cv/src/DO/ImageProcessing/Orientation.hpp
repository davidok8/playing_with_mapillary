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

#ifndef DO_IMAGEPROCESSING_ORIENTATION_HPP
#define DO_IMAGEPROCESSING_ORIENTATION_HPP

namespace DO {

  /*!
    \ingroup Differential
    @{
   */

  /*!
    \brief Computes an orientation field from a 2D vector field.
    @param[in] 
      src
      an image in which each element **src(x,y)** are 2D vectors, i.e., a discretized 
      2D vector field, e.g. image gradients.
    @param[in,out]
      dst
      an image where each pixel **dst(x,y)** contains the orientation of the 2D 
      vector 'src(x,y)'
   */
  template <typename T>
  void orientation(Image<T>& dst, const Image<Matrix<T,2,1> >& src)
  {
    if (dst.sizes() != src.sizes())
      dst.resize(src.sizes());

    typedef typename Image<Matrix<T,2,1> >::const_iterator InputIterator;
    typedef typename Image<T>::iterator OutputIterator;

    InputIterator src_it(src.begin());
    InputIterator src_it_end(src.end());
    OutputIterator dst_it(dst.begin());
    for ( ; src_it != src_it_end; ++src_it, ++dst_it)
      *dst_it = std::atan2(src_it->y(), src_it->x());
  }
  /*!
    \brief Computes an orientation field from a 2D vector field.
    @param[in] 
      src
      an image in which each element **src(x,y)** are 2D vectors, i.e., a discretized 
      2D vector field, e.g. image gradients.
    @param[out]
      dst
      an image where each pixel **dst(x,y)** contains the orientation of the 2D 
      vector **src(x,y)**
   */
  template <typename T>
  Image<T> orientation(const Image<Matrix<T,2,1> >& src)
  {
    Image<T> ori;
    orientation(ori, src);
    return ori;
  }

  //! \brief Helper class to use Image<T,N>::compute<Orientation>()
  template <typename T, int N> struct Orientation;
  //! \brief Helper class to use Image<T,N>::compute<Orientation>()
  template <typename T> struct Orientation<T,2>
  {
    typedef typename T::Scalar Scalar;
    typedef Image<Scalar> ReturnType;
    Orientation(const Image<T>& src)
      : src_(src) {}
    Image<Scalar> operator()() const
    { return orientation(src_); }
    const Image<T>& src_;
  };

  //! @}

} /* namespace DO */

#endif /* DO_IMAGEPROCESSING_ORIENTATION_HPP */