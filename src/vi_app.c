#include "vi_app.h"
#include "vi_mem.h"
#include "vi_lua.h"

#include "test.h"

static vi_app* __app_instance = NULL;

vi_app* vi_app_info()
{
	return __app_instance;
}

int vi_app_init()
{
	__app_instance = vi_mem_alloc(sizeof(vi_app));
	memset(__app_instance, 0, sizeof(vi_app));

	vi_lua_init();
	test_init();
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
	test_draw();
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

void vi_app_screen_size(int w, int h)
{
	if (__app_instance)
	{
		__app_instance->screen_w = w;
		__app_instance->screen_h = h;
		_vi_app_adjust_viewport();
	}
}

void vi_app_design_size(int w, int h)
{
	if (__app_instance)
	{
		__app_instance->design_w = w;
		__app_instance->design_h = h;
		_vi_app_adjust_viewport();
	}
}









