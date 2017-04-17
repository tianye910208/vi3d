#include "vi_app.h"
#include "vi_mem.h"
#include "vi_log.h"
#include "vi_lua.h"
#include "vi_file.h"

static vi_app* __app_instance = NULL;

vi_app* vi_app_info()
{
	return __app_instance;
}

float vi_app_time()
{
	return __app_instance->time;
}


int vi_app_init(const char* datapath, const char* savepath)
{
	__app_instance = vi_mem_alloc(sizeof(vi_app));
	memset(__app_instance, 0, sizeof(vi_app));

	char* data_path = __app_instance->data_path;
	strcpy(data_path, datapath);
	int data_path_len = (int)strlen(data_path);
	if (data_path_len > 0 && data_path[data_path_len - 1] != '/')
	{
		data_path[data_path_len] = '/';
		data_path[data_path_len+1] = '\0';
	}

	char* save_path = __app_instance->save_path;
	strcpy(save_path, savepath);
	int save_path_len = (int)strlen(save_path);
	if (save_path_len > 0 && save_path[save_path_len - 1] != '/')
	{
		save_path[save_path_len] = '/';
		save_path[save_path_len + 1] = '\0';
	}


	vi_lua_init();

	char luafile[256];
	memset(luafile, 0, sizeof(luafile));
	strcpy(luafile, __app_instance->data_path);
	strcat(luafile, "lua/main.lua");
	vi_file* f = vi_file_open(luafile, "r");
	if (!f)
	{
		vi_log("[E]vi_app_init open file failed: %s", luafile);
		return 1;
	}

	int size = vi_file_size(f);
	char *data = vi_mem_alloc(size + 1);
	data[size] = '\0';
	vi_file_read(f, data, size);
	vi_file_close(f);
	vi_lua_exec((const char*)data);
	vi_mem_free(data);
	return 0;
}

void vi_app_exit()
{
	vi_lua_exit();
	if (__app_instance)
	{
		vi_mem_free(__app_instance);
		__app_instance = NULL;
	}
}


void vi_app_loop(float dt)
{
	__app_instance->time += dt;
	vi_lua_update(dt);
	vi_lua_render(dt);
}

void _vi_app_adjust_viewport()
{
	if (__app_instance->design_h <= 0 || __app_instance->design_w <= 0)
		return;

	float sw = (float)__app_instance->screen_w;
	float sh = (float)__app_instance->screen_h;
	float dw = sh / __app_instance->design_h * __app_instance->design_w;

	__app_instance->viewport_x = (int)((sw - dw) / 2);
	__app_instance->viewport_y = 0;
	__app_instance->viewport_w = (int)dw;
	__app_instance->viewport_h = (int)sh;
}

void vi_app_set_screen_size(int w, int h)
{
	if (__app_instance)
	{
		__app_instance->screen_w = w;
		__app_instance->screen_h = h;
		_vi_app_adjust_viewport();
	}
}

void vi_app_set_design_size(int w, int h)
{
	if (__app_instance)
	{
		__app_instance->design_w = w;
		__app_instance->design_h = h;
		_vi_app_adjust_viewport();
	}
}














