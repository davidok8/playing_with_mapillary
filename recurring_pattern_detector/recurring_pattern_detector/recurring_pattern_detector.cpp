#include <DO/Core.hpp>
#include <DO/ImageIO.hpp>

#include "visualization.hpp"
#include "GCoptimization.h"


using namespace DO;
using namespace std;


enum Label { STREETVIEW = 0, UNINTERESTING = 1 };
const Rgb8 LABEL_COLORS[2] = { Blue8, Red8 };


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
// Computation of prior histograms.
Image<float, 3> compute_prior_histogram(const Image<Rgb8>& training_image,
                                        const Image<Rgb8>& labels,
                                        Label label, int vq_step = 2)
{
  const int num_bins = 256 / vq_step;
  Image<float, 3> histogram(num_bins, num_bins, num_bins);
  histogram.array().setZero();

  for (int y = 0; y < training_image.height(); ++y)
  {
    for (int x = 0; x < training_image.width(); ++x)
    {
      if (labels(x,y) == LABEL_COLORS[label])
      {
        Vector3i quantized_color = training_image(x,y).cast<int>() / vq_step;
        histogram(quantized_color) += 1.f;
      }
    }
  }
  return histogram;
}


void update_prior_histogram(Image<float, 3>& histogram,
                            const Image<Rgb8>& training_image,
                            const Image<Rgb8>& labels,
                            Label label, int vq_step = 2)
{
  for (int y = 0; y < training_image.height(); ++y)
  {
    for (int x = 0; x < training_image.width(); ++x)
    {
      if (labels(x,y) == LABEL_COLORS[label])
      {
        Vector3i quantized_color = training_image(x,y).cast<int>() / vq_step;
        histogram(quantized_color) += 1.f;
        ++histogram(quantized_color);
      }
    }
  }
}


// ========================================================================== //
// Computation of temporal frequency of each foreground pixel.
void update_streetview_temporal_frequencies(Image<int>& fg_temporal_frequency,
                                            Image<Rgb8>& labels)
{
  for (int y = 0; y < labels.height(); ++y)
    for (int x = 0; x < labels.width(); ++x)
      if(labels(x,y) == LABEL_COLORS[UNINTERESTING])
        ++fg_temporal_frequency(x,y);
}


// ========================================================================== //
// Graph-cut based segmentation.
double weight(Vector3d& a, Vector3d& b,
              double sigma_pixel = 5., double scaling = 200.)
{
  return scaling * exp(- (a-b).squaredNorm() / pow(sigma_pixel, 2) );
}


Image<Rgb8> graph_cut_segmentation(const Image<Rgb8>& image,
                                   const Image<float, 3>& prob_bg,
                                   const Image<float, 3>& prob_fg,
                                   int vq_step,
                                   const Image<float> *fg_temporal_prob = 0,
                                   double eps = 1e-7)
{
  int width = image.width();
  int height = image.height();
  int num_pixels = width*height;

  typedef Graph<int, int, int> GraphType;
  GraphType *g = new GraphType(num_pixels, num_pixels*6);

  for(int i = 0; i < num_pixels; ++i)
    g->add_node();

  // Data term.
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      size_t i = y*width + x;
      Vector3i f = image(x,y).cast<int>() / vq_step;

      // Reminder: add eps to avoid evaluating log(0)
      double weight_fg = -log(prob_fg(f) + eps);
      double weight_bg = -log(prob_bg(f) + eps);

      if (fg_temporal_prob)
      {
        weight_fg += -log(  (*fg_temporal_prob)(x, y) + 0.1);
        weight_bg += -log(1-(*fg_temporal_prob)(x, y) + 0.1);
      }

      g->add_tweights(i, weight_fg, weight_bg);
    }
  }

  // Smoothness term (horizontal edges).
  for (int y = 0; y < height; ++y)
  {
    for (int x = 1; x < width; ++x)
    {
      int i = y*width + x - 1;
      int j = y*width + x;
      Vector3d a = image(x-1, y).cast<double>();
      Vector3d b = image(x, y).cast<double>();
      double w = weight(a, b);
      g->add_edge(i, j, w, w);
    }
  }

  // Smoothness term (vertical edges).
  for (int y = 1; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      int i = (y-1)*width + x;
      int j = y*width + x;
      Vector3d a = image(x, y-1).cast<double>();
      Vector3d b = image(x, y).cast<double>();
      double w = weight(a, b);
      g->add_edge(i, j, w, w);
    }
  }

  // Apply max-flow.
  g->maxflow();

  // Get the labels in each pixel
  Image<Rgb8> labels(width, height);
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      if (g->what_segment(y*width+x) == GraphType::SOURCE)
        labels(x, y) = LABEL_COLORS[STREETVIEW];
      else
        labels(x,y) = LABEL_COLORS[UNINTERESTING];
    }
  }

  // Delete graph cut.
  delete g;

  return labels;
}


