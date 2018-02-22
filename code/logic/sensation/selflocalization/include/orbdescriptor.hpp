/**
 * File: FORB.h
 * Date: June 2012
 * Author: Dorian Galvez-Lopez
 * Description: functions for ORB descriptors
 * License: see the LICENSE.txt file
 *
 */

#ifndef __D_T_F_ORB__
#define __D_T_F_ORB__

#include <opencv2/core/core.hpp>
#include <vector>
#include <string>

namespace opendlv {
namespace logic {
namespace sensation {


/// Functions to manipulate ORB descriptors
class OrbDescriptor
{
public:

  /// Descriptor length (in bytes)
  static const int L;

  /**
   * Calculates the mean value of a set of descriptors
   * @param descriptors
   * @param mean mean descriptor
   */
  static void meanValue(const std::vector<cv::Mat*> &descriptors,
    cv::Mat &mean);

  /**
   * Calculates the distance between two descriptors
   * @param a
   * @param b
   * @return distance
   */
  static int distance(const cv::Mat &a, const cv::Mat &b);

  /**
   * Returns a string version of the descriptor
   * @param a descriptor
   * @return string version
   */
  static std::string toString(const cv::Mat &a);

  /**
   * Returns a descriptor from a string
   * @param a descriptor
   * @param s string version
   */
  static void fromString(cv::Mat &a, const std::string &s);

  /**
   * Returns a mat with the descriptors in float format
   * @param descriptors
   * @param mat (out) NxL 32F matrix
   */
  static void toMat32F(const std::vector<cv::Mat> &descriptors,
    cv::Mat &mat);

  static void toMat8U(const std::vector<cv::Mat> &descriptors,
    cv::Mat &mat);

};

} // namespace sensation
} // namespace logic
} // namespace opendlv
#endif

