#ifndef _VI3D_FILE_
#define _VI3D_FILE_

#include "vi_sys.h"

#ifdef VI3D_SYS_ANDROID
#include <android/asset_manager.h>
typedef AAsset rawfile;
#else
typedef FILE rawfile;
#endif

typedef struct _file
{
	rawfile* fd;
	void*	 ud;
} file;


file* file_open(const char* filepath, const char* mode);
int file_read(file* f, char* data, int n);
int file_seek(file* f, int offset, int origin);
int file_size(file* f);
int file_close(file* f);



#endif 


