#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Dense>
using namespace cv;

// Decide the region intervals
void GetRegionIntervals(Eigen::MatrixXd *regionPoints,Eigen::MatrixXd *lines,int currentRow, int nCols);

// Get the lines COL = k * ROW + m
void GetRegionLines(Eigen::MatrixXd *regionPoints, Eigen::MatrixXd *lines,int rowV, int colV);

void GetRegionLinesV2(Eigen::MatrixXd &regions, Eigen::MatrixXd &lines);

void GetRegionIntervals(Eigen::MatrixXd *regionPoints,Eigen::MatrixXd *lines,int currentRow, int nCols)
{
  regionPoints->col(0)(0) = 0;
  regionPoints->col(regionPoints->cols()-1)(0)= nCols;
  
  for (int i=0;i<lines->rows();i++)
  {
    regionPoints->col(i+1)(0) = min(max((int)(lines->col(0)(i)*currentRow+lines->col(1)(i)),0),nCols);
    
  }
  
}


void GetRegionLinesV2(Eigen::MatrixXd &regions, Eigen::MatrixXd &lines)
{
  /* 
   0 col : col1 point
   1 col : row1 point
   2 col : col2 point
   3 col : row2 point
   */
   
  long nRegions = regions.rows();
  for (int i=0; i<nRegions;i++)
  {
    double k = (double)( regions.row(i)(2)-regions.row(i)(0)) / (double)(regions.row(i)(3)-regions.row(i)(1));
    double m = (double)(regions.row(i)(2) - k*regions.row(i)(3));
    lines.row(i)(0) = k;
    lines.row(i)(1) = m;
  }
}


void GetRegionLines(Eigen::MatrixXd *regionPoints, Eigen::MatrixXd *lines,int rowV, int colV)
{
  // Col = K * row + m
  long width = regionPoints->cols();
  for (int i=0; i<regionPoints->cols()-1; i++) {
    
    double k = (double)(colV-regionPoints->col(i)(0)) / (double)(rowV - regionPoints->col(width-1)(0));
    double m = (double)(colV - k*rowV);
    lines->col(0)(i) = k;
    lines->col(1)(i) = m;
    
  }
  
}


