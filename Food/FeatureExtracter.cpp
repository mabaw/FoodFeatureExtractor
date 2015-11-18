#include "FeatureExtracter.h"



bool FeatureExtracter::LoadSTFResult(int nCat,int nItm)
{
	char tmp[MAXPATH];
	sprintf(tmp,"%s%d_%d_s.txt",setting->STFDir,nCat,nItm);
	FILE *fp = fopen(tmp,"r");

	
	if(fp!=0)
	{
		
		fscanf(fp,"%d %d %d",&width,&height,&channel);
		for(int i=0;i<channel;i++)
		{			
			if(softLabel[i])
			{
				cvReleaseMat(&softLabel[i]);
				softLabel[i]=0;
			}
			softLabel[i]= cvCreateMatHeader(height,width,CV_32FC1);
			cvCreateData(softLabel[i]);
		}
		for(int i=0;i<height;i++)
			for(int j=0;j<width;j++)
			{
				for(int k=0;k<channel;k++)
				{
					float f;
					fscanf(fp,"%f",&f);
					*( (float*)CV_MAT_ELEM_PTR(*softLabel[k],i,j)) = f; 		
					float v = CV_MAT_ELEM(*softLabel[k],float,i,j);
					//printf("(i,i) = (%d,%d) v = %f\n",i,j,v);
				}

			}

		//printf("%d %d %d\n",nCat,nItm, *numCat);
		fclose(fp);
		return true;
	}

	return false;
}

void FeatureExtracter::ClearMat()
{
	for(int i=0;i<COLOR;i++)
		cvReleaseMat(&softLabel[i]);

}

float*  FeatureExtracter::GIRExtract()
{
	int nNonBG=0;
	for (int i = 0; i < height; i++)
         for (int j = 0; j < width; j++)
                    {
						
                        if (CV_MAT_ELEM(*softLabel[0],float,i,j) < 0.5) nNonBG++;
                        for (int k = 1; k < channel; k++)
                        {
                            GIRFeature[k] +=CV_MAT_ELEM(*softLabel[k],float,i,j) ;
                        }
                    }
                if(nNonBG>0)
                    for (int m = 0; m < COLOR; m++)
                        GIRFeature[m] /= nNonBG;
	printf("GIR Feature : ");
	for (int m = 0; m < COLOR; m++)
		printf("%f ",GIRFeature[m]);
	printf("\n");

	return GIRFeature;

}

void FeatureExtracter::RandomPoint()
{
	int **mark; // mark that this pixel already take or not
	int numSelect = 0; // count the selected pixel
	int randWidth,randHeight;
	// clear memory
	if(randomPoints!=0)
	{
		free( randomPoints );
		randomPoints=0;
	}
	mark = (int**)malloc(sizeof(int**)*width*height);
	randomPoints = (CvPoint*)malloc(sizeof(CvPoint)*pointsAmount);
		//new CvPoint[pointsAmount];	
	*mark = (int *)malloc(sizeof(int)*height);
		//new int[height];
	for(int i=0;i<height;i++)
	{
		mark[i] = (int*)malloc(sizeof(int)*width);
			//new int[width];
		//for(int j=0;j<width;j++)
		//	mark[j] = 0;
		
		memset(mark[i],0,sizeof(int)*width);
	}
	srand ( time(NULL) );
	while(numSelect<100)
	{
		randWidth = rand() % width;
		randHeight = rand() % height;

		// not choose yet and not background
		if(mark[randHeight][randWidth]==0 && CV_MAT_ELEM(*softLabel[0],float,randHeight,randWidth) < 0.1)
		{
			mark[randHeight][randWidth]=1;
			randomPoints[numSelect].x = randWidth;
			randomPoints[numSelect].y = randHeight;			
			numSelect++;
		}
	}

	for(int i=0;i<height;i++)
		free(mark[i]);
		//delete mark[i];
	free(mark);
}

int  FeatureExtracter::calcBin(float value,float lowerBound,float upperBound,int n)
{
	float step = (upperBound - lowerBound) / n;
	return (value - lowerBound) / step;
}

