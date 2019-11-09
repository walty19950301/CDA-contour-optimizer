#pragma once
#include <opencv2/opencv.hpp>

#define FV 1
#define BV 0
/**
* @auther Walty
* @email 494688825@qq.com
*/
namespace wq {
	class GOP
	{
	private:
		int R = 20, D1 = 11, D2 = 5, Cmax = 33, Cpmax = 1, RegionDist = 20;	//论文中的阈值
		cv::Mat mOldMat, mCountCD, mCountDif, mRegion, mEcdp, mFdifTemp, mFdifTemp2, mGhost;	//论文中未提及的 中间量
		std::vector<cv::Mat> mFdifC;
		cv::Mat mFdif, mFcd, mEdif, mEcd;										//论文中提及的变量
		cv::Size mSize;														//论文中未提及的变量
		cv::Mat mKernelIntegrateCD, mKernelIntegrateDif;					//积分操作，使用卷积实现
	public:
		GOP();
		~GOP();
		int len=36, Cmin = 0;
		//用于初始化，并且保存，用于之后的帧差法
		void init(const cv::Mat& first);
		/*
		*@param now 	BGR frame
		*@param mask	result mask of Change Detection Algorithm
		*用于检测，返回经过处理后正确的图像
		**/
		cv::Mat detect(const cv::Mat& now,const cv::Mat &mask);
	};
}

