

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <list>

using namespace std;

#include "cv.h"

#include "MainSetting.h"
#include "FeatureExtracter.h"
#include "svm\svm.h"



class Classifier {
	
	enum {SIZE_GIR,SIZE_D,SIZE_O,SIZE_M,SIZE_BP,SIZE_DISTO,SIZE_OM};

	public :
		
		void SetFeature(float **_girFeature,CvHistogram *_distanceHistFeature[100],CvHistogram *_orientationHistFeature[100],CvHistogram *_midPointHistFeature[100],
		CvHistogram *_betweenPairHistFeature[100],CvHistogram *_distAndOrientHistFeature[100],CvHistogram *_orientAndMidPointHistFeature[100]);
		void SetLabel(double * _label)
		{
			label = _label;
		}
		void SetSize(int _size)
		{
			size = _size;
		}
		void TrainDistanceHist();
		void TrainGir();
		void PredictDistanceHist(MainSetting s);
		void PredictGir(MainSetting s);

	private :

		float **GIRFeature;
		CvHistogram **distanceHistFeature;
		CvHistogram **orientationHistFeature;
		CvHistogram **midPointHistFeature;
		CvHistogram **betweenPairHistFeature;
		CvHistogram **distAndOrientHistFeature;
		CvHistogram **orientAndMidPointHistFeature;

		int size;
		double *label;
};

#endif
