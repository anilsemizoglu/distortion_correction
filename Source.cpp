#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"
#include "opencv2\imgproc.hpp"
#include <iostream>
#include <sstream>

#define DEBUG_CHESSBOARD

using namespace std;
using namespace cv;

int main() {

		int h = 17, w = 17, n_pics = 11;

		vector<vector<Point2f>> imagePoints; // this will be filled with pointbuf for multiple images	
		vector<vector<Point3f>> arrayObjectPoints;

		vector<Mat> cameraMatrix_pics, distCoeff_pics, rvecs, tvecs;

		Mat  cameraMatrix, distCoeffs, img, img_bw;

		for (int i = 0; i < n_pics; i++) {

			
			vector<Point2f> pointBuf;		//this will be filled by the detected centers for each image		
			vector<Point3f> objectPoints;
		
			ostringstream oss;
			oss << "18x18_board/img" << i << ".jpg";
			img = imread(oss.str());

			cvtColor(img, img_bw, COLOR_BGR2GRAY);
			objectPoints.clear();

			for (int y = 0; y < 17; ++y) {
			for (int x = 0; x < 17; ++x) {
			objectPoints.push_back(Point3f(x * 24.3, y * 24.3, 0));
					}
			}

			// One vector of chessboard points for each chessboard image
			arrayObjectPoints.push_back(objectPoints);
			
			bool found = findChessboardCorners(img_bw, Size(w, h), pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
			cout << "found: " << found << endl;

			if (found) {
			drawChessboardCorners(img, Size(w, h), Mat(pointBuf), found);
			imshow(oss.str(), img);
			imagePoints.push_back(pointBuf);
			
			cameraMatrix_pics.push_back(cameraMatrix);
			distCoeff_pics.push_back(distCoeffs);
			} //if found
			
		} // picture for loop

		double rms = calibrateCamera(arrayObjectPoints, imagePoints, img.size(), cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K4 | CALIB_FIX_K5);
		cout << rms << endl;

		//undistort the pics
		for (int i = 0; i < n_pics; i++) {
			ostringstream oss;
			oss << "18x18_board/img" << i << ".jpg";
			Mat img = imread(oss.str());

			Mat img_ud, R;
			Mat new_camera_matrix, map1, map2;

			initUndistortRectifyMap(cameraMatrix, distCoeffs, R, new_camera_matrix, img.size(), CV_32FC1, map1, map2);
			remap(img, img_ud, map1, map2, INTER_LANCZOS4, BORDER_CONSTANT);

		//	imshow(oss.str(), img_ud);
		}
		waitKey(0);
	return 0;
}
