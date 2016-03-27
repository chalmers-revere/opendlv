#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Dense>

using namespace std;
using namespace cv;

// Extract points for the different regions
void ScanImage(cv::Mat *img, Eigen::MatrixXd *lines,Eigen::MatrixXd *recoveredPoints,int nPoints,int minRow, int maxRow);

// Remove unwanted points
void RemoveInvalidPoints(Eigen::MatrixXd *tmpMat, int nPoints);

// Solve RMS problem for points to a line
void LinearSolve(Eigen::MatrixXd *dataPoints, double *k, double *m);

void removeRow(Eigen::MatrixXd& matrix, unsigned int rowToRemove);

void removeRow(Eigen::VectorXd& matrix, unsigned int rowToRemove);

void ExtractLines(Eigen::MatrixXd *RECOVEREDPOINTS,Eigen::MatrixXd *KCOEFF,
                  Eigen::MatrixXd *MCOEFF,long nRegions,int nPoints,
                  Eigen::VectorXd *pointsPerRegion);






// Eigen VERSION
void ScanImage(cv::Mat *img, Eigen::MatrixXd *lines,Eigen::MatrixXd *recoveredPoints,int nPoints,int minRow, int maxRow)
{
  double deltaRow = (maxRow-minRow) / (double)nPoints;
  int currentRow;
  
  Eigen::MatrixXd regionPoints(1,lines->rows()+2);
  for (int i=0; i<nPoints; i++) {
    currentRow = minRow + i*deltaRow;
    
    // Calculate the start and end points for the regions
    GetRegionIntervals(&regionPoints,lines,currentRow,img->cols);
    Mat slice = img->row(currentRow);

    double meanPoint;
    for (int j=0; j<lines->rows()+1; j++) {
      Mat indices,regionSlice;
      if (fabs(regionPoints(0,j) - regionPoints(0,j+1)) > 0.0001)
      {
        regionSlice = slice.colRange(regionPoints.col(j)(0),regionPoints.col(j+1)(0));
        findNonZero(regionSlice,indices);
        
        if (abs(mean(indices)[0]) < 0.1){
          meanPoint= 0;
        } else {
          meanPoint= mean(indices)[0]+regionPoints(0,j);
        }
        
        recoveredPoints->col(j)(i) = (int)meanPoint;
        
      } else
      {
        recoveredPoints->col(j)(i) = 0;
      }
      
      
    }
    recoveredPoints->col(lines->rows()+1)(i) = currentRow;
  }
}

void removeRow(Eigen::MatrixXd& matrix, unsigned int rowToRemove)
{
  unsigned int numRows = (int)matrix.rows()-1;
  unsigned int numCols = (int)matrix.cols();
  
  if( rowToRemove < numRows )
    matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);
  
  matrix.conservativeResize(numRows,numCols);
}

void removeRow(Eigen::VectorXd& matrix, unsigned int rowToRemove)
{
  unsigned int numRows = (int)matrix.rows()-1;
  unsigned int numCols = (int)matrix.cols();
  
  if( rowToRemove < numRows )
    matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);
  
  matrix.conservativeResize(numRows,numCols);
}



void RemoveInvalidPoints(Eigen::MatrixXd *tmpMat, int nPoints)
{
  int counter = 0;
  for (int i=0; i<nPoints; i++) {
    if ((int)tmpMat->col(0)(i-counter) == 0)
    {
      removeRow(*tmpMat,i-counter);
      counter++;
    }
  }
}

// EIGEN VERSION
void LinearSolve(Eigen::MatrixXd *dataPoints, double *k, double *m)
{
  if (dataPoints->rows()>=1)
  {
    // Solve the system COLS = k * ROWS + m
    Eigen::MatrixXd COLS = dataPoints->col(0);
    Eigen::MatrixXd ROWS = Eigen::MatrixXd::Ones(dataPoints->rows(),2);
    ROWS.col(1) = dataPoints->col(1);
    
    Eigen::MatrixXd B = ROWS.householderQr().solve(COLS);
    *k = B(1,0);
    *m = B(0,0);
    
  }
}

// EIGEN VERSION
void ExtractLines(Eigen::MatrixXd *RECOVEREDPOINTS,Eigen::MatrixXd *KCOEFF,
                  Eigen::MatrixXd *MCOEFF,long nRegions,int nPoints,
                  Eigen::VectorXd *pointsPerRegion)
{
  for (int i=0; i<nRegions; i++) {
    Eigen::MatrixXd tmpMat(nPoints,2);
    tmpMat << RECOVEREDPOINTS->col(i),RECOVEREDPOINTS->col(nRegions);
    RemoveInvalidPoints(&tmpMat,nPoints);
    
    if (tmpMat.rows()>0.05*nPoints){
      pointsPerRegion->col(0)(i) = tmpMat.rows();
    } else {
      pointsPerRegion->col(0)(i) = 0;
    }
    double k,m;
    LinearSolve(&tmpMat,&k,&m);
    KCOEFF->row(i)(0) = k;
    MCOEFF->row(i)(0) = m;
    
  }
}

