

#ifndef MAIN_SETTING_H
#define MAIN_SETTING_H

#define MAXPATH 255

#include <string>
#include <windows.h>
#include <iostream>

using namespace std;

#define COLOR 7
#define NCAT 6
#define NITEM 12

enum {
		NOTCHOOSE = 0,
		GIR,
		DIST,
		ORIENT,
		MIDPOINT,
		OM
	};
	

class MainSetting {



	public :
		char STFDir[MAXPATH];
		int color,ncat,nitem;
		int featureType;

		bool Intialize()
		{
			color = COLOR;
			ncat = NCAT;
			nitem = NITEM;
			featureType = NOTCHOOSE;
			FILE *fp = fopen("directory.txt","r");
			if(fp!=0)
			{
				fgets(STFDir,MAXPATH,fp);
				fclose(fp);
				return true;
			}

			
			return false;
		}

	private :
};

#endif