void FeatureExtracter::KernelGIRExtract(float **feature1,double label[],int size,char *result)
{
	FILE *fout = fopen(result,"w");
	for(int i=0;i<size;i++)
	{
		cout << label[i] << " ";
		cout << "0:" << i << " ";
		fprintf(fout,"%lf 0:%d ",label[i],i);
		for(int j=0;j<size;j++)
		{
			cout << j+1 << ":" ;
			fprintf(fout,"%d:",j+1);
			double sum=0;
			for(int k=0;k<COLOR;k++)
			{
				double diff = feature1[i][k] - feature1[j][k];
				double plus = feature1[i][k] + feature1[j][k];
				if(plus != 0)
					sum += diff*diff/plus;
			}
			cout << sum << " " ;
			fprintf(fout,"%lf ",sum);

		}

		cout << endl;
		fprintf(fout,"\n");
		//getch();
	}
	fclose(fout);

}
void KernelDistanceHist(CvHistogram *feature1[],int size,float *result){}
void KernelOrientationHist(CvHistogram *feature1[],int size,float *result){}
void KernelMidPointHist(CvHistogram *feature1[],int size,float *result){}
void KernelBetweenPairHist(CvHistogram *feature1[],int size,float *result){}
void KernelDistAndOrientHist(CvHistogram *feature1[],int size,float *result){}
void KernelOrientAndMidPointHist(CvHistogram *feature1[],int size,float *result){}







CvHistogram *  FeatureExtracter::DistanceHist(){

	int dim[3];
	dim[0] = dim[1] = channel;
	dim[2] = 12;
	float chRange[2];
	chRange[0] = 0;
	chRange[1] = channel-1;
	float distRange[] = {0,5};// try later >w<
	float *range[] = {chRange,chRange,distRange};

	if(distanceHistFeature)
		cvReleaseHist(&distanceHistFeature);

	distanceHistFeature = cvCreateHist(3,dim,CV_HIST_ARRAY,range,1);
	float min=10;
	float max=0;

	for(int m = 0;m<channel;m++)
	{
				for(int n=0;n<channel;n++)
				{
					for(int o=0;o<12;o++)
					{
						float *value = cvGetHistValue_3D(distanceHistFeature,m,n,o);
						*value = 0;
					}
				}
	}


	/*	for(int m = 0;m<channel;m++)
	{
				for(int n=0;n<channel;n++)
				{
					for(int o=0;o<12;o++)
					{
						float *value = cvGetHistValue_3D(distanceHistFeature,m,n,o);
						cout << *value << " " ;
					}
					cout << endl;
				}
		cout << endl << endl;
	}*/

	for(int i=0;i<pointsAmount;i++)
		for(int j=i+1;j<pointsAmount;j++)
		{
			float x = randomPoints[i].x-randomPoints[j].x;
			float y = randomPoints[i].y-randomPoints[j].y;
			float distance = log(ceil(sqrt(x*x + y*y) + 1));
			if(distance<min) min = distance;
			if(distance>max) max = distance;
			int distBin = calcBin(distance,0,5,12);
			for(int m = 0;m<channel;m++)
				for(int n=0;n<channel;n++)
				{
					float *value = cvGetHistValue_3D(distanceHistFeature,m,n,distBin);
					*value += (CV_MAT_ELEM(*softLabel[m],float,randomPoints[i].y,randomPoints[i].x)+
					CV_MAT_ELEM(*softLabel[n],float,randomPoints[j].y,randomPoints[j].x)); 
				}
		}

	/*	
		for(int m = 0;m<channel;m++)
	{
				for(int n=0;n<channel;n++)
				{
					for(int o=0;o<12;o++)
					{
						float *value = cvGetHistValue_3D(distanceHistFeature,m,n,o);
						cout << *value << " " ;
					}
					cout << endl;
				}
		cout << endl << endl;
	}*/

		return distanceHistFeature;
}
void  FeatureExtracter::OrientationHist(){

	int dim[3];
	dim[0] = dim[1] = channel;
	dim[2] = 12;
	float chRange[2];
	chRange[0] = 0;
	chRange[1] = channel-1;
	float distRange[] = {0,360};
	float *range[] = {chRange,chRange,distRange};

	if(distanceHistFeature)
		cvReleaseHist(&orientationHistFeature);

	orientationHistFeature = cvCreateHist(3,dim,CV_HIST_ARRAY,range,1);
	float min=360;
	float max=-1;
	for(int i=0;i<pointsAmount;i++)
		for(int j=i+1;j<pointsAmount;j++)
		{
			float x = randomPoints[i].x-randomPoints[j].x;
			float y = randomPoints[i].y-randomPoints[j].y;
			float orient = (atan2(y,x) *  180 / PI) + 180 ;
			if(orient<min) min = orient;
			if(orient>max) max = orient;
			//printf("max = %f min = %f \n",max,min);
			
			int distBin = calcBin(orient,0,360,11);
			//printf("orient = %f distbin = %d \n",orient,distBin);
			for(int m = 0;m<channel;m++)
				for(int n=0;n<channel;n++)
				{
					float *value = cvGetHistValue_3D(orientationHistFeature,m,n,distBin);
					*value += (CV_MAT_ELEM(*softLabel[m],float,randomPoints[i].y,randomPoints[i].x)+
					CV_MAT_ELEM(*softLabel[n],float,randomPoints[j].y,randomPoints[j].x)); 
				}
		}

}
void  FeatureExtracter::MidPointHist()
{

	int dim[3];
	dim[0] = dim[1] = channel;
	dim[2] = channel;
	float chRange[2];
	chRange[0] = 0;
	chRange[1] = channel-1;
	float *range[] = {chRange,chRange,chRange};

	if(midPointHistFeature)
		cvReleaseHist(&midPointHistFeature);

	midPointHistFeature = cvCreateHist(3,dim,CV_HIST_ARRAY,range,1);
	float min=10;
	float max=0;
	for(int i=0;i<pointsAmount;i++)
		for(int j=i+1;j<pointsAmount;j++)
		{
			int midx = abs(randomPoints[i].x-randomPoints[j].x) / 2.0;
			int midy = abs(randomPoints[i].y-randomPoints[j].y) / 2.0;			
			for(int m = 0;m<channel;m++)
				for(int n=0;n<channel;n++)
				{
					for(int o=0;o<channel;o++)
					{
						float *value = cvGetHistValue_3D(midPointHistFeature,m,n,o);
						*value += CV_MAT_ELEM(*softLabel[m],float,randomPoints[i].y,randomPoints[i].x)+
						CV_MAT_ELEM(*softLabel[n],float,randomPoints[j].y,randomPoints[j].x) +
						CV_MAT_ELEM(*softLabel[o],float,midy,midx); 
					}
				}
		}
}

