#include <iostream>
#include <vector>

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


using namespace std;

#define FOCAL_LENGTH 1000
#define CUBE_SIZE 10

int main(int argc, char** argv)
{
	(void) argc, (void) argv;
	cout << "OpenCV POSIT tutorial" << endl;
	cout << "by Javier Barandiaran(jbarandiaran@gmail.com)" << endl;

	CvPOSITObject* positObject;

	//Create the model pointss
	std::vector<CvPoint3D32f> modelPoints;
	modelPoints.push_back(cvPoint3D32f(0.0f, 0.0f, 0.0f)); //The first must be (0,0,0)
	modelPoints.push_back(cvPoint3D32f(0.0f, 0.0f, CUBE_SIZE));
	modelPoints.push_back(cvPoint3D32f(CUBE_SIZE, 0.0f, 0.0f));
	modelPoints.push_back(cvPoint3D32f(0.0f, CUBE_SIZE, 0.0f));

	//Create the image points
	std::vector<CvPoint2D32f> srcImagePoints;
	srcImagePoints.push_back( cvPoint2D32f( -48, -224 ) );
	srcImagePoints.push_back( cvPoint2D32f( -287, -174 ) );
	srcImagePoints.push_back( cvPoint2D32f( 132, -153 ) );
	srcImagePoints.push_back( cvPoint2D32f( -52, 149 ) );

	//Create the POSIT object with the model points
	positObject = cvCreatePOSITObject( &modelPoints[0], (int)modelPoints.size() );

	//Estimate the pose
	CvMatr32f rotation_matrix = new float[9];
	CvVect32f translation_vector = new float[3];
	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 1.0e-4f);
	cvPOSIT( positObject, &srcImagePoints[0], FOCAL_LENGTH, criteria, rotation_matrix, translation_vector );

	cout << "\n-.- SOURCE MODEL POINTS -.-\n";
	for ( size_t  p=0; p<modelPoints.size(); p++ )
		cout << modelPoints[p].x << ", " << modelPoints[p].y << ", " << modelPoints[p].z << "\n";

	cout << "\n-.- SOURCE IMAGE POINTS -.-\n";
	for ( size_t p=0; p<srcImagePoints.size(); p++ )
		cout << srcImagePoints[p].x << ", " << srcImagePoints[p].y << " \n";

	cout << "\n-.- ESTIMATED ROTATION\n";
	for ( size_t p=0; p<3; p++ )
		cout << rotation_matrix[p*3] << " | " << rotation_matrix[p*3+1] << " | " << rotation_matrix[p*3+2] << "\n";

	cout << "\n-.- ESTIMATED TRANSLATION\n";
	cout << translation_vector[0] << " | " << translation_vector[1] << " | " << translation_vector[2] << "\n";

	//Project the model points with the estimated pose
	std::vector<CvPoint2D32f> projectedPoints;
	for ( size_t  p=0; p<modelPoints.size(); p++ )
	{
		CvPoint3D32f point3D;
		point3D.x = rotation_matrix[0] * modelPoints[p].x +
			rotation_matrix[1] * modelPoints[p].y +
			rotation_matrix[2] * modelPoints[p].z +
			translation_vector[0];
		point3D.y = rotation_matrix[3] * modelPoints[p].x +
			rotation_matrix[4] * modelPoints[p].y +
			rotation_matrix[5] * modelPoints[p].z +
			translation_vector[1];
		point3D.z = rotation_matrix[6] * modelPoints[p].x +
			rotation_matrix[7] * modelPoints[p].y +
			rotation_matrix[8] * modelPoints[p].z +
			translation_vector[2];
		CvPoint2D32f point2D = cvPoint2D32f( 0.0, 0.0 );
		if ( point3D.z != 0 )
		{
			point2D.x = FOCAL_LENGTH * point3D.x / point3D.z;
			point2D.y = FOCAL_LENGTH * point3D.y / point3D.z;
		}
		projectedPoints.push_back( point2D );
	}

	IplImage *img = cvLoadImage( "img/img.jpg" );
	if ( !img )
	{
		cout << "Image not loaded" << endl;
		return 0;
	}

	int centreX = static_cast<int>( img->width * 0.5 );
	int centreY = static_cast<int>( img->height * 0.5 );
	//Draw the source image points
	for ( size_t p=0; p<modelPoints.size(); p++ )
		cvCircle( img, cvPoint( centreX + (int)srcImagePoints[p].x, centreY - (int)srcImagePoints[p].y ), 8, CV_RGB(255,0,0 ) );
	//Draw the axes
	cvLine( img, cvPoint( centreX + (int)projectedPoints[0].x, centreY - (int)projectedPoints[0].y ),
				cvPoint( centreX + (int)projectedPoints[1].x, centreY - (int)projectedPoints[1].y ), CV_RGB(  0, 0, 255 ), 2 );
	cvLine( img, cvPoint( centreX + (int)projectedPoints[0].x, centreY - (int)projectedPoints[0].y ),
				cvPoint( centreX + (int)projectedPoints[2].x, centreY - (int)projectedPoints[2].y ), CV_RGB( 255, 0, 0 ), 2 );
	cvLine( img, cvPoint( centreX + (int)projectedPoints[0].x, centreY - (int)projectedPoints[0].y ),
				cvPoint( centreX + (int)projectedPoints[3].x, centreY - (int)projectedPoints[3].y ), CV_RGB( 0, 255, 0 ), 2 );
	//Draw the projected model points
	cout << "\n-.- ESTIMATED IMAGE POINTS -.-\n";
	for ( size_t p=0; p<projectedPoints.size(); p++ )
	{
		cvCircle( img, cvPoint( centreX + (int)projectedPoints[p].x, centreY - (int)projectedPoints[p].y ), 3, CV_RGB(255,255,255 ), -1 );
		cout << projectedPoints[p].x << ", " << projectedPoints[p].y << " \n";
	}
	cvNamedWindow( "POSIT" );
	cvShowImage( "POSIT", img );
	cvWaitKey( );

	delete rotation_matrix;
	delete translation_vector;
	cvReleasePOSITObject(&positObject);
	cvReleaseImage( &img );

	return 0;
}
