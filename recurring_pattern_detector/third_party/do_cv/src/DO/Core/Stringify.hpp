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

#ifndef DO_CORE_STRINGIFY_HPP
#define DO_CORE_STRINGIFY_HPP

#include <sstream>
#include <string>

namespace DO {

  //! \ingroup Utility
  //! \brief stringifying function.
  template <typename T>
  std::string toString(const T& x)
  {
    std::stringstream oss;
    oss << x;
    return oss.str();
  }

} /* namespace DO */

#endif /* DO_CORE_STRINGIFY_HPP */