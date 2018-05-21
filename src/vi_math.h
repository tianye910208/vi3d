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
	struct { float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33; };
	float m[4][4];
	float p[16];
} mat4;


static inline vec2* vec2_add(vec2 *a, vec2 *b, vec2 *r) {
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	return r;
}

static inline vec2* vec2_del(vec2 *a, vec2 *b, vec2 *r) {
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	return r;
}

static inline vec2* vec2_mul(vec2 *v, float k, vec2 *r) {
	r->x = v->x * k;
	r->y = v->y * k;
	return r;
}

static inline float vec2_len(vec2 *v) {
	return sqrtf(v->x * v->x + v->y * v->y);
}

static inline float vec2_dot(vec2 *a, vec2 *b) {
	return a->x * b->x + a->y * b->y;
}

static inline vec2* vec2_normalize(vec2 *v, vec2* r) {
	float k = 1.0f / vec2_len(v);
	return vec2_mul(v, k, r);
}



static inline vec3* vec3_add(vec3 *a, vec3 *b, vec3 *r) {
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
	return r;
}

static inline vec3* vec3_del(vec3 *a, vec3 *b, vec3 *r) {
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
	return r;
}

static inline vec3* vec3_mul(vec3 *v, float k, vec3 *r) {
	r->x = v->x * k;
	r->y = v->y * k;
	r->z = v->z * k;
	return r;
}

static inline float vec3_len(vec3 *v) {
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

static inline float vec3_dot(vec3 *a, vec3 *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline vec3* vec3_cross(vec3 *a, vec3 *b, vec3 *r) {
	float x = a->y * b->z - a->z * b->y;
	float y = a->z * b->x - a->x * b->z;
	float z = a->x * b->y - a->y * b->x;
	r->x = x;
	r->y = y;
	r->z = z;
	return r;
}

static inline vec3* vec3_normalize(vec3 *v, vec3 *r) {
	float k = 1.0f / vec3_len(v);
	return vec3_mul(v, k, r);
}


static inline vec4* vec4_add(vec4 *a, vec4 *b, vec4 *r) {
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
	r->w = a->w + b->w;
	return r;
}

static inline vec4* vec4_del(vec4 *a, vec4 *b, vec4 *r) {
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
	r->w = a->w - b->w;
	return r;
}

static inline vec4* vec4_mul(vec4 *v, float k, vec4 *r) {
	r->x = v->x * k;
	r->y = v->y * k;
	r->z = v->z * k;
	r->w = v->w * k;
	return r;
}

static inline vec4* vec4_rot(vec4 *a, vec4 *b, vec4 *r) {//quaternion_multiply
	float x = a->y * b->z - a->z * b->y + b->x * a->w + a->x * b->w;
	float y = a->z * b->x - a->x * b->z + b->y * a->w + a->y * b->w;
	float z = a->x * b->y - a->y * b->x + b->z * a->w + a->z * b->w;
	float w = a->w * b->w - (a->x * b->x + a->y * b->y + a->z * b->z);
	r->x = x;
	r->y = y;
	r->z = z;
	r->w = w;
	return r;
}

static inline float vec4_len(vec4 *v) {
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

static inline vec4* vec4_slerp(vec4 *a, vec4 *b, float k, vec4 *r) {
	float s = 1 - k;
	float bx = b->x;
	float by = b->y;
	float bz = b->z;
	float bw = b->w;

	float cosTheta = a->x * bx + a->y * by + a->z * bz + a->w * bw;
	if (cosTheta < 0) {
		cosTheta = -cosTheta;
		bx = -bx; 
		by = -by;
		bz = -bz; 
		bw = -bw;
	}

	if (1 - cosTheta > 0.001f) {
		float theta = acosf(cosTheta);
		float sinTheta = sinf(theta);
		s = sinf(s * theta) / sinTheta;
		k = sinf(k * theta) / sinTheta;
	}

	r->x = a->x * s + bx * k;
	r->y = a->y * s + by * k;
	r->z = a->z * s + bz * k;
	r->w = a->w * s + bw * k;
	return r;
}

static inline vec4* vec4_inverse(vec4 *v, vec4 *r) {//for quaternion
	float len = v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
	if (len > 0) {
		float inv = -1.0f / len;
		r->x = v->x * inv;
		r->y = v->y * inv;
		r->z = v->z * inv;
		r->w = -v->w * inv;
	}
	else {
		r->x = r->y = r->z = r->w = 0;
	}
	return r;
}

static inline vec4* vec4_normalize(vec4 *v, vec4 *r) {
	float k = 1.0f / vec4_len(v);
	vec4_mul(v, k, r);
	return r;
}

static inline vec4* vec4_from_euler_angle(vec3 *euler, vec4 *r) {
	vec4 roll	= { sinf(euler->x * 0.5f), 0, 0, cosf(euler->x * 0.5f) };
	vec4 pitch	= { 0, sinf(euler->y * 0.5f), 0, cosf(euler->y * 0.5f) };
	vec4 yaw	= { 0, 0, sinf(euler->z * 0.5f), cosf(euler->z * 0.5f) };

	// Order: y * x * z
	vec4_rot(&pitch, &roll, r);
	vec4_rot(r, &yaw, r);
	return r;
}

static inline vec4* vec4_from_axis_angle(vec3 *axis, float angle, vec4 *r) {
	float t = sinf(angle * 0.5f);

	r->x = axis->x * t;
	r->y = axis->y * t;
	r->z = axis->z * t;
	r->w = cosf(angle * 0.5f);
	return r;
}



static inline vec3* mat4_mul_vec3(mat4 *m, vec3 *v, vec3 *r) {
	float x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + m->m30;
	float y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + m->m31;
	float z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + m->m32;
	r->x = x;
	r->y = y;
	r->z = z;
	return r;
}

static inline vec4* mat4_mul_vec4(mat4 *m, vec4 *v, vec4 *r) {
	r->x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + v->w * m->m30;
	r->y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + v->w * m->m31;
	r->z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + v->w * m->m32;
	r->w = v->x * m->m03 + v->y * m->m13 + v->z * m->m23 + v->w * m->m33;
	return r;
}

static inline mat4* mat4_mul_mat4(mat4 *a, mat4 *b, mat4 *r) {
	mat4 t;
	float *rp = t.p;
	float *ap = a->p;
	float *bp = b->p;

	
	rp[0] = ap[0] * bp[0] + ap[4] * bp[1] + ap[8] * bp[2] + ap[12] * bp[3];
	rp[1] = ap[1] * bp[0] + ap[5] * bp[1] + ap[9] * bp[2] + ap[13] * bp[3];
	rp[2] = ap[2] * bp[0] + ap[6] * bp[1] + ap[10] * bp[2] + ap[14] * bp[3];
	rp[3] = ap[3] * bp[0] + ap[7] * bp[1] + ap[11] * bp[2] + ap[15] * bp[3];

	rp[4] = ap[0] * bp[4] + ap[4] * bp[5] + ap[8] * bp[6] + ap[12] * bp[7];
	rp[5] = ap[1] * bp[4] + ap[5] * bp[5] + ap[9] * bp[6] + ap[13] * bp[7];
	rp[6] = ap[2] * bp[4] + ap[6] * bp[5] + ap[10] * bp[6] + ap[14] * bp[7];
	rp[7] = ap[3] * bp[4] + ap[7] * bp[5] + ap[11] * bp[6] + ap[15] * bp[7];

	rp[8] = ap[0] * bp[8] + ap[4] * bp[9] + ap[8] * bp[10] + ap[12] * bp[11];
	rp[9] = ap[1] * bp[8] + ap[5] * bp[9] + ap[9] * bp[10] + ap[13] * bp[11];
	rp[10] = ap[2] * bp[8] + ap[6] * bp[9] + ap[10] * bp[10] + ap[14] * bp[11];
	rp[11] = ap[3] * bp[8] + ap[7] * bp[9] + ap[11] * bp[10] + ap[15] * bp[11];

	rp[12] = ap[0] * bp[12] + ap[4] * bp[13] + ap[8] * bp[14] + ap[12] * bp[15];
	rp[13] = ap[1] * bp[12] + ap[5] * bp[13] + ap[9] * bp[14] + ap[13] * bp[15];
	rp[14] = ap[2] * bp[12] + ap[6] * bp[13] + ap[10] * bp[14] + ap[14] * bp[15];
	rp[15] = ap[3] * bp[12] + ap[7] * bp[13] + ap[11] * bp[14] + ap[15] * bp[15];

	*r = t;
	return r;
}

static inline mat4* mat4_inverse(mat4 *m, mat4 *r) { 
	float d = m->m03 * m->m12 * m->m21 * m->m30 - m->m02 * m->m13 * m->m21 * m->m30 - m->m03 * m->m11 * m->m22 * m->m30 + m->m01 * m->m13 * m->m22 * m->m30 +
			  m->m02 * m->m11 * m->m23 * m->m30 - m->m01 * m->m12 * m->m23 * m->m30 - m->m03 * m->m12 * m->m20 * m->m31 + m->m02 * m->m13 * m->m20 * m->m31 +
			  m->m03 * m->m10 * m->m22 * m->m31 - m->m00 * m->m13 * m->m22 * m->m31 - m->m02 * m->m10 * m->m23 * m->m31 + m->m00 * m->m12 * m->m23 * m->m31 +
			  m->m03 * m->m11 * m->m20 * m->m32 - m->m01 * m->m13 * m->m20 * m->m32 - m->m03 * m->m10 * m->m21 * m->m32 + m->m00 * m->m13 * m->m21 * m->m32 +
			  m->m01 * m->m10 * m->m23 * m->m32 - m->m00 * m->m11 * m->m23 * m->m32 - m->m02 * m->m11 * m->m20 * m->m33 + m->m01 * m->m12 * m->m20 * m->m33 +
			  m->m02 * m->m10 * m->m21 * m->m33 - m->m00 * m->m12 * m->m21 * m->m33 - m->m01 * m->m10 * m->m22 * m->m33 + m->m00 * m->m11 * m->m22 * m->m33;
	if (d == 0) {
		*r = *m;
		return r;
	}

	d = 1.0f / d;

	mat4 t;
	t.m00 = d * (m->m12 * m->m23 * m->m31 - m->m13 * m->m22 * m->m31 + m->m13 * m->m21 * m->m32 - m->m11 * m->m23 * m->m32 - m->m12 * m->m21 * m->m33 + m->m11 * m->m22 * m->m33);
	t.m01 = d * (m->m03 * m->m22 * m->m31 - m->m02 * m->m23 * m->m31 - m->m03 * m->m21 * m->m32 + m->m01 * m->m23 * m->m32 + m->m02 * m->m21 * m->m33 - m->m01 * m->m22 * m->m33);
	t.m02 = d * (m->m02 * m->m13 * m->m31 - m->m03 * m->m12 * m->m31 + m->m03 * m->m11 * m->m32 - m->m01 * m->m13 * m->m32 - m->m02 * m->m11 * m->m33 + m->m01 * m->m12 * m->m33);
	t.m03 = d * (m->m03 * m->m12 * m->m21 - m->m02 * m->m13 * m->m21 - m->m03 * m->m11 * m->m22 + m->m01 * m->m13 * m->m22 + m->m02 * m->m11 * m->m23 - m->m01 * m->m12 * m->m23);
	t.m10 = d * (m->m13 * m->m22 * m->m30 - m->m12 * m->m23 * m->m30 - m->m13 * m->m20 * m->m32 + m->m10 * m->m23 * m->m32 + m->m12 * m->m20 * m->m33 - m->m10 * m->m22 * m->m33);
	t.m11 = d * (m->m02 * m->m23 * m->m30 - m->m03 * m->m22 * m->m30 + m->m03 * m->m20 * m->m32 - m->m00 * m->m23 * m->m32 - m->m02 * m->m20 * m->m33 + m->m00 * m->m22 * m->m33);
	t.m12 = d * (m->m03 * m->m12 * m->m30 - m->m02 * m->m13 * m->m30 - m->m03 * m->m10 * m->m32 + m->m00 * m->m13 * m->m32 + m->m02 * m->m10 * m->m33 - m->m00 * m->m12 * m->m33);
	t.m13 = d * (m->m02 * m->m13 * m->m20 - m->m03 * m->m12 * m->m20 + m->m03 * m->m10 * m->m22 - m->m00 * m->m13 * m->m22 - m->m02 * m->m10 * m->m23 + m->m00 * m->m12 * m->m23);
	t.m20 = d * (m->m11 * m->m23 * m->m30 - m->m13 * m->m21 * m->m30 + m->m13 * m->m20 * m->m31 - m->m10 * m->m23 * m->m31 - m->m11 * m->m20 * m->m33 + m->m10 * m->m21 * m->m33);
	t.m21 = d * (m->m03 * m->m21 * m->m30 - m->m01 * m->m23 * m->m30 - m->m03 * m->m20 * m->m31 + m->m00 * m->m23 * m->m31 + m->m01 * m->m20 * m->m33 - m->m00 * m->m21 * m->m33);
	t.m22 = d * (m->m01 * m->m13 * m->m30 - m->m03 * m->m11 * m->m30 + m->m03 * m->m10 * m->m31 - m->m00 * m->m13 * m->m31 - m->m01 * m->m10 * m->m33 + m->m00 * m->m11 * m->m33);
	t.m23 = d * (m->m03 * m->m11 * m->m20 - m->m01 * m->m13 * m->m20 - m->m03 * m->m10 * m->m21 + m->m00 * m->m13 * m->m21 + m->m01 * m->m10 * m->m23 - m->m00 * m->m11 * m->m23);
	t.m30 = d * (m->m12 * m->m21 * m->m30 - m->m11 * m->m22 * m->m30 - m->m12 * m->m20 * m->m31 + m->m10 * m->m22 * m->m31 + m->m11 * m->m20 * m->m32 - m->m10 * m->m21 * m->m32);
	t.m31 = d * (m->m01 * m->m22 * m->m30 - m->m02 * m->m21 * m->m30 + m->m02 * m->m20 * m->m31 - m->m00 * m->m22 * m->m31 - m->m01 * m->m20 * m->m32 + m->m00 * m->m21 * m->m32);
	t.m32 = d * (m->m02 * m->m11 * m->m30 - m->m01 * m->m12 * m->m30 - m->m02 * m->m10 * m->m31 + m->m00 * m->m12 * m->m31 + m->m01 * m->m10 * m->m32 - m->m00 * m->m11 * m->m32);
	t.m33 = d * (m->m01 * m->m12 * m->m20 - m->m02 * m->m11 * m->m20 + m->m02 * m->m10 * m->m21 - m->m00 * m->m12 * m->m21 - m->m01 * m->m10 * m->m22 + m->m00 * m->m11 * m->m22);

	*r = t;
	return r;
}

static inline mat4* mat4_transpose(mat4 *m, mat4* r) {
	float t;

	r->m00 = m->m00;
	r->m11 = m->m11;
	r->m22 = m->m22;
	r->m33 = m->m33;

	t = m->m01;
	r->m01 = m->m10;
	r->m10 = t;

	t = m->m02;
	r->m02 = m->m20;
	r->m20 = t;

	t = m->m03;
	r->m03 = m->m30;
	r->m30 = t;

	t = m->m12;
	r->m12 = m->m21;
	r->m21 = t;

	t = m->m13;
	r->m13 = m->m31;
	r->m31 = t;

	t = m->m23;
	r->m23 = m->m32;
	r->m32 = t;

	return r;
}





static inline mat4* mat4_from_identity(mat4* r) {
	memset(r, 0, sizeof(mat4));
	r->m00 = 1;
	r->m11 = 1;
	r->m22 = 1;
	r->m33 = 1;
	return r;
}

static inline mat4* mat4_from_quaternion(vec4 *q, mat4 *r) {
	float x2 = q->x + q->x, y2 = q->y + q->y, z2 = q->z + q->z;
	float xx = q->x * x2, xy = q->x * y2, xz = q->x * z2;
	float yy = q->y * y2, yz = q->y * z2, zz = q->z * z2;
	float wx = q->w * x2, wy = q->w * y2, wz = q->w * z2;


	r->m00 = 1 - (yy + zz);
	r->m01 = xy + wz;
	r->m02 = xz - wy;
	r->m03 = 0;

	r->m10 = xy - wz;
	r->m11 = 1 - (xx + zz);
	r->m12 = yz + wx;
	r->m13 = 0;

	r->m20 = xz + wy;
	r->m21 = yz - wx;
	r->m22 = 1 - (xx + yy);
	r->m23 = 0;

	r->m30 = 0;
	r->m31 = 0;
	r->m32 = 0;
	r->m33 = 1;
	
	return r;
}

static inline mat4* mat4_from_perspective(float l, float r, float b, float t, float n, float f, mat4 *m) {
	m->m00 = 2 * n / (r - l);
	m->m01 = 0;
	m->m02 = 0;
	m->m03 = 0;

	m->m10 = 0;
	m->m11 = 2 * n / (t - b);
	m->m12 = 0;
	m->m13 = 0;

	m->m20 = (r + l) / (r - l);
	m->m21 = (t + b) / (t - b);
	m->m22 = -(f + n) / (f - n);
	m->m23 = -1;

	m->m30 = 0;
	m->m31 = 0;
	m->m32 = -2 * f * n / (f - n);
	m->m33 = 0;

	return m;
}

static inline mat4* mat4_from_orthographic(float l, float r, float b, float t, float n, float f, mat4 *m) {
	m->m00 = 2 / (r - l);
	m->m01 = 0;
	m->m02 = 0;
	m->m03 = 0;

	m->m10 = 0;
	m->m11 = 2 / (t - b);
	m->m12 = 0;
	m->m13 = 0;

	m->m20 = 0;
	m->m21 = 0;
	m->m22 = -2 / (f - n);
	m->m23 = 0;

	m->m30 = -(r + l) / (r - l);
	m->m31 = -(t + b) / (t - b);
	m->m32 = -(f + n) / (f - n);
	m->m33 = 1;

	return m;
}


static inline mat4* mat4_translate(mat4 *m, vec3 *v, mat4* r) {
	*r = *m;
	r->m30 += v->x;
	r->m31 += v->y;
	r->m32 += v->z;
	return r;
}

static inline mat4* mat4_rotate(mat4 *m, vec4 *q, mat4 *r) {
	mat4_from_quaternion(q, r);
	mat4_mul_mat4(r, m, r);
	return r;
}

static inline mat4* mat4_scale(mat4 *m, vec3 *v, mat4 *r) {
	*r = *m;
	r->m00 *= v->x;
	r->m10 *= v->x;
	r->m20 *= v->x;
	r->m01 *= v->y;
	r->m11 *= v->y;
	r->m21 *= v->y;
	r->m02 *= v->z;
	r->m12 *= v->z;
	r->m22 *= v->z;
	return r;
}





#endif 










