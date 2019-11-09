#include "GOP.h"

namespace wq {

	GOP::GOP()
	{
	}


	GOP::~GOP()
	{
	}

	void wq::GOP::init(const cv::Mat& first)
	{
		mOldMat = first.clone();
		mSize = mOldMat.size();
		mFdif = cv::Mat::zeros(mSize, CV_32FC1);
		mEcd = cv::Mat::zeros(mSize, CV_8UC1);
		mEdif = cv::Mat::zeros(mSize, CV_8UC1);
		mGhost = cv::Mat::zeros(mSize, CV_8UC1);
		mKernelIntegrateCD = cv::Mat::ones(cv::Size(D1, D1), CV_32FC1) / FV;
		mKernelIntegrateDif = cv::Mat::ones(cv::Size(D2, D2), CV_32FC1) / FV;
		Cmax = Cmin + len;
	}

	cv::Mat GOP::detect(const cv::Mat& now, const cv::Mat& Fcd)
	{
		mFcd = Fcd;
		cv::absdiff(now, mOldMat, mFdifTemp);
		mFdifTemp.convertTo(mFdifTemp, CV_32FC3);
		cv::split(mFdifTemp, mFdifC);
		mFdifTemp2 = cv::Mat::zeros(now.size(), CV_32FC1);
		for (int i = 0; i < mFdifC.size(); i++) {//LI
			cv::add(mFdifTemp2, mFdifC[i], mFdifTemp2);
		}
		cv::threshold(mFdifTemp2, mFdif, R, FV, CV_THRESH_BINARY);
		//cv::imwrite("D:\\master\\master_traindata\\dataset2014\\PaperTest\\Fcd.png", mFcd * 255);
		//cv::imwrite("D:\\master\\master_traindata\\dataset2014\\PaperTest\\Fdif.png", mFdif * 255);
		cv::filter2D(mFcd, mCountCD, CV_32FC1, mKernelIntegrateCD);//此处可以直接使用cda->getMsk();
		cv::filter2D(mFdif, mCountDif, CV_32FC1, mKernelIntegrateDif);
		cv::threshold(mCountCD, mEcdp, Cmin, FV, CV_THRESH_BINARY);
		cv::threshold(mCountCD, mEcd, Cmax, FV, CV_THRESH_BINARY_INV);
		cv::threshold(mCountDif, mEdif, Cpmax, FV, CV_THRESH_BINARY_INV);
		mEcd &= mEcdp;
		//cv::imwrite("D:\\master\\master_traindata\\dataset2014\\PaperTest\\Ecd.png", mEcd * 255);
		//cv::imwrite("D:\\master\\master_traindata\\dataset2014\\PaperTest\\Edif.png", mEdif * 255);

		mEdif &= mEcd;
		mEdif.convertTo(mEdif,CV_8UC1);
		//cv::imwrite("D:\\master\\master_traindata\\dataset2014\\PaperTest\\Error.png", mEdif * 255);
		mOldMat = now.clone();
		return Fcd - mEdif;
	}

}