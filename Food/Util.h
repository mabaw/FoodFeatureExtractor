#ifndef UTIL_H
#define UTIL_H






class Util
{
	public:
		void ConvertCHARToTCHAR( const char* str, TCHAR* wstr, UINT cchDest )
		{
			mbstowcs( wstr, str, strlen(str) + 1 );
		}


	private:
	
};

#endif