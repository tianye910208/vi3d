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


static inline vec2 vec2_add(vec2 a, vec2 b) {
	vec2 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

static inline vec2 vec2_del(vec2 a, vec2 b) {
	vec2 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}

static inline vec2 vec2_mul(vec2 a, float k) {
	vec2 c;
	c.x = a.x * k;
	c.y = a.y * k;
	return c;
}

static inline float vec2_len(vec2 a) {
	return sqrtf(a.x * a.x + a.y * a.y);
}

static inline float vec2_dot(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

static inline vec2 vec2_normalize(vec2 a) {
	float k = 1.0f / vec2_len(a);
	return vec2_mul(a, k);
}



static inline vec3 vec3_add(vec3 a, vec3 b) {
	vec3 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}

static inline vec3 vec3_del(vec3 a, vec3 b) {
	vec3 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}

static inline vec3 vec3_mul(vec3 a, float k) {
	vec3 c;
	c.x = a.x * k;
	c.y = a.y * k;
	c.z = a.z * k;
	return c;
}

static inline float vec3_len(vec3 a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline float vec3_dot(vec3 a, vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline vec3 vec3_cross(vec3 a, vec3 b) {
	vec3 c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}

static inline vec3 vec3_normalize(vec3 a) {
	float k = 1.0f / vec3_len(a);
	return vec3_mul(a, k);
}


static inline vec4 vec4_add(vec4 a, vec4 b) {
	vec4 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;
	return c;
}

static inline vec4 vec4_del(vec4 a, vec4 b) {
	vec4 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
	return c;
}

static inline vec4 vec4_mul(vec4 a, float k) {
	vec4 c;
	c.x = a.x * k;
	c.y = a.y * k;
	c.z = a.z * k;
	c.w = a.w * k;
	return c;
}

static inline vec4 vec4_rot(vec4 a, vec4 b) {//quaternion_multiply
	vec4 c;
	c.x = a.y * b.z - a.z * b.y + b.x * a.w + a.x * b.w;
	c.y = a.z * b.x - a.x * b.z + b.y * a.w + a.y * b.w;
	c.z = a.x * b.y - a.y * b.x + b.z * a.w + a.z * b.w;
	c.w = a.w * b.w - (a.x * b.x + a.y * b.y + a.z * b.z);
	return c;
}

static inline float vec4_len(vec4 a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

static inline vec4 vec4_slerp(vec4 a, vec4 b, float k) {
	float cosTheta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	if (cosTheta < 0) {
		cosTheta = -cosTheta;
		b.x = -b.x; 
		b.y = -b.y;
		b.z = -b.z; 
		b.w = -b.w;
	}

	float s = 1 - k;
	if (1 - cosTheta > 0.001f) {
		float theta = acosf(cosTheta);
		float sinTheta = sinf(theta);
		s = sinf(s * theta) / sinTheta;
		k = sinf(k * theta) / sinTheta;
	}

	vec4 c;
	c.x = a.x * s + b.x * k;
	c.y = a.y * s + b.y * k;
	c.z = a.z * s + b.z * k;
	c.w = a.w * s + b.w * k;
	return c;
}

static inline vec4 vec4_inverted(vec4 a) {
	vec4 c;
	float len = a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
	if (len > 0) {
		float inv = -1.0f / len;
		c.x = a.x * inv;
		c.y = a.y * inv;
		c.z = a.z * inv;
		c.w = -a.w * inv;
	}
	else {
		c.x = c.y = c.z = c.w = 0;
	}
	return c;
}

static inline vec4 vec4_normalize(vec4 a) {
	float k = 1.0f / vec4_len(a);
	return vec4_mul(a, k);
}

static inline vec4 vec4_create_euler(vec3 euler) {
	vec4 roll	= { sinf(euler.x * 0.5f), 0, 0, cosf(euler.x * 0.5f) };
	vec4 pitch	= { 0, sinf(euler.y * 0.5f), 0, cosf(euler.y * 0.5f) };
	vec4 yaw	= { 0, 0, sinf(euler.z * 0.5f), cosf(euler.z * 0.5f) };

	// Order: y * x * z
	return vec4_rot(vec4_rot(pitch, roll), yaw);
}

static inline vec4 vec4_create_axis_angle(vec3 axis, float angle) {
	float t = sinf(angle * 0.5f);

	vec4 c;
	c.x = axis.x * t;
	c.y = axis.y * t;
	c.z = axis.z * t;
	c.w = cosf(angle * 0.5f);
	return c;
}

static inline vec3 mat4_mul_vec3(mat4 a, vec3 b) {
	vec3 c;

	return c;
}

static inline vec4 mat4_mul_vec4(mat4 a, vec4 b) {
	vec4 c;

	return c;
}

static inline mat4 mat4_mul_mat4(mat4 a, mat4 b) {
	mat4 c;
	return c;
}

static inline mat4 mat4_inverted(mat4 a) {
	mat4 c;
	return c;
}

static inline mat4 mat4_translate(mat4 a, vec3 b) {
	mat4 c;
	return c;
}

static inline mat4 mat4_rotate(mat4 a, vec4 b) {
	mat4 c;
	return c;
}

static inline mat4 mat4_scale(mat4 a, vec3 b) {
	mat4 c;
	return c;
}



static inline mat4 mat4_create_identity() {
	mat4 c;
	return c;
}

static inline mat4 mat4_create_transform(vec3 position, vec4 rotation, vec3 scale) {
	mat4 c;
	return c;
}

#endif 










