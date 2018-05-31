#include "vi_image.h"
#include "vi_file.h"
#include "vi_mem.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO

#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#define STBI_ONLY_TGA

#include "stb_image.h"

struct _cb_data{
	vi_file *file;
	int size;
	int read;
};

static int _cb_fread(void* user, char* data, int size) {
    struct _cb_data* p = (struct _cb_data*)user;
	int n = vi_file_read(p->file, data, size);
	p->read += n;
	return n;
}
static void _cb_fskip(void* user, int n) {
    struct _cb_data* p = (struct _cb_data*)user;
	vi_file_seek(p->file, n, SEEK_CUR);
	p->read += n;
}
static int _cb_feof(void* user) {
    struct _cb_data* p = (struct _cb_data*)user;
	return p->read >= p->size;
}

static stbi_io_callbacks _cb_func = {_cb_fread, _cb_fskip, _cb_feof};

int vi_image_info(const char* filename, int *x, int *y, int* comp) {
	vi_file *f = vi_file_open(filename, "rb");
	if (f == NULL)
		return 0;


	struct _cb_data p;
	p.file = f;
	p.size = vi_file_size(f);
	p.read = 0;

	int ret = stbi_info_from_callbacks(&_cb_func, &p, x, y, comp);
	
	vi_file_close(f);

	return ret;
}

vi_image* vi_image_load(const char* filename) {
	vi_file* f = vi_file_open(filename, "rb");
	if (f == NULL)
		return NULL;

	int size = vi_file_size(f);

#ifdef VI3D_SYS_WIN
	char *data = (char *)alloca(size);
#else
	char data[size];
#endif

	size = vi_file_read(f, data, size);
	
	int x, y, comp;
	unsigned char * p = stbi_load_from_memory(data, size, &x, &y, &comp, 0);
	if (p == NULL)
		return NULL;

	vi_image *img = (vi_image*)vi_mem_alloc(sizeof(vi_image));
	img->x = x;
	img->y = y;
	img->comp = comp;
	img->data = p;
	img->size = x * y * comp;
	
	return img;
}

void vi_image_free(vi_image *img) {
	if (img->data)
		stbi_image_free(img->data);
	vi_mem_free(img);
}















