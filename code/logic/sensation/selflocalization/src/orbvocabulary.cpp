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

#include "orbvocabulary.hpp"

namespace opendlv {
namespace logic {
namespace sensation {


OrbVocabulary::OrbVocabulary(const std::string &filename):
    m_k(0)
  , m_L(0)
  , m_nodes(std::vector<Node>())
  , m_words(std::vector<Node*>())
{
  loadFromTextFile(filename);
}

OrbVocabulary::~OrbVocabulary()
{
}

bool OrbVocabulary::loadFromTextFile(const std::string &filename){
    std::ifstream f;
    f.open(filename.c_str());
	
    if(f.eof())
	return false;

    m_words.clear();
    m_nodes.clear();

    std::string s;
    std::getline(f,s);
    std::stringstream ss;
    ss << s;
    ss >> m_k;
    ss >> m_L;
    int n1, n2;
    ss >> n1;
    ss >> n2;

    if(m_k<0 || m_k>20 || m_L<1 || m_L>10 || n1<0 || n1>5 || n2<0 || n2>3)
    {
        std::cerr << "Vocabulary loading failure: This is not a correctly formated text file!" << std::endl;
	return false;
    }

    // nodes
    int expected_nodes = (int)((pow((double)m_k, (double)m_L + 1) - 1)/(m_k - 1));
    m_nodes.reserve(expected_nodes);

    m_words.reserve(pow((double)m_k, (double)m_L + 1));

    m_nodes.resize(1);
    m_nodes[0].id = 0;
    while(!f.eof())
    {
        std::string snode;
        std::getline(f,snode);
        std::stringstream ssnode;
        ssnode << snode;

        int nid = m_nodes.size();
        m_nodes.resize(m_nodes.size()+1);
	m_nodes[nid].id = nid;
	
        int pid ;
        ssnode >> pid;
        m_nodes[nid].parent = pid;
        m_nodes[pid].children.push_back(nid);

        int nIsLeaf;
        ssnode >> nIsLeaf;

        std::stringstream ssd;
        for(int iD=0;iD<OrbDescriptor::L;iD++)
        {
            std::string sElement;
            ssnode >> sElement;
            ssd << sElement << " ";
	}
        OrbDescriptor::fromString(m_nodes[nid].descriptor, ssd.str());

        ssnode >> m_nodes[nid].weight;

        if(nIsLeaf>0)
        {
            int wid = m_words.size();
            m_words.resize(wid+1);

            m_nodes[nid].word_id = wid;
            m_words[wid] = &m_nodes[nid];
        }
        else
        {
            m_nodes[nid].children.reserve(m_k);
        }
    }

    return true;
}


double OrbVocabulary::getScore(const OrbBowVector &bowVector1, const OrbBowVector &bowVector2) const{

  
  OrbBowVector::const_iterator vector1_it, vector2_it;
  const OrbBowVector::const_iterator vector1_end = bowVector1.end();
  const OrbBowVector::const_iterator vector2_end = bowVector2.end();
  
  vector1_it = bowVector1.begin();
  vector2_it = bowVector1.begin();
  
  double score = 0;
  
  while(vector1_it != vector1_end && vector2_it != vector2_end)
  {
    const double& wordVal1 = vector1_it->second;
    const double& wordVal2 = vector2_it->second;
    
    if(vector1_it->first == vector2_it->first)
    {
      score += fabs(wordVal1 - wordVal2) - fabs(wordVal1) - fabs(wordVal2);
      
      // move v1 and v2 forward
      ++vector1_it;
      ++vector2_it;
    }
    else if(vector1_it->first < vector2_it->first)
    {
      // move v1 forward
      vector1_it = bowVector1.lower_bound(vector2_it->first);
    }
    else
    {
      // move v2 forward
      vector2_it = bowVector2.lower_bound(vector1_it->first);
    }
  }
 
  score = -score/2.0;

  return score;
}

int OrbVocabulary::getSize() const{

  return m_words.size();
}

void OrbVocabulary::transform4(const std::vector<cv::Mat> features, OrbBowVector &bowVector, OrbFeatureVector &featureVector, int levelsUp) const{

  bowVector.clear();
  featureVector.clear();
  
  if(m_words.empty()) // safe for subclasses
  {
    return;
  }
  
  // normalize 
  //LNorm norm;
  //bool must = m_scoring_object->mustNormalize(norm);
  
  std::vector<cv::Mat>::const_iterator feature_it;
 
  uint32_t i_feature = 0;
  for(feature_it = features.begin(); feature_it < features.end(); ++feature_it, ++i_feature)
  {
    uint32_t wordId;
    uint32_t nodeId;
    double w; 
    // w is the idf value if TF_IDF, 1 if TF      
    transform5(*feature_it, wordId, w, &nodeId, levelsUp);
    
    if(w > 0) // not stopped
    { 
      bowVector.addWeight(wordId, w);
      featureVector.addFeature(nodeId, i_feature);
    }
  }
  bowVector.normalize();
}

void OrbVocabulary::transform5(const cv::Mat &feature, uint32_t &wordId, double &weight, uint32_t *nodeId, int levelsUp) const{

  // propagate the feature down the tree
  std::vector<uint32_t> nodes;
  std::vector<uint32_t>::const_iterator node_it;

  // level at which the node must be stored in nid, if given
  const int nodeId_level = m_L - levelsUp;
  if(nodeId_level <= 0 && nodeId != NULL) *nodeId = 0; // root

  uint32_t finalNodeId = 0; // root
  int current_level = 0;

  do
  {
    ++current_level;
    nodes = m_nodes[finalNodeId].children;
    finalNodeId = nodes[0];
 
    double best_d = OrbDescriptor::distance(feature, m_nodes[finalNodeId].descriptor);

    for(node_it = nodes.begin() + 1; node_it != nodes.end(); ++node_it)
    {
      uint32_t id = *node_it;
      double d = OrbDescriptor::distance(feature, m_nodes[id].descriptor);
      if(d < best_d)
      {
        best_d = d;
        finalNodeId = id;
      }
    }
    
    if(nodeId != NULL && current_level == nodeId_level)
      *nodeId = finalNodeId;
    
  } while( !m_nodes[finalNodeId].isLeaf() );

  // turn node id into word id
  wordId = m_nodes[finalNodeId].word_id;
  weight = m_nodes[finalNodeId].weight;
}

} //namespace sensation
} //namespace logic
} //namespace opendlv
