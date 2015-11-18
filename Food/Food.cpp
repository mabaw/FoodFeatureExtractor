// Food.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
//#include <WinDef.h>
#include <iostream>
#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <math.h>
#include "MainSetting.h"

#include "FoodGUI.h"
#include "FeatureExtracter.h"
#include "Classifier.h"
#include "FeatureData.h"

#endif


bool	isRunning;

MainSetting			setting;
FoodGUI				gui;
FeatureExtracter	extracter;
Classifier			classifier;
FeatureData			featureData;



int main2( int argc, char** argv )
{
    // read soft label file
	setting.Intialize();
	extracter.Intialize(&setting);
	




	// access every soft label result file
	int item,cat,size;
	

	item=cat=1;
	int count=0;
	//while(cat<=6)
	featureData.GIRFeature = new float*[100];
	for(int i=0;i<100;i++)
		featureData.GIRFeature[i] = new float[COLOR];
	for(cat=1;cat<=6;cat++)
	{
		// while(extracter.LoadSTFResult(cat,item,&size))
		for(item=1;item<=12;item++)
		{
			extracter.LoadSTFResult(cat,item);
			printf("Load soft label %d_%d \n",cat,item);

			// do stm with label
			
			extracter.RandomPoint();
			float *tmp = extracter.GIRExtract();
		    for(int i=0;i<COLOR;i++)
				featureData.GIRFeature[count][i] = tmp[i];
			//cvCopyHist(extracter.DistanceHisr
			//extracter.MidPointHist();
			//extracter.BetweenPairHist();
			//extracter.DistAndOrientHist();
			//extracter.OrientAndMidPointHist();
			featureData.label[count] = cat;
			count++;
		}
	}

	float *result = 0;
	extracter.KernelGIRExtract(featureData.GIRFeature,featureData.label,72,"gir");
	
	cout << "Finish extract feature" << endl;
	classifier.SetLabel(featureData.label);
	classifier.SetFeature(featureData.GIRFeature,featureData.distanceHistFeature,featureData.orientationHistFeature,
		featureData.midPointHistFeature,featureData.betweenPairHistFeature,featureData.distAndOrientHistFeature,
		featureData.orientAndMidPointHistFeature);
	classifier.SetSize(count);

	

	/*classifier.TrainDistanceHist();
	classifier.PredictDistanceHist(setting);*/

	//classifier.TrainGir();

	//classifier.PredictGir(setting);
	getch();
    return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	MSG		msg;										// Windows Message Structure
	setting.Intialize();
	
	featureData.GIRFeature = new float*[100];
	for(int i=0;i<100;i++)
		featureData.GIRFeature[i] = new float[COLOR];

	gui.Initialize(&setting,&featureData);
//	if( !Initialize() ) 
	//{
	//	printf("Failed Initialize Program terminate !!!\n");
	//	return 0;
	//}
	isRunning = true;
	while( isRunning )										// Loop That Runs Until done = TRUE
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)					// Have We Received A Quit Message?
				isRunning = FALSE;						// If So done = TRUE
			else										// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);					// Translate The Message
				DispatchMessage(&msg);					// Dispatch The Message
			}
		}
		else											// If There Are No Messages
		{
			gui.Update();
			//Process();
		}
	}
	
	return 0;
}



#ifdef _EiC
main(1,"");
#endif
