#ifndef _VI3D_APP_
#define _VI3D_APP_

typedef struct _vi_app {
	float time;

	int screen_w;
	int screen_h;
	
	int design_w;
	int design_h;

	int viewport_x;
	int viewport_y;
	int viewport_w;
	int viewport_h;

	char data_path[256];
	char save_path[256];
} vi_app;

vi_app* vi_app_info();
float vi_app_time();

int vi_app_init(const char* datapath, const char* savepath);
int vi_app_main();
int vi_app_exit();

void vi_app_loop(float dt);

void vi_app_set_screen_size(int w, int h);
void vi_app_set_design_size(int w, int h);




#endif 


