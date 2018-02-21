/**
 * Copyright (C) 2017 Chalmers Revere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */
/*Maybe needed
class ExtractorNode
{
public:
    ExtractorNode():bNoMore(false){}

    void DivideNode(ExtractorNode &n1, ExtractorNode &n2, ExtractorNode &n3, ExtractorNode &n4);

    std::vector<cv::KeyPoint> vKeys;
    cv::Point2i UL, UR, BL, BR;
    std::list<ExtractorNode>::iterator lit;
    bool bNoMore;
};*/

#ifndef OPENDLV_ORBEXTRACTOR_HPP
#define OPENDLV_ORBEXTRACTOR_HPP

#include <vector>
#include <list>
#include <iostream>
//#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace opendlv {
namespace logic {
namespace sensation {

class ExtractorNode
{
public:
    //ExtractorNode():bNoMore(false){}
    ExtractorNode();
    ~ExtractorNode();
    void DivideNode(ExtractorNode &n1, ExtractorNode &n2, ExtractorNode &n3, ExtractorNode &n4);

    std::vector<cv::KeyPoint> vKeys;
    cv::Point2i UL, UR, BL, BR;
    std::list<ExtractorNode>::iterator nodeIterator;
    bool bNoMore;
};

class OrbExtractor {
 public:
  OrbExtractor(int nFeatures, float scaleFactor, int nLevels, int initialFastTh, int minFastTh);
  ~OrbExtractor();
  /*Called by other classes to get keypoints and descriptors*/
  void ExtractFeatures(cv::InputArray image,std::vector<cv::KeyPoint>& keypoints,cv::OutputArray descriptors);

  std::vector<cv::Mat> m_vImagePyramid;
 private:
   //float IC_Angle(const cv::Mat& img, cv::Point2f point,  const std::vector<int> & u_max);
   //void computeOrbDescriptor(const cv::KeyPoint& kpt, const cv::Mat& img, const cv::Point* pattern, uchar* desc);
   void computeOrientation(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, const std::vector<int>& umax);
   //void computeDescriptors(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors, const std::vector<cv::Point>& pattern);
   void ComputePyramid(cv::Mat image);
   
   void ComputeKeyPointsOctTree(std::vector<std::vector<cv::KeyPoint> >& allKeypoints);    
   
   std::vector<cv::KeyPoint> DistributeOctTree(const std::vector<cv::KeyPoint>& vToDistributeKeys, const int &minX,
                                           const int &maxX, const int &minY, const int &maxY, const int &nFeatures);

   std::vector<cv::Point> m_vPattern;

   int m_nFeatures;
   double m_scaleFactor;
   int m_nLevels;
   int m_initialFastTh;
   int m_minFastTh;

   std::vector<int> m_nFeaturesPerLevel;
   std::vector<int> m_umax;

   std::vector<float> m_vScaleFactor;
   std::vector<float> m_vInvScaleFactor;    
   std::vector<float> m_vLevelSigma2;
   std::vector<float> m_vInvLevelSigma2;
};

} // namespace sensation
} // namespace logic
} // namespace opendlv

#endif //OPENDLV_ORBEXTRACTOR_HPP
