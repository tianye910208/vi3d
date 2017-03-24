#include "file.h"
#include "mem.h"


file* file_open(const char* filename, const char* mode)
{
	rawfile* fd = NULL;
	void* ud = NULL;

#ifdef VI3D_SYS_ANDROID
	ANativeActivity* activity = sys_get_activity();
	if (activity && activity->assetManager)
	{
		AAssetManager *assetManager = (AAssetManager *)activity->assetManager;
		fd = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);
	}
#else
	fd = fopen(filename, mode);
#endif

	if (fd)
	{
		file* f = mem_alloc(sizeof(file));
		f->fd = fd;
		f->ud = ud;
		return f;
	}
	else
	{
		return NULL;
	}
}


int file_close(file* f)
{
	if (f && f->fd)
	{
#ifdef VI3D_SYS_ANDROID
		AAsset_close(f->fd);	
#else
		fclose(f->fd);
#endif
		f->fd = NULL;
	}
	if (f)
	{
		mem_free(f);
	}
	return 0;
}


int file_read(file* f, char* data, int n)
{
	int read = 0;

	if (f && f->fd)
	{
#ifdef VI3D_SYS_ANDROID
		read = AAsset_read(f->fd, data, n);
#else
		read = (int)fread(data, n, 1, f->fd);
#endif
	}
	return read;
}









