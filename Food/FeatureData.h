#ifndef FEATURE_DATA_H
#define FEATURE_DATA_H


class FeatureData {
public :
	
	double		label[100];
	float		**GIRFeature;
	CvHistogram *distanceHistFeature[100];
	CvHistogram *orientationHistFeature[100];
	CvHistogram *midPointHistFeature[100];
	CvHistogram *betweenPairHistFeature[100];
	CvHistogram *distAndOrientHistFeature[100];
	CvHistogram *orientAndMidPointHistFeature[100];



};

#endif