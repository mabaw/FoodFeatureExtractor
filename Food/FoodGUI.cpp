#include "FoodGUI.h"


BOOL CALLBACK MainDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	FoodGUI* mainDlg = (FoodGUI*)GetWindowLong( hDlg, GWL_USERDATA );
	return mainDlg->DlgProc( hDlg, message, wParam, lParam );
}


bool FoodGUI::Initialize(MainSetting *_mainSetting,FeatureData *_featureData)
{
	setting = _mainSetting;
	featureData = _featureData;
	InitDialog();
	ResetDialog();


	setting->Intialize();
	extracter.Intialize(setting);
	return true;
}

void FoodGUI::ResetDialog()
{

	UpdateText();
}


void FoodGUI::Update()
{
	//UpdateText();
}

bool FoodGUI::InitDialog()
{

	m_fInstance	= GetModuleHandle(NULL);
	m_fDlg		= CreateDialog(m_fInstance, MAKEINTRESOURCE(IDD_Fooddialog), NULL, (DLGPROC)MainDlgProc);
	if(!m_fDlg)
	{
		MessageBox(NULL, L"Failed To Initialize MainGUI.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

		Util u;
		char buff[30];	
		WCHAR wbuff[30];

		sprintf(buff,"-");				
		u.ConvertCHARToTCHAR(buff,wbuff,strlen(buff));
		SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_ADDSTRING, 0, (LPARAM)(wbuff) );


		sprintf(buff,"GIR");				
		u.ConvertCHARToTCHAR(buff,wbuff,strlen(buff));
		SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_ADDSTRING, 0, (LPARAM)(wbuff) );

		sprintf(buff,"Distance");		
		u.ConvertCHARToTCHAR(buff,wbuff,strlen(buff));
		SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_ADDSTRING, 0, (LPARAM)(wbuff) );
					
		sprintf(buff,"Orientation");		
		u.ConvertCHARToTCHAR(buff,wbuff,strlen(buff));
		SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_ADDSTRING, 0, (LPARAM)(wbuff) );
					
		sprintf(buff,"Midpoint");		
		u.ConvertCHARToTCHAR(buff,wbuff,strlen(buff));
		SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_ADDSTRING, 0, (LPARAM)(wbuff) );
		
		sprintf(buff,"OM");		
		u.ConvertCHARToTCHAR(buff,wbuff,strlen(buff));
		SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_ADDSTRING, 0, (LPARAM)(wbuff) );
		
		
		char buf[10];
		TCHAR tchar[10];
		_snprintf(buf, 10, "%d", setting->color);
		u.ConvertCHARToTCHAR(buf,tchar,10);
		SetDlgItemText(m_fDlg,IDC_nocolor,tchar);

		_snprintf(buf, 10, "%d", setting->ncat);
		u.ConvertCHARToTCHAR(buf,tchar,10);
		SetDlgItemText(m_fDlg,IDC_nocat,tchar);
		
		_snprintf(buf, 10, "%d", setting->nitem);
		u.ConvertCHARToTCHAR(buf,tchar,10);
		SetDlgItemText(m_fDlg,IDC_noitm,tchar);


	SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_SETCURSEL, 0 , 0 );
	int currSizeIdx = SendMessage( GetDlgItem(m_fDlg, IDC_featuretype), CB_GETCURSEL, 0 , 0 );



	SetWindowLong(m_fDlg, GWL_USERDATA, (LONG)this);
	ShowWindow(m_fDlg, SW_SHOW);

	return true;
}


BOOL CALLBACK FoodGUI::DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{

	
	OPENFILENAME ofn;		
	WCHAR szFileName[MAX_PATH] = L"";
	
	int wmId;
	int count = 0;
	float *tmp;
//	char buff[256];
	switch (message)
	{
	case WM_COMMAND:			
		wmId = LOWORD(wParam);
		switch(wmId)
		{
		    case IDC_browsetrain :		
				if( LoadDialog(IDC_trainfile) )
				{
					char *dst;
					WCHAR wname[MAX_PATH], *ptr;
					GetDlgItemText(m_fDlg, IDC_trainfile, wname, MAX_PATH);
					dst = featurefilename; ptr = wname;
					while(*ptr) *dst++ = *ptr++;
					*dst = 0;				
				}		
				break;

			case IDC_browsepredict :				
				if( LoadDialog(IDC_predictfile) )
				{
					char *dst;
					WCHAR wname[MAX_PATH], *ptr;
					GetDlgItemText(m_fDlg, IDC_predictfile, wname, MAX_PATH);
					dst = modelfilename; ptr = wname;
					while(*ptr) *dst++ = *ptr++;
					*dst = 0;				
				}		
				break;
			case IDC_kernel:
				count = 0;
				for(int cat=1;cat<=setting->ncat;cat++)
				{
					// while(extracter.LoadSTFResult(cat,item,&size))
					for(int item=1;item<=setting->nitem;item++)
					{
						extracter.LoadSTFResult(cat,item);
						printf("Load soft label %d_%d \n",cat,item);

						// do stm with label
			
						extracter.RandomPoint();

						if(setting->featureType == GIR)
						{
							tmp = extracter.GIRExtract();
							for(int i=0;i<COLOR;i++)
								featureData->GIRFeature[count][i] = tmp[i];
							//cvCopyHist(extracter.DistanceHisr
							//extracter.MidPointHist();
							//extracter.BetweenPairHist();
							//extracter.DistAndOrientHist();
							//extracter.OrientAndMidPointHist();
							
						}
						featureData->label[count] = cat;
						count++;
					}
				}
				
				{
					char savefilename[MAX_PATH];
					char *dst;
					WCHAR wname[MAX_PATH], *ptr;
					GetDlgItemText(m_fDlg, IDC_kernelfile, wname, MAX_PATH);
					dst = savefilename; ptr = wname;
					while(*ptr) *dst++ = *ptr++;
					*dst = 0;	

					extracter.KernelGIRExtract(featureData->GIRFeature,featureData->label,setting->ncat*setting->nitem,savefilename);
				}
				break;
			case IDC_predict :

				break;
			default : break;
		}
		break;
	case WM_KEYDOWN:										// Is A Key Being Held Down?
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);								// Send A Quit Message
		break;
	case WM_CLOSE:
		DestroyWindow(m_fDlg);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return false;
}



void FoodGUI::UpdateText(){
	char buf[10];
	TCHAR tchar[10];
	Util u;
	_snprintf(buf, 10, "%d", setting->color);
	u.ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_fDlg,IDC_nocolor,tchar);
	
	_snprintf(buf, 10, "%d", setting->ncat);
	u.ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_fDlg,IDC_nocat,tchar);
	
	_snprintf(buf, 10, "%d", setting->nitem);
	u.ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_fDlg,IDC_noitm,tchar);
}



bool FoodGUI::LoadDialog(int textItem)
{
	OPENFILENAME ofn;
	WCHAR szFileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"txt";
	ofn.hwndOwner = m_fDlg;
	if(GetOpenFileName(&ofn))
	{
		SetDlgItemText(m_fDlg, textItem, ofn.lpstrFile);
		return true;
	}
	return false;
}

