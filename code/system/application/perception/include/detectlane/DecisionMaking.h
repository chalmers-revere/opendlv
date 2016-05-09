#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Dense>

using namespace std;
using namespace cv;

// Declarations
void SelectClosestLines(Eigen::MatrixXd *pointsPerRegion,Eigen::MatrixXd *regionIndex);

double GetLateralPosition(double K1,double M1, double K2,double M2,double kMid,double mMid,double MAXROW);

void AddMomentum(Eigen::MatrixXd &K, Eigen::MatrixXd &kPrev,
      Eigen::MatrixXd &M,Eigen::MatrixXd &mPrev,
      double alpha,Eigen::MatrixXd &regionIndex);

void SelectLanesV2(Eigen::VectorXd &numberOfPoints,Eigen::VectorXd &laneLocation);

void SelectLaneOrientation(Eigen::MatrixXd &regionIndex,Eigen::VectorXd &laneLocation,int nRegions);

void removeRowDM(Eigen::VectorXd& matrix, unsigned int rowToRemove);


void SelectClosestLines(Eigen::MatrixXd *pointsPerRegion,Eigen::MatrixXd *regionIndex)
{
  int midRegion = (int)(pointsPerRegion->rows()) / 2;
  
  //Left lane
  bool leftLaneSet = 0;
  for (int i=midRegion-1; i>0; i--) {
    if ((pointsPerRegion->col(0)(i) > pointsPerRegion->col(0)(i-1))) {
      regionIndex->col(0)(0) = i;
      leftLaneSet = 1;
      break;
    }
  }
  if (!leftLaneSet)
    regionIndex->col(0)(0)=0;
  
  //Right lane
  bool rightLaneSet = 0;
  for (int i=midRegion;i<pointsPerRegion->rows()-1;i++){
    if ( (pointsPerRegion->col(0)(i) > pointsPerRegion->col(0)(i+1))){
      regionIndex->col(0)(1) = i;
      rightLaneSet = 1;
      break;
    }
  }
  if (!rightLaneSet)
    regionIndex->col(0)(1)=pointsPerRegion->rows()-1;
}

double GetLateralPosition(double K1,double M1, double K2,double M2,double kMid,double mMid,double MAXROW)
{
  double laneWidth = 3.5;
  double centerLane = ((K2 * MAXROW + M2) + (K1 * MAXROW + M1) ) / 2.0;
  double pixelLaneWidth = abs((K2 * MAXROW + M2) - (K1 * MAXROW + M1) );
  double pixelsPerMeter = pixelLaneWidth / laneWidth;
  double offset = (kMid*MAXROW+mMid)-centerLane;
  
  return offset / (pixelsPerMeter);
  
  
}

void AddMomentum(Eigen::MatrixXd &K, Eigen::MatrixXd &kPrev,Eigen::MatrixXd &M,Eigen::MatrixXd &mPrev,double alpha,Eigen::MatrixXd &regionIndex)
{
  for (int i = 0;i<regionIndex.rows();i++)
  {
    K.row(i)(0) = (1-alpha) * K.row(i)(0) + alpha * kPrev.row(i)(0);
    M.row(i)(0) = (1-alpha) * M.row(i)(0) + alpha * mPrev.row(i)(0);
  }
  
}

// TMP SOLUTION
void removeRowDM(Eigen::VectorXd& matrix, unsigned int rowToRemove)
{
  unsigned int numRows = (int)matrix.rows()-1;
  unsigned int numCols = (int)matrix.cols();
  
  if( rowToRemove < numRows )
    matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);
  
  matrix.conservativeResize(numRows,numCols);
}

// TMP SOLUTION

void SelectLanesV2(Eigen::VectorXd &numberOfPoints,Eigen::VectorXd &laneLocation)
{
  int nTracks = 0, leftMidPoint = (int)numberOfPoints.rows()/2-1;
  int pos;
  for (int i = 0; i <numberOfPoints.rows();i++)
  {
    if ((int)numberOfPoints.row(i)(0) != 0)
    {
      if ( i ==leftMidPoint && (int)numberOfPoints.row(i+1)(0) !=0)
      {
        numberOfPoints.segment(i, 2).maxCoeff(&pos);
        laneLocation.row(nTracks)(0)=i+pos;
        i++;
      }
      else
      {
        laneLocation.row(nTracks)(0)=i;
      }
      nTracks++;
    }
  }
  
  if (nTracks < laneLocation.rows())
  {
    for (int i=(int)laneLocation.rows()-1;i>nTracks-1;i--)
    {
      removeRowDM(laneLocation, i);
      
    }
  }
}


void SelectLaneOrientation(Eigen::MatrixXd &regionIndex,Eigen::VectorXd &laneLocation,int nRegions)
{
  int nTracks = (int)laneLocation.rows();
  int nLeftTracks = 0;
  int nRightTracks = 0;
  for ( int i = 0;i<nTracks;i++)
  {
    if (laneLocation.row(i)(0)<=nRegions/2-1)
      nLeftTracks++;
  }
  if (nLeftTracks == 0)
    nLeftTracks++;
  
  nRightTracks = nTracks - nLeftTracks;
  if (nRightTracks == 0)
    nRightTracks++;
  regionIndex.row(0)(0) = laneLocation.row(nLeftTracks-1)(0);
  regionIndex.row(1)(0) = laneLocation.row(nTracks-nRightTracks)(0);
}





