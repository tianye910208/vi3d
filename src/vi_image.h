#ifndef _VI3D_IMAGE_
#define _VI3D_IMAGE_

typedef struct _vi_image {
	int x;
	int y;
	int comp;
	int size;
	unsigned char* data;
} vi_image;



int vi_image_info(const char* filename, int *x, int *y, int* comp);
vi_image* vi_image_load(const char* filename);
void vi_image_free(vi_image *img);

#endif 