// ========================================================================== //
// Main program.
int main()
{
  // ======================================================================== //
  // Load the image sequence.
  const string ALL_DATASET_DIR_PATH = srcPath("datasets");
  const string SEQUENCE_KEY = "16U02a2VZI6P3Wz2z8QPZA";
  const string DATASET_DIR_PATH = ALL_DATASET_DIR_PATH + "/" + SEQUENCE_KEY;
  const int NUM_IMAGES = 55;

  vector<Image<Rgb8> > image_sequence;
  if (!load_dataset(image_sequence, DATASET_DIR_PATH, NUM_IMAGES) ||
      image_sequence.empty())
    return EXIT_FAILURE;

  // ======================================================================== //
  // Load the necessary data for the training step.
  Image<Rgb8> training_image(image_sequence.front());
  Image<Rgb8> training_layer;
  const string training_layer_filepath = DATASET_DIR_PATH + "/" + "layer.png";
  if (!imread(training_layer, training_layer_filepath))
  {
    cerr
      << "Error: cannot load training layer image:\n"
      << training_layer_filepath << endl;
    return EXIT_FAILURE;
  }

  // ======================================================================== //
  // Some variables and objects that will be used afterwards.
  //
  // Color quantization step for the histogram.
  int vq_step = 2;

  // Color-based prior distributions.
  Image<float, 3> hist_uninteresting, hist_streetview;
  Image<float, 3> prob_fg, prob_bg;

  // Temporal consistency prior of each foreground pixel.
  Image<int> streetview_temporal_occ_count(training_image.sizes());
  Image<float> streetview_temporal_freq(training_image.sizes());
  streetview_temporal_occ_count.array().setZero();
  streetview_temporal_freq.array().setZero();

  // Variables and object for visualization purposes.
  Image<Rgb8> labels, result;
  double alpha = 0.5;

  // ======================================================================== //
  // Where to save segmentation results.
  bool save_to_disk = true;
  int save_quality = 85;
  // You need to manually create the folder "results/some_image_sequence_key".
  // Otherwise nothing will be saved.
  string results_folderpath = string(srcPath("results")) + "/" + SEQUENCE_KEY;
  string segmentation_result_filepath;
  string temporal_prior_filepath; 

  // ======================================================================== //
  // Compute the probabilities that a color belongs to some class in
  // { FOREGROUND, BACKGROUND }.
  hist_uninteresting = compute_prior_histogram(training_image, training_layer,
                                               UNINTERESTING, vq_step);
  hist_streetview = compute_prior_histogram(training_image, training_layer,
                                            STREETVIEW, vq_step);
  prob_bg.resize(hist_streetview.sizes());
  prob_fg.resize(hist_uninteresting.sizes());
  prob_fg.array() = hist_uninteresting.array() / hist_uninteresting.array().sum();
  prob_bg.array() = hist_streetview.array() / hist_streetview.array().sum();

  // ======================================================================== //
  // Warm up with the first frame.
  labels = graph_cut_segmentation(image_sequence[0], prob_bg, prob_fg, vq_step);
  result = blend(image_sequence[0], labels, alpha);

  // ======================================================================== //
  // Update the prior distributions.
  //
  // Update the color-based prior histograms.
  hist_uninteresting = compute_prior_histogram(training_image, training_layer,
                                               UNINTERESTING, vq_step);
  hist_streetview = compute_prior_histogram(training_image, training_layer,
                                            STREETVIEW, vq_step);

  prob_fg.array() = hist_uninteresting.array() / hist_uninteresting.array().sum();
  prob_bg.array() = hist_streetview.array() / hist_streetview.array().sum();

  // Update the temporal frequency of each foreground pixel.
  update_streetview_temporal_frequencies(streetview_temporal_occ_count, labels);
  streetview_temporal_freq.array() = 
    streetview_temporal_occ_count.array().cast<float>();

  if (save_to_disk)
  {
    segmentation_result_filepath = results_folderpath + "/result_" + toString(0) + ".jpg";
    temporal_prior_filepath = results_folderpath + "/temporal_" + toString(0) + ".jpg"; 
    saveJpeg(result, segmentation_result_filepath, save_quality);
    saveJpeg(streetview_temporal_freq.convert<Rgb8>(), temporal_prior_filepath,
             save_quality);
  }
  

  // ======================================================================== //
  // Process the next frames.
  Timer timer_for_sequence;
  Timer timer_for_frame;
  timer_for_sequence.restart();
  for (unsigned i = 1; i < image_sequence.size(); ++i)
  {
    // ===================================================================== //
    // Start timing the computation time of each segmentation.
    timer_for_frame.restart();

    // Wait for 5 frames before using the temporal consistency.
    // The number 5 is arbitrary.
    Image<float> *ptr_fg_temporal_freq = 0;
    if (i > 5)
      ptr_fg_temporal_freq = &streetview_temporal_freq;


    // ===================================================================== //
    // Segment the frame $i$ using prior knowledge accumulated from frame
    // $0$ to frame. $i-1$.
    labels = graph_cut_segmentation(image_sequence[i], prob_bg, prob_fg, vq_step,
                                    ptr_fg_temporal_freq);
    result = blend(image_sequence[i], labels, alpha);


    // ===================================================================== //
    // Incorporate the segmentation results as a new prior knowledge.
    //
    // Update the color-based prior histograms.
    update_prior_histogram(hist_uninteresting, training_image, training_layer,
                           UNINTERESTING, vq_step);
    update_prior_histogram(hist_streetview, training_image, training_layer,
                           STREETVIEW, vq_step);
    prob_fg.array() = hist_uninteresting.array() / hist_uninteresting.array().sum();
    prob_bg.array() = hist_streetview.array() / hist_streetview.array().sum();

    // Update the temporal frequency of each foreground pixel.
    update_streetview_temporal_frequencies(streetview_temporal_occ_count, labels);
    streetview_temporal_freq.array() = 
      streetview_temporal_occ_count.array().cast<float>() / (i+1.f+1e-2f);

    // Print total computation timer (graph-cut + prior update).
    cout << "[" << i << "]\t Segmentation:\t" << timer_for_frame.elapsed() << " s" << endl;


    // ===================================================================== //
    // Save the results to disk.
    if (save_to_disk)
    {
      timer_for_frame.restart();
      segmentation_result_filepath = results_folderpath + "/result_" + toString(i) + ".jpg";
      temporal_prior_filepath = results_folderpath + "/temporal_" + toString(i) + ".jpg";
      saveJpeg(result, segmentation_result_filepath, save_quality);
      saveJpeg(streetview_temporal_freq.convert<Rgb8>(), temporal_prior_filepath, save_quality);
      cout << "[" << i << "]\t Save to disk:\t" << timer_for_frame.elapsed() << " s" << endl;
    }
  }
  
  cout << "Total processing time: " << timer_for_sequence.elapsed() << " s" << endl;

  return EXIT_SUCCESS;
}
