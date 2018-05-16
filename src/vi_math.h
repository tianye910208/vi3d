#ifndef _VI3D_MATH_
#define _VI3D_MATH_

#include "vi_sys.h"

typedef union vec2 {
	struct { float x, y; };
	struct { float u, v; };
	float p[2];
} vec2;

typedef union vec3 {
	struct { float x, y, z; };
	struct { float r, g, b; };
	float p[3];
} vec3;

typedef union vec4 {
	struct { float x, y, z, w; };
	struct { float u, v, s, t; };
	struct { float r, g, b, a; };
	float p[4];
} vec4;


typedef union mat4 {
	float m[4][4];
	float p[16];
} mat4;


inline vec2 vec2_add(vec2 a, vec2 b)
{
	return a;
}
inline vec2 vec2_del(vec2 a, vec2 b);
inline vec2 vec2_mul(vec2 a, float k);

inline float vec2_len(vec2 a);
inline vec2 vec2_dot(vec2 a, vec2 b);
inline vec2 vec2_normalize(vec2 a);



inline vec3 vec3_add(vec3 a, vec3 b);
inline vec3 vec3_del(vec3 a, vec3 b);
inline vec3 vec3_mul(vec3 a, float k);

inline float vec3_len(vec3 a);
inline vec3 vec3_dot(vec3 a, vec3 b);
inline vec3 vec3_cross(vec3 a, vec3 b);
inline vec3 vec3_normalize(vec3 a);


inline vec4 vec4_add(vec4 a, vec4 b);
inline vec4 vec4_del(vec4 a, vec4 b);
inline vec4 vec4_mul(vec4 a, float k);
inline vec4 vec4_rot(vec4 a, vec4 b);//quaternion_multiply

inline float vec4_len(vec4 a);
inline vec4 vec4_slerp(vec4 a, vec4 b, float k);
inline vec4 vec4_normalize(vec4 a);

inline vec4 vec4_create_euler(vec3 euler);
inline vec4 vec4_create_axis_angle(vec3 axis, float angle);




inline vec3 mat4_mul_vec3(mat4 a, vec3 b);
inline vec4 mat4_mul_vec4(mat4 a, vec4 b);
inline mat4 mat4_mul_mat4(mat4 a, mat4 b);

inline mat4 mat4_inverted(mat4 a);

inline mat4 mat4_translate(mat4 a, vec3 b);
inline mat4 mat4_rotate(mat4 a, vec4 b);
inline mat4 mat4_scale(mat4 a, vec3 b);



inline mat4 mat4_create_identity();
inline mat4 mat4_create_transform(vec3 position, vec4 rotation, vec3 scale);


#endif 










