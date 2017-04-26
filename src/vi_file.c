#include "vi_file.h"
#include "vi_mem.h"


vi_file* vi_file_open(const char* filepath, const char* mode)
{
#ifdef VI3D_SYS_ANDROID
	if(filepath[0] == '/')
	{
		FILE* fd = (void*)fopen(filepath, mode);
		if (fd)
		{
			vi_file* f = vi_mem_alloc(sizeof(vi_file));
			f->fd = fd;
			f->ud = NULL;
			f->asset = NULL;
			return f;
		}
	}
	else
	{	
		ANativeActivity* activity = vi_sys_get_activity();
		if (activity && activity->assetManager)
		{
			AAsset* asset = AAssetManager_open(activity->assetManager, filepath, AASSET_MODE_STREAMING);
			if (asset)
			{
				vi_file* f = vi_mem_alloc(sizeof(vi_file));
				f->fd = NULL;
				f->ud = NULL;
				f->asset = asset;
				return f;
			}
		}
	}
#else
	FILE* fd = fopen(filepath, mode);
	if (fd)
	{
		vi_file* f = vi_mem_alloc(sizeof(vi_file));
		f->fd = fd;
		f->ud = NULL;
		return f;
	}
#endif

	return NULL;
	
}

void vi_file_close(vi_file* f)
{
	if (f)
	{
		if (f->fd)
		{
			fclose(f->fd);
			f->fd = NULL;
		}

#ifdef VI3D_SYS_ANDROID
		if(f->asset)
		{
			AAsset_close(f->asset);
			f->asset = NULL;
		}
#endif
		vi_mem_free(f);
	}
}

int vi_file_read(vi_file* f, char* data, int n)
{
	if (f)
	{
		if (f->fd)
			return (int)fread(data, 1, n, f->fd);

#ifdef VI3D_SYS_ANDROID
		if (f->asset)
			return AAsset_read(f->asset, data, n);
#endif
	}
	return 0;
}

int vi_file_seek(vi_file* f, int offset, int origin)
{
	if (f)
	{
		if (f->fd)
			return fseek(f->fd, offset, origin);

#ifdef VI3D_SYS_ANDROID
		if (f->asset)
			return AAsset_seek(f->asset, offset, origin);
#endif
	}
	return 1;
}

int vi_file_size(vi_file* f)
{
	if (f)
	{
		if (f->fd)
		{
			int size = 0;
			if (fseek(f->fd, 0, SEEK_END) == 0)
			{
				size = (int)ftell(f->fd);
				fseek(f->fd, 0, SEEK_SET);
			}
			return size;
		}
#ifdef VI3D_SYS_ANDROID
		if(f->asset)
			return AAsset_getLength(f->asset);
#endif
	}
	return 0;
}





