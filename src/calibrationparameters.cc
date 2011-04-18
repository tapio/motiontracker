#include "motiontracker.hh"

CalibrationParameters::CalibrationParameters() : intrinsic_parameters(), distortion_coeffs(), hues(), dHues(), satval_l(), satval_h()
{ }

CalibrationParameters::CalibrationParameters(cv::Mat ip, cv::Mat dc, cv::Mat hues, cv::Mat dHues, cv::Mat satval_l, cv::Mat satval_h)
	: intrinsic_parameters(ip), distortion_coeffs(dc), hues(hues), dHues(dHues), satval_l(satval_l), satval_h(satval_h)
{ }

void CalibrationParameters::saveToFile(std::string filename)
{
	try {
		cv::FileStorage fs(filename, cv::FileStorage::WRITE);
		fs << "intrinsic" << intrinsic_parameters;
		fs << "distortion" << distortion_coeffs;

		fs << "hues" << hues;
		fs << "dHues" << dHues;
		fs << "satval_l" << satval_l;
		fs << "satval_h" << satval_h;

		fs.release();
	} catch (cv::Exception& e) {
		throw std::runtime_error(e.what());
	}
}

// static
CalibrationParameters CalibrationParameters::fromFile(std::string filename)
{
	cv::Mat ip, dc, hues, dHues, satval_l, satval_h;

	try {
		cv::FileStorage fs(filename, cv::FileStorage::READ);
		fs["intrinsic"] >> ip;
		fs["distortion"] >> dc;
		fs["hues"] >> hues;
		fs["dHues"] >> dHues;
		fs["satval_l"] >> satval_l;
		fs["satval_h"] >> satval_h;
		fs.release();
		// OpenCV silently ignores errors (like missing file to read),
		// so let's check that we got all parameters
		if (ip.empty() || dc.empty() || hues.empty() || dHues.empty() || satval_l.empty() || satval_h.empty())
			throw std::runtime_error("Missing calibration parameters in " + filename);
	} catch (cv::Exception& e) {
		throw std::runtime_error(e.what());
	}
	return CalibrationParameters(ip, dc, hues, dHues, satval_l, satval_h);
}
