/**
*Modified for OpenDLV 2018 
*Rewritten from template to class handling ORB features
*/

/**
 * This is a modified version of TemplatedVocabulary.h from DBoW2 (see below).
 * Added functions: Save and Load from text files without using cv::FileStorage.
 * Date: August 2015
 * Raúl Mur-Artal
 */

/**
 * File: TemplatedVocabulary.h
 * Date: February 2011
 * Author: Dorian Galvez-Lopez
 * Description: templated vocabulary 
 * License: see the LICENSE.txt file
 *
 */

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_ORBVOCABULARY_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_ORBVOCABULARY_HPP

#include <cassert>

#include <vector>
#include <numeric>
#include <cfloat>
#include <fstream>
#include <string>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <limits>

#include "orbfeaturevector.hpp"
#include "orbbowvector.hpp"
#include "orbdescriptor.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

class OrbVocabulary
{
public:
    
  /**
   * Creates the vocabulary by loading a file
   * Should be the only constructor we need
   * @param filename
   */
  OrbVocabulary(const std::string &filename);

	~OrbVocabulary();

  bool loadFromTextFile(const std::string &filename);

	double getScore(const OrbBowVector &bowVector1, const OrbBowVector &bowVector2) const;

  int getSize() const;

  void transform4(const std::vector<cv::Mat> features, OrbBowVector &bowVector, OrbFeatureVector &featureVector, int levelsUp) const;
  
private:

  void transform5(const cv::Mat &feature, uint32_t &wordId, double &weight, uint32_t *nodeId, int levelsUp) const;

    /// Tree node
  struct Node 
  {
    /// Node id
    uint32_t id; //NodeId type
    /// Weight if the node is a word
    double weight; //WordValue type
    /// Children 
    std::vector<uint32_t> children;
    /// Parent node (undefined in case of root)
    uint32_t parent;
    /// Node descriptor
    cv::Mat descriptor;

    /// Word id if the node is a word
    uint32_t word_id; //word_id

    /**
     * Empty constructor
     */
    Node(): id(0), weight(0), children(std::vector<uint32_t>()), parent(0), descriptor(cv::Mat()),  word_id(0){}
    
    /**
     * Constructor
     * @param _id node id
     */
    Node(uint32_t a_id): id(a_id), weight(0),children(std::vector<uint32_t>()), parent(0), descriptor(cv::Mat()), word_id(0){}

    /**
     * Returns whether the node is a leaf node
     * @return true iff the node is a leaf
     */
    inline bool isLeaf() const { return children.empty(); }
  };

  /// Branching factor
  int m_k;
  
  /// Depth levels 
  int m_L;
  
  /// Tree nodes
  std::vector<Node> m_nodes;
  
  /// Words of the vocabulary (tree leaves)
  /// this condition holds: m_words[wid]->word_id == wid
  std::vector<Node*> m_words;

};
	
} //namespace sensation
} //namespace logic
} //namespace opendlv

#endif
