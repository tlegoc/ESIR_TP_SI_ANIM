#include <System/Path.h>

// Conditional compilation (differences between Windows and Linux)

#ifdef WIN32

#include <windows.h>

::std::string System::Path::executable()
{
	char result[ MAX_PATH ];
	std::string res( result, GetModuleFileNameA( NULL, result, MAX_PATH ) );
	int index = res.find_last_of('\\') ;
	return res.substr(0, index) ;
}

#else

#include <limits.h>
#include <unistd.h>

::std::string System::Path::executable()
{
	char result[ PATH_MAX ];
	ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
	std::string res = result ;
	int index = res.find_last_of('/') ;
	return res.substr(0,index) ;
	//return std::string( result, (count > 0) ? count : 0 );
}

#endif
