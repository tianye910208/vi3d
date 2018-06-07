#ifndef _VI3D_MSG_
#define _VI3D_MSG_

#include "vi_math.h"

typedef enum _vi_msg_type {
	VI_MSG_KEY_DOWN,
	VI_MSG_KEY_UP,
	VI_MSG_TOUCH_DOWN,
	VI_MSG_TOUCH_UP,
	VI_MSG_TOUCH_MOVE
} vi_msg_type;

typedef struct _vi_msg {
	vi_msg_type type;
	int			size;
	int			data[4];
} vi_msg;


vi_msg* vi_msg_push(vi_msg_type type, int size);
vi_msg* vi_msg_pull();


#endif 


