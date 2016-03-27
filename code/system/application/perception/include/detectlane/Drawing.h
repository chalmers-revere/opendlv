#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Dense>

using namespace cv;

void DrawLine( Mat img, Point start, Point end ,Scalar color);
void DrawTracks(cv::Mat *src,Eigen::MatrixXd *k,Eigen::MatrixXd *m,int minRow,Scalar color);
void DrawBorders(cv::Mat *src, int minRow, int maxRow, double k1,double k2,double m1,double m2);

void DrawLine( Mat img, Point start, Point end ,Scalar color)
{
  int thickness = 2;
  int lineType = 1;
  line( img,
       start,
       end,
       color,//Scalar( 0, 255, 0 ),
       thickness,
       lineType );
}

void DrawTracks(cv::Mat *src,Eigen::MatrixXd *k,Eigen::MatrixXd *m,int minRow,Scalar color)
{
  int nRows = src->rows;
  for (int i = 0; i<k->rows();i++)
  {
    if (fabs(k->col(0)(i)) > 0.00001 || fabs(m->col(0)(i)) > 0.00001) {
      DrawLine(*src,Point((nRows*k->col(0)(i)+m->col(0)(i)),nRows), Point((minRow*k->col(0)(i)+m->col(0)(i)),minRow),color);
    }
  }
}

void DrawBorders(cv::Mat *src, int minRow, int maxRow, double k1,double k2,double m1,double m2)
{
    cv::Mat tmpImg = src->clone();
    Point pts[4][1];
    pts[0][0] = Point((maxRow*k1+m1),maxRow);
    pts[1][0] = Point((maxRow*k2+m2),maxRow);
    pts[3][0] = Point((minRow*k1+m1),minRow);
    pts[2][0] = Point((minRow*k2+m2),minRow);
    int nPoints = 4;
    const Point* ppt[1] = { pts[0]};
    fillPoly(tmpImg,ppt,&nPoints,1,Scalar(0,255,0),8);
    addWeighted( *src, 0.8, tmpImg, 0.2, 0.0, *src);
  
}


