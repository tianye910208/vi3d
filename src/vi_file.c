#include "vi_file.h"
#include "vi_mem.h"


vi_file* vi_file_open(const char* filepath, const char* mode)
{
	vi_rawfile* fd = NULL;
	void*		ud = NULL;

#ifdef VI3D_SYS_ANDROID
	if(filepath[0] == '/')
	{
		ud = (void*)fopen(filepath, mode);
	}
	else
	{	
		ANativeActivity* activity = vi_sys_get_activity();
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
		vi_file* f = vi_mem_alloc(sizeof(vi_file));
		f->fd = fd;
		f->ud = ud;
		return f;
	}
	else
	{
		return NULL;
	}
}

void vi_file_close(vi_file* f)
{
	if (f)
	{
		if (f->fd)
		{
#ifdef VI3D_SYS_ANDROID
			AAsset_close(f->fd);
#else
			fclose(f->fd);
#endif
			f->fd = NULL;
		}

		vi_mem_free(f);
	}
}

int vi_file_read(vi_file* f, char* data, int n)
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

int vi_file_seek(vi_file* f, int offset, int origin)
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

int vi_file_size(vi_file* f)
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