int FeatureExtracter::findLineList2(Point2F a,Point2F b,Point2F* list,int size)
{

	Point2F mid;
	mid.x = (a.x + b.x) / 2.0;
	mid.y = (a.y + b.y) / 2.0;
	list[size].x	= mid.x;
	list[size++].y	= mid.y;	
	if (abs(a.x-b.x) < 1 &&abs(a.y-b.y) < 1)
		return size;
	size = findLineList2(a,mid,list,size);
	size = findLineList2(mid,b,list,size);
	return size;

}

FeatureExtracter::Point2F* FeatureExtracter::findLineList(FeatureExtracter::Point2F a,FeatureExtracter::Point2F b, int *size)
{
	FeatureExtracter::Point2F list[2000];
	*size = findLineList2(a,b,list,0);
	return list;
}


void  FeatureExtracter::BetweenPairHist(){

	
	int dim[3];
	dim[0] = dim[1] = channel;
	dim[2] = channel;
	float chRange[2];
	chRange[0] = 0;
	chRange[1] = channel-1;
	float *range[] = {chRange,chRange,chRange};

	if(betweenPairHistFeature)
		cvReleaseHist(&betweenPairHistFeature);

	betweenPairHistFeature = cvCreateHist(3,dim,CV_HIST_ARRAY,range,1);
	
	float min=10;
	float max=0;
	for(int i=0;i<pointsAmount;i+=10)
		for(int j=i+1;j<pointsAmount;j+=10)
		{
			int size=0;
			Point2F a,b;
			a.x = randomPoints[i].x;
			a.y = randomPoints[i].y;
			b.x = randomPoints[j].x;
			b.y = randomPoints[j].y;
			Point2F *list = findLineList(a,b, &size);
			//cout << size << endl;
			for(int ii = 0;ii<size;ii++)
			{
				
				for(int m = 0;m<channel;m++)
					for(int n=0;n<channel;n++)
					{
						for(int o=0;o<channel;o++)
						{
							float *value = cvGetHistValue_3D(betweenPairHistFeature,m,n,o);
							*value += CV_MAT_ELEM(*softLabel[m],float,randomPoints[i].y,randomPoints[i].x)+
							CV_MAT_ELEM(*softLabel[n],float,randomPoints[j].y,randomPoints[j].x) +
							CV_MAT_ELEM(*softLabel[o],float,(int)list[ii].y,(int)list[ii].x); 
						}
					}
			}
		}

}
void  FeatureExtracter::DistAndOrientHist()
{
	int dim[4];
	dim[0] = dim[1] = channel;
	dim[2] = dim[3] = 12;
	float chRange[2];
	chRange[0] = 0;
	chRange[1] = channel-1;
	float distRange[] = {0,5};
	float orientRange[] = {0,360};
	float *range[] = {chRange,chRange,distRange,orientRange};

	if(distAndOrientHistFeature)
		cvReleaseHist(&distAndOrientHistFeature);

	distAndOrientHistFeature = cvCreateHist(4,dim,CV_HIST_ARRAY,range,1);
	float min=10;
	float max=0;
	for(int i=0;i<pointsAmount;i++)
		for(int j=i+1;j<pointsAmount;j++)
		{
			float x = randomPoints[i].x-randomPoints[j].x;
			float y = randomPoints[i].y-randomPoints[j].y;
			float distance = log(ceil(sqrt(x*x + y*y) + 1));
			if(distance<min) min = distance;
			if(distance>max) max = distance;


			float orient = (atan2(y,x) *  180 / PI) + 180 ;
			if(orient<min) min = orient;
			if(orient>max) max = orient;
			int distBin2 = calcBin(orient,0,360,11);
			
			int distBin = calcBin(distance,0,5,12);
			for(int m = 0;m<channel;m++)
				for(int n=0;n<channel;n++)
				{		
					int a[4] = {m,n,distBin,distBin2};
					float *value = cvGetHistValue_nD(distAndOrientHistFeature,a);
					*value += (CV_MAT_ELEM(*softLabel[m],float,randomPoints[i].y,randomPoints[i].x)+
					CV_MAT_ELEM(*softLabel[n],float,randomPoints[j].y,randomPoints[j].x)); 
				}
		}


}
void  FeatureExtracter::OrientAndMidPointHist()
{
		int dim[3];
	dim[0] = dim[1] = channel;
	dim[2] = 12;
	float chRange[2];
	chRange[0] = 0;
	chRange[1] = channel-1;
	float distRange[] = {0,360};
	float *range[] = {chRange,chRange,distRange};

	if(distanceHistFeature)
		cvReleaseHist(&orientationHistFeature);

	orientationHistFeature = cvCreateHist(3,dim,CV_HIST_ARRAY,range,1);
	float min=10;
	float max=0;
	for(int i=0;i<pointsAmount;i++)
		for(int j=i+1;j<pointsAmount;j++)
		{
			float x = randomPoints[i].x-randomPoints[j].x;
			float y = randomPoints[i].y-randomPoints[j].y;
			float orient = (atan2(y,x) *  180 / PI) + 180 ;
			if(orient<min) min = orient;
			if(orient>max) max = orient;
			int distBin = calcBin(orient,0,360,11);

			int midx = abs(randomPoints[i].x-randomPoints[j].x) / 2.0;
			int midy = abs(randomPoints[i].y-randomPoints[j].y) / 2.0;			

			for(int m = 0;m<channel;m++)
				for(int n=0;n<channel;n++)
				{
					for(int o=0;o<channel;o++)
					{
						int a[4] = {m,n,distBin,o};
						float *value = cvGetHistValue_nD(orientationHistFeature,a);
						*value += (CV_MAT_ELEM(*softLabel[m],float,randomPoints[i].y,randomPoints[i].x)+
						CV_MAT_ELEM(*softLabel[n],float,randomPoints[j].y,randomPoints[j].x) +
						CV_MAT_ELEM(*softLabel[o],float,midy,midx)); 
					}
				}
		}



}
