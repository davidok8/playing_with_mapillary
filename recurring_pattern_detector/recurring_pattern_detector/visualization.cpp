#include "visualization.hpp"


namespace DO {

  Image<Rgb8> blend(const Image<Rgb8>& src_1, const Image<Rgb8>& src_2,
                    double alpha)
  {
    Image<Rgb8> out(src_1.sizes());

    Image<Rgb8>::const_iterator in_pix_1 = src_1.begin();
    Image<Rgb8>::const_iterator in_pix_2 = src_2.begin();
    Image<Rgb8>::iterator out_pix = out.begin();

    // Loop.
    while (out_pix != out.end())
    {
      Rgb64f in_pix_1_d;
      convertColor(in_pix_1_d, *in_pix_1);

      Rgb64f in_pix_2_d;
      convertColor(in_pix_2_d, *in_pix_2);

      Rgb64f out_pix_d;
      out_pix_d.matrix() =     alpha  * in_pix_1_d.matrix()
                         + (1.-alpha) * in_pix_2_d.matrix();
      convertColor(*out_pix, out_pix_d);

      ++in_pix_1;
      ++in_pix_2;
      ++out_pix;
    }

    return out;
  }

  Image<Rgb8> blend(const Image<double>& src_1, const Image<Rgb8>& src_2)
  {
    Image<Rgb8> out(src_1.sizes());

    Image<double>::const_iterator in_pix_1 = src_1.begin();
    Image<Rgb8>::const_iterator in_pix_2 = src_2.begin();
    Image<Rgb8>::iterator out_pix = out.begin();

    // Loop.
    while (out_pix != out.end())
    {
      double in_pix_1_d = (1 - *in_pix_1);

      Rgb64f in_pix_2_d;
      convertColor(in_pix_2_d, *in_pix_2);

      Rgb64f out_pix_d;
      out_pix_d.matrix() = in_pix_1_d*in_pix_2_d.matrix();
      convertColor(*out_pix, out_pix_d);

      ++in_pix_1;
      ++in_pix_2;
      ++out_pix;
    }

    return out;
  }

}
