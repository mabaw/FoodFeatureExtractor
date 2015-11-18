#ifndef FOOD_GUI_H
#define FOOD_GUI_H

// globle
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include "resource.h"

#include <iostream>

#include "Util.h"
#include "MainSetting.h"
#include "FeatureExtracter.h"
#include "FeatureData.h"
using namespace std;

class FoodGUI
{
	public:
		bool Initialize(MainSetting *_mainSetting,FeatureData *_featureData);
		void Update();

		BOOL CALLBACK DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
	
	private:
		HWND		m_fDlg;
		HINSTANCE	m_fInstance;
		MainSetting	*setting;
		FeatureExtracter extracter;
		FeatureData	*featureData;
		char		featurefilename[MAX_PATH];
		char		modelfilename[MAX_PATH];
		bool LoadDialog(int textItem);
		void ResetDialog();
		bool InitDialog();
		void UpdateText();
};

#endif