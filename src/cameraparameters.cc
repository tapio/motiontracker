#include "motiontracker.hh"

CameraParameters::CameraParameters() : intrinsic_parameters(), distortion_coeffs()
{ }

CameraParameters::CameraParameters(cv::Mat ip, cv::Mat dc) : intrinsic_parameters(ip), distortion_coeffs(dc)
{ }

void CameraParameters::saveToFile(std::string filename)
{
	try {
		cv::FileStorage fs(filename, cv::FileStorage::WRITE);
		fs << "intrinsic" << intrinsic_parameters;
		fs << "distortion" << distortion_coeffs;
		fs.release();
	} catch (cv::Exception& e) {
		throw std::runtime_error(e.what());
	}
}

// static
CameraParameters CameraParameters::fromFile(std::string filename)
{
	cv::Mat ip, dc;
	try {
		cv::FileStorage fs(filename, cv::FileStorage::READ);
		fs["intrinsic"] >> ip;
		fs["distortion"] >> dc;
		fs.release();
	} catch (cv::Exception& e) {
		throw std::runtime_error(e.what());
	}
	return CameraParameters(ip, dc);
}
