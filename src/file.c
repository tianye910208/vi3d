#include "file.h"
#include "mem.h"


file* file_open(const char* filepath, const char* mode)
{
	rawfile* fd = NULL;
	void* ud = NULL;

#ifdef VI3D_SYS_ANDROID
	if(filepath[0] == '/')
	{
		ud = (void*)fopen(filepath, mode);
	}
	else
	{	
		ANativeActivity* activity = sys_get_activity();
		if (activity && activity->assetManager)
		{
			fd = AAssetManager_open(activity->assetManager, filepath, AASSET_MODE_STREAMING);
		}
	}
#else
	fd = fopen(filepath, mode);
#endif

	if (fd || ud)
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

int file_read(file* f, char* data, int n)
{
	int read = 0;

	if (f)
	{
#ifdef VI3D_SYS_ANDROID
		if (f->fd)
			read = AAsset_read(f->fd, data, n);
		else if(f->ud)
			read = (int)fread(data, 1, n, (FILE*)f->ud);
#else
		if (f->fd)
			read = (int)fread(data, 1, n, f->fd);
#endif
	}
	return read;
}

int file_seek(file* f, int offset, int origin)
{
	if (f && f->fd)
	{
#ifdef VI3D_SYS_ANDROID
		return AAsset_seek(f->fd, offset, origin);
#else
		return fseek(f->fd, offset, origin);
#endif
	}
	return 1;
}

int file_size(file* f)
{
	if (f && f->fd)
	{
#ifdef VI3D_SYS_ANDROID
		return AAsset_getLength(f->fd);
#else
		int size = 0;
		if (fseek(f->fd, 0, SEEK_END) == 0)
		{
			size = ftell(f->fd);
			fseek(f->fd, 0, SEEK_SET);
		}
		return size;
#endif
	}
	return 0;
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








