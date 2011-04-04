#include "motiontracker.hh"

CalibrationParameters::CalibrationParameters() : intrinsic_parameters(), distortion_coeffs()
{ }

CalibrationParameters::CalibrationParameters(cv::Mat ip, cv::Mat dc) : intrinsic_parameters(ip), distortion_coeffs(dc)
{ }

void CalibrationParameters::saveToFile(std::string filename)
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
CalibrationParameters CalibrationParameters::fromFile(std::string filename)
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
	return CalibrationParameters(ip, dc);
}
