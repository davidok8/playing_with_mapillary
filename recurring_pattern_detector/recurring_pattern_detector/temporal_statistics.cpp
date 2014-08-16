#include <stdexcept>

#include <DO/Core.hpp>
#include <DO/ImageIO.hpp>
#include <DO/ImageProcessing.hpp>

#include "visualization.hpp"
#include "GCoptimization.h"


using namespace DO;
using namespace std;


// ========================================================================== //
// Load dataset.
template <typename Color>
bool load_dataset(vector<Image<Color> >& frames, const string& folder,
                  const int num_images, bool verbose = false)
{
  if (!frames.empty())
    frames.clear();
  frames.resize(num_images);
  for (unsigned i = 0; i < frames.size(); ++i)
  {
    string filepath = folder + "/" + toString(i) + ".jpg";

    if (!imread(frames[i], filepath))
    {
      cerr << "Error: cannot load image:\n" << filepath << endl;
      return false; 
    }
    if (verbose)
      cout << "Loaded image:\n" << filepath << endl;
  }
  return true;
}


// ========================================================================== //
// Utility function to compute image statistics.
Image<Rgb64f> mean(const vector<Image<Rgb64f> >& frames)
{
  if (frames.empty())
    std::runtime_error("Error: list of frames is empty.");

  Image<Rgb64f> mean_image(frames.front().sizes());

  for (int i = 0; i < mean_image.array().size(); ++i)
    mean_image.array()[i] = Vector3d::Zero();

  for (unsigned i = 0; i < frames.size(); ++i)
    mean_image.array() += frames[i].array();

  for (int i = 0; i < mean_image.array().size(); ++i)
    mean_image.array()[i] /= frames.size(); 

  return mean_image;
}

Image<Matrix3d> squared_mean(const vector<Image<Rgb64f> >& frames)
{
  Image<Rgb64f> mean_image(mean(frames));
  Image<Matrix3d> squared_mean_image(mean_image.sizes());
  for (int i = 0; i != mean_image.array().size(); ++i)
  {
    const Vector3d& mean_vector = mean_image.array()[i];
    squared_mean_image.array()[i] = mean_vector*mean_vector.transpose();
  }
  return squared_mean_image;
}

Image<Matrix3d> second_moment(const vector<Image<Rgb64f> >& frames)
{
  if (frames.empty())
    std::runtime_error("Error: list of frames is empty.");

  Image<Matrix3d> second_moment_image(frames.front().sizes());
  for (int i = 0; i < second_moment_image.array().size(); ++i)
    second_moment_image.array()[i] = Matrix3d::Zero();

  for (unsigned i = 0; i != frames.size(); ++i)
  {
    const Image<Rgb64f>& frame = frames[i];
    for (int i = 0; i < frame.array().size(); ++i)
    {
      Vector3d f_i= frame.array()[i];
      Matrix3d second_moment = f_i*f_i.transpose();
      second_moment_image.array()[i] += second_moment;
    }
  }

  for (int i = 0; i < second_moment_image.array().size(); ++i)
    second_moment_image.array()[i] /= frames.size(); 

  return second_moment_image;
}

Image<Matrix3d> covariance(const vector<Image<Rgb64f> >& frames)
{
  Image<Matrix3d> squared_mean_image(squared_mean(frames));
  Image<Matrix3d> moment_image(second_moment(frames));

  Image<Matrix3d> covariance_image(moment_image.sizes());
  covariance_image.array() = moment_image.array() - squared_mean_image.array();
  return covariance_image;
}


// ========================================================================== //
// Main.
int main()
{
  const string ALL_DATASET_DIR_PATH = srcPath("datasets");
  const string SEQUENCE_KEY = "-ywLysPFu3TL85zOMOYJkw";
  const string DATASET_DIR_PATH = ALL_DATASET_DIR_PATH + "/" + SEQUENCE_KEY;
  const int NUM_IMAGES = 183;

  // Load the image frames.
  vector<Image<Rgb64f> > image_sequence;
  if (!load_dataset(image_sequence, DATASET_DIR_PATH, NUM_IMAGES) ||
      image_sequence.empty())
    return EXIT_FAILURE;

  // Temporal mean image.
  Image<Rgb64f> mean_image = mean(image_sequence);

  // Temporal covariance image.
  Image<Matrix3d> cov_image = covariance(image_sequence);

  // Temporal standard deviation image.
  Image<double> std_dev_image(cov_image.sizes());
  std_dev_image.array() = cov_image.compute<SquaredNorm>().array().sqrt();

  const string RESULTS_DIR_PATH = srcPath("results");
  const string OUT_DIR_PATH = RESULTS_DIR_PATH + "/" + SEQUENCE_KEY;

  saveJpeg(mean_image.convert<Rgb8>(),
           OUT_DIR_PATH + "/" + "temporal_mean_image.jpg");
  saveJpeg(std_dev_image.compute<ColorRescale>().convert<Rgb8>(),
           OUT_DIR_PATH + "/" + "temporal_std_dev_image.jpg");

  return EXIT_SUCCESS;
}
