#ifndef _VI3D_APP_
#define _VI3D_APP_

#include "vi_sys.h"

typedef struct _vi_app
{
	float time;

	int screen_w;
	int screen_h;
	
	int design_w;
	int design_h;

	int viewport_x;
	int viewport_y;
	int viewport_w;
	int viewport_h;
} vi_app;

vi_app* vi_app_info();

int vi_app_init();
void vi_app_exit();
void vi_app_loop(float dt);

void vi_app_screen_size(int w, int h);
void vi_app_design_size(int w, int h);


#endif 


