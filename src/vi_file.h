#ifndef _VI3D_FILE_
#define _VI3D_FILE_

#include "vi_sys.h"

#ifdef VI3D_SYS_ANDROID
#include <android/asset_manager.h>
typedef AAsset vi_rawfile;
#else
typedef FILE vi_rawfile;
#endif

typedef struct _vi_file
{
	vi_rawfile* fd;
	void*		ud;
} vi_file;


vi_file* vi_file_open(const char* filepath, const char* mode);
void vi_file_close(vi_file* f);

int vi_file_read(vi_file* f, char* data, int n);
int vi_file_seek(vi_file* f, int offset, int origin);
int vi_file_size(vi_file* f);




#endif 


