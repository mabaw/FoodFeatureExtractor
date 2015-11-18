#include "Classifier.h"


void Classifier::SetFeature(float **_girFeature,CvHistogram *_distanceHistFeature[],CvHistogram *_orientationHistFeature[],CvHistogram *_midPointHistFeature[],
		CvHistogram *_betweenPairHistFeature[],CvHistogram *_distAndOrientHistFeature[],CvHistogram *_orientAndMidPointHistFeature[])
{
	GIRFeature						=	_girFeature;
	distanceHistFeature				=	_distanceHistFeature;
	orientationHistFeature			=	_orientationHistFeature;
	midPointHistFeature				=	_midPointHistFeature;	
	betweenPairHistFeature			=	_betweenPairHistFeature;
	distAndOrientHistFeature		=	_distAndOrientHistFeature;
	orientAndMidPointHistFeature	=	_orientAndMidPointHistFeature;

}

void Classifier::TrainGir()
{
	svm_problem prob;
	svm_parameter param;
		
	// default values
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	prob.l = size;
	prob.y = new double[prob.l];

	if(param.gamma == 0) param.gamma = 0.5;
	svm_node *x_space = new svm_node[8 * prob.l];
	prob.x = new svm_node *[prob.l];

	for(int i=0;i<size;i++)
		{
			for(int m=0;m<7;m++)
			{						
						x_space[i*8+m].index = m;
						x_space[i*8+m].value = GIRFeature[i][m];	
						//cout << GIRFeature[i][m] << endl;
			}
			x_space[i*8+7].index = -1;
		
			prob.x[i] = &x_space[8 * i];
			prob.y[i] = label[i];
		}

		for(int i=0;i<size;i++)
		{
			for(int j=0;j<8;j++)
			{
				cout << (prob.x[i]+j)->index << " ";

			}
			cout << endl;
		}
		const char *error_msg;
		error_msg = svm_check_parameter(&prob,&param);

		if(error_msg)
		{
			fprintf(stderr,"Error: %s\n",error_msg);
			exit(1);
		}
		// build model 
		svm_model *model = svm_train(&prob, &param);
		svm_save_model("svmModel_gir", model);


		svm_free_and_destroy_model(&model);
		delete[] x_space;
		delete[] prob.x;
		delete[] prob.y;

	free(param.weight_label);
	free(param.weight);
}

void Classifier::TrainDistanceHist()
{
	svm_problem prob;


	svm_parameter param;
		
	// default values
	param.svm_type = C_SVC;
	param.kernel_type = POLY;
	param.degree = 3;
	param.gamma = 0;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	prob.l = size;
	prob.y = new double[prob.l];

	if(param.gamma == 0) param.gamma = 0.5;
	svm_node *x_space = new svm_node[(589) * prob.l];//7 * 7 * 12 + 1
	prob.x = new svm_node *[prob.l];

	for(int i=0;i<size;i++)
//	for (list<CvHistogram>::iterator q = point_list.begin(); q != point_list.end(); q++, i++)
		{
			int count=0;
			int m,n,o;
			for( m=0;m<7;m++)
				for( n=0;n<7;n++)
					for( o=0;o<12;o++)
					{
						//cout << m << " " << n << " " << o << " " << size << endl;
						float *value = cvGetHistValue_3D(distanceHistFeature[i],m,n,o);
						x_space[i*(589)+m*(7*12)+n*12+o].index = count++;;
						x_space[i*(589)+m*(7*12)+n*12+o].value = *value;	
					}
			x_space[(i+1)*(589)-1].index = -1;
			
			prob.x[i] = &x_space[ i * (589) ];
			prob.y[i] = label[i];
		}
		const char *error_msg;
		error_msg = svm_check_parameter(&prob,&param);
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<589;j++)
			cout << (prob.x[i]+j)->index << " ";
		cout << endl;
	}

		if(error_msg)
		{
			fprintf(stderr,"Error: %s\n",error_msg);
			exit(1);
		}
		// build model 
		svm_model *model = svm_train(&prob, &param);
		svm_save_model("svmModel_distance", model);


		svm_free_and_destroy_model(&model);
		delete[] x_space;
		delete[] prob.x;
		delete[] prob.y;

	free(param.weight_label);
	free(param.weight);
}


void Classifier::PredictGir(MainSetting s)
{
	svm_model *model = svm_load_model("svmModel_gir");

	// load result
	FeatureExtracter f;
	f.Intialize(&s);
		for(int cat=1;cat<=6;cat++)
			for(int itm=1;itm<=12;itm++)
			{
				int size;
				f.LoadSTFResult(cat,itm); // read the soft label result
				f.RandomPoint();
				f.GIRExtract();
				svm_node *x_space = (svm_node*)malloc(sizeof(svm_node)*(7 * 7 * 12 + 1));
				//	new svm_node[(7 * 7 * 12 + 1)];				
				int m,n,o;
				int count=0;
				for( m=0;m<7;m++)				
				{
					x_space[m].index = count++;
					x_space[m].value = f.GIRFeature[m];
				}
				x_space[7].index = -1;
		
				double d = svm_predict(model, x_space);
				cout << "GIR : cat = " << cat << " predict = " << d << endl;
				free( x_space );

			}
}

void Classifier::PredictDistanceHist(MainSetting s)
{
	svm_model *model = svm_load_model("svmModel_distance");

	// load result
	FeatureExtracter f;
	f.Intialize(&s);
		for(int cat=1;cat<=6;cat++)
			for(int itm=1;itm<=12;itm++)
			{
				int size;
				f.LoadSTFResult(cat,itm); // read the soft label result
				f.RandomPoint();
				f.DistanceHist();
				svm_node *x_space = (svm_node*)malloc(sizeof(svm_node)*(7 * 7 * 12 + 1));
				//	new svm_node[(7 * 7 * 12 + 1)];				
				int m,n,o;
				int count=0;
				for( m=0;m<7;m++)
					for( n=0;n<7;n++)
						for( o=0;o<12;o++)
						{
							
							float *value = cvGetHistValue_3D(f.distanceHistFeature,m,n,o);
							//cout << m << " " << n << " " << o << " "<< *value<< endl;
							x_space[m*(7*12)+n*7+o].index = count++;
							x_space[m*(7*12)+n*7+o].value = *value;
						}
				x_space[7*7*12].index = -1;
			
				double d = svm_predict(model, x_space);
				cout << "cat = " << cat << " predict = " << d << endl;
				free( x_space );

			}
}