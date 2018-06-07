#include "vi_msg.h"
#include "vi_log.h"

#define VI_MSG_MAXN 128

static vi_msg  __vi_msg_list[VI_MSG_MAXN] = {0};
static int	   __vi_msg_head = 0;
static int	   __vi_msg_tail = 0;


vi_msg* vi_msg_push(vi_msg_type type, int size) {
	int head = __vi_msg_head + 1;
	if (head >= VI_MSG_MAXN)
		head = 0;
	if (head == __vi_msg_tail)
		return NULL;

	vi_msg* m = __vi_msg_list + __vi_msg_head;
	__vi_msg_head = head;

	m->type = type;
	m->size = size;
	return m;
}

vi_msg* vi_msg_pull() {
	if (__vi_msg_head == __vi_msg_tail)
		return NULL;

	vi_msg* m = __vi_msg_list + __vi_msg_tail;

	__vi_msg_tail += 1;
	if (__vi_msg_tail >= VI_MSG_MAXN)
		__vi_msg_tail = 0;
	
	return m;
}





