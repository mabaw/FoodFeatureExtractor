

#ifndef FEATURE_EXTRACTER_H
#define FEATURE_EXTRACTER_H

#include <string>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
using namespace std;

#include "cv.h"

#include "MainSetting.h"

#define PI 3.14159265


class FeatureExtracter {

	


	public :
		struct Point2F{
			float x;
			float y;
		};
		CvMat			*softLabel[COLOR];
		float GIRFeature[COLOR];
		CvHistogram *distanceHistFeature;
		CvHistogram *orientationHistFeature;
		CvHistogram *midPointHistFeature;
		CvHistogram *betweenPairHistFeature;
		CvHistogram *distAndOrientHistFeature;
		CvHistogram *orientAndMidPointHistFeature;

		float* GIRExtract();
		CvHistogram * DistanceHist();
		void OrientationHist();
		void MidPointHist();
		void BetweenPairHist();
		void DistAndOrientHist();
		void OrientAndMidPointHist();
		bool LoadSTFResult(int nCat,int nItm); // read the soft label result
		void RandomPoint();

		
		void KernelGIRExtract(float **feature1,double label[],int size,char *result);
		void KernelDistanceHist(CvHistogram *feature1[],int size,float *result);
		void KernelOrientationHist(CvHistogram *feature1[],int size,float *result);
		void KernelMidPointHist(CvHistogram *feature1[],int size,float *result);
		void KernelBetweenPairHist(CvHistogram *feature1[],int size,float *result);
		void KernelDistAndOrientHist(CvHistogram *feature1[],int size,float *result);
		void KernelOrientAndMidPointHist(CvHistogram *feature1[],int size,float *result);



		void ClearMat();

		FeatureExtracter()
		{
			randomPoints=0;
			pointsAmount = 100; // amount of random point 100 is Paper's suggestion
			for(int i=0;i<COLOR;i++)
				softLabel[i] = 0;

			distanceHistFeature=0;
			orientationHistFeature=0;
			midPointHistFeature=0;
			betweenPairHistFeature=0;
			distAndOrientHistFeature=0;
			orientAndMidPointHistFeature=0;
		}

		~FeatureExtracter()
		{
			for(int i=0;i<COLOR;i++)
			{
				if(softLabel[i]!=0)
					cvReleaseMat(&softLabel[i]);
			}
			if(distanceHistFeature)
				cvReleaseHist(&distanceHistFeature);
			if(orientationHistFeature)
				cvReleaseHist(&orientationHistFeature);
			if(midPointHistFeature)
				cvReleaseHist(&midPointHistFeature);
			if(betweenPairHistFeature)
				cvReleaseHist(&betweenPairHistFeature);
			if(distAndOrientHistFeature)
				cvReleaseHist(&distAndOrientHistFeature);
			if(orientAndMidPointHistFeature)
				cvReleaseHist(&orientAndMidPointHistFeature);

			if(randomPoints)
				delete [] randomPoints;
		}

		bool Intialize(MainSetting *_setting)
		{
			setting = _setting;

			
			
			return false;
		}

	private :
		MainSetting		*setting;
		
		int				width,height,channel;
		CvPoint			*randomPoints;
		int				pointsAmount;
	
		int calcBin(float value,float lowerBound,float upperBound,int n);
		int findLineList2(Point2F a,Point2F b,Point2F* list,int size);
		FeatureExtracter::Point2F* findLineList(Point2F a,Point2F b,int *size);
		

};

#endif
