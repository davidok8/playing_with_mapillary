#pragma once

#include <DO/Core.hpp>

namespace DO {

  Image<Rgb8> blend(const Image<Rgb8>& src_1, const Image<Rgb8>& src_2, double alpha);

  Image<Rgb8> blend(const Image<double>& src_1, const Image<Rgb8>& src_2);

}