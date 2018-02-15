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
#include "orbextractor.hpp"


namespace opendlv {
namespace logic {
namespace sensation {
OrbExtractor::OrbExtractor(int nFeatures, float scaleFactor, int nLevels, int initialFastTh, int minFastTh):
		m_vImagePyramid()
	, m_vPattern()
	,	m_nFeatures(nFeatures)
	, m_scaleFactor(scaleFactor)
	, m_nLevels(nLevels)
	, m_initialFastTh(initialFastTh)
	, m_minFastTh(minFastTh)
	, m_nFeaturesPerLevel()
	, m_umax()
	, m_vScaleFactor()
	, m_vInvScaleFactor()
	, m_vLevelSigma2()
	, m_vInvLevelSigma2()
	{
		m_vScaleFactor.resize(nLevels);
    m_vLevelSigma2.resize(nLevels);
    m_vScaleFactor[0]=1.0f;
    m_vLevelSigma2[0]=1.0f;
    for(int i=1; i<nLevels; i++)
    {
        m_vScaleFactor[i]=m_vScaleFactor[i-1]*scaleFactor;
        m_vLevelSigma2[i]=m_vScaleFactor[i]*m_vScaleFactor[i];
    }

    m_vInvScaleFactor.resize(nLevels);
    m_vInvLevelSigma2.resize(nLevels);
    for(int i=0; i<nLevels; i++)
    {
        m_vInvScaleFactor[i]=1.0f/m_vScaleFactor[i];
        m_vInvLevelSigma2[i]=1.0f/m_vLevelSigma2[i];
    }

    m_vImagePyramid.resize(nLevels);

    m_nFeaturesPerLevel.resize(nLevels);
    float factor = 1.0f / scaleFactor;
    float nDesiredFeaturesPerScale = nFeatures*(1 - factor)/(1 - (float)pow((double)factor, (double)nLevels));

    int sumFeatures = 0;
    for( int level = 0; level < nLevels-1; level++ )
    {
        m_nFeaturesPerLevel[level] = cvRound(nDesiredFeaturesPerScale);
        sumFeatures += m_nFeaturesPerLevel[level];
        nDesiredFeaturesPerScale *= factor;
    }
    m_nFeaturesPerLevel[nLevels-1] = std::max(nFeatures - sumFeatures, 0);

    //const int npoints = 512;
    //const Point* pattern0 = (const Point*)bit_pattern_31_;
    //std::copy(pattern0, pattern0 + npoints, std::back_inserter(pattern));

    //This is for orientation
    // pre-compute the end of a row in a circular patch
    /*m_umax.resize(HALF_PATCH_SIZE + 1);

    int v, v0, vmax = cvFloor(HALF_PATCH_SIZE * sqrt(2.f) / 2 + 1);
    int vmin = cvCeil(HALF_PATCH_SIZE * sqrt(2.f) / 2);
    const double hp2 = HALF_PATCH_SIZE*HALF_PATCH_SIZE;
    for (v = 0; v <= vmax; ++v)
        m_umax[v] = cvRound(sqrt(hp2 - v * v));

    // Make sure we are symmetric
    for (v = HALF_PATCH_SIZE, v0 = 0; v >= vmin; --v)
    {
        while (m_umax[v0] == m_umax[v0 + 1])
            ++v0;
        m_umax[v] = v0;
        ++v0;
		}*/
	}
OrbExtractor::~OrbExtractor()
{
}
} // namespace sensation
} // namespace logic
} // namespace opendlv
