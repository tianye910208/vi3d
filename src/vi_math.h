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



static inline void vec2_add(vec2 *a, vec2 *b, vec2 *r) {
	r->x = a->x + b->x;
	r->y = a->y + b->y;
}

static inline void vec2_sub(vec2 *a, vec2 *b, vec2 *r) {
	r->x = a->x - b->x;
	r->y = a->y - b->y;
}

static inline void vec2_mul(vec2 *v, float k, vec2 *r) {
	r->x = v->x * k;
	r->y = v->y * k;
}

static inline float vec2_dot(vec2 *a, vec2 *b) {
	return a->x * b->x + a->y * b->y;
}

static inline float vec2_len(vec2 *v) {
	return sqrtf(v->x * v->x + v->y * v->y);
}

static inline void vec2_normalize(vec2 *v) {
	float k = 1.0f / sqrtf(v->x * v->x + v->y * v->y);
	v->x = v->x * k;
	v->y = v->y * k;
}



static inline void vec3_add(vec3 *a, vec3 *b, vec3 *r) {
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
}

static inline void vec3_sub(vec3 *a, vec3 *b, vec3 *r) {
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
}

static inline void vec3_mul(vec3 *v, float k, vec3 *r) {
	r->x = v->x * k;
	r->y = v->y * k;
	r->z = v->z * k;
}

static inline void vec3_mul_mat4(vec3 *v, mat4 *m, vec3 *r) {
	float x = v->x;
	float y = v->y;
	float z = v->z;
	r->x = x * m->m00 + y * m->m10 + z * m->m20 + m->m30;
	r->y = x * m->m01 + y * m->m11 + z * m->m21 + m->m31;
	r->z = x * m->m02 + y * m->m12 + z * m->m22 + m->m32;
}

static inline void vec3_cross(vec3 *a, vec3 *b, vec3 *r) {
	float x = a->y * b->z - a->z * b->y;
	float y = a->z * b->x - a->x * b->z;
	float z = a->x * b->y - a->y * b->x;
	r->x = x;
	r->y = y;
	r->z = z;
}

static inline float vec3_dot(vec3 *a, vec3 *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline float vec3_len(vec3 *v) {
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

static inline void vec3_normalize(vec3 *v) {
	float k = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x = v->x * k;
	v->y = v->y * k;
	v->z = v->z * k;
}




static inline void vec4_add(vec4 *a, vec4 *b, vec4 *r) {
	r->x = a->x + b->x;
	r->y = a->y + b->y;
	r->z = a->z + b->z;
	r->w = a->w + b->w;
}

static inline void vec4_sub(vec4 *a, vec4 *b, vec4 *r) {
	r->x = a->x - b->x;
	r->y = a->y - b->y;
	r->z = a->z - b->z;
	r->w = a->w - b->w;
}

static inline void vec4_mul(vec4 *v, float k, vec4 *r) {
	r->x = v->x * k;
	r->y = v->y * k;
	r->z = v->z * k;
	r->w = v->w * k;
}

static inline void vec4_mul_mat4(vec4 *v, mat4 *m, vec4 *r) {
	float x = v->x;
	float y = v->y;
	float z = v->z;
	float w = v->w;
	r->x = x * m->m00 + y * m->m10 + z * m->m20 + w * m->m30;
	r->y = x * m->m01 + y * m->m11 + z * m->m21 + w * m->m31;
	r->z = x * m->m02 + y * m->m12 + z * m->m22 + w * m->m32;
	r->w = x * m->m03 + y * m->m13 + z * m->m23 + w * m->m33;
}


static inline float vec4_len(vec4 *v) {
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

static inline void vec4_normalize(vec4 *v) {
	float k = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
	v->x = v->x * k;
	v->y = v->y * k;
	v->z = v->z * k;
	v->w = v->w * k;
}




static inline void vec4_quat_mul(vec4 *a, vec4 *b, vec4 *r) {
	float x = a->y * b->z - a->z * b->y + b->x * a->w + a->x * b->w;
	float y = a->z * b->x - a->x * b->z + b->y * a->w + a->y * b->w;
	float z = a->x * b->y - a->y * b->x + b->z * a->w + a->z * b->w;
	float w = a->w * b->w - (a->x * b->x + a->y * b->y + a->z * b->z);
	r->x = x;
	r->y = y;
	r->z = z;
	r->w = w;
}

static inline void vec4_quat_rot(vec4 *q, vec3 *v, vec3 *r) {
	vec3 uv, uuv;
	vec3_cross((vec3*)q, v, &uv);
	vec3_cross((vec3*)q, &uv, &uuv);

	vec3_mul(&uv, 2.0f * q->w, &uv);
	vec3_mul(&uuv, 2.0f, &uuv);

	vec3_add(v, &uv, r);
	vec3_add(r, &uuv, r);
}

static inline void vec4_quat_nlerp(vec4 *a, vec4 *b, float k, vec4 *r) {
	float cosTheta = a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;

	if (cosTheta < 0) {
		r->x = a->x + (-b->x - a->x) * k;
		r->y = a->y + (-b->y - a->y) * k;
		r->z = a->z + (-b->z - a->z) * k;
		r->w = a->w + (-b->w - a->w) * k;
	}
	else {
		r->x = a->x + (b->x - a->x) * k;
		r->y = a->y + (b->y - a->y) * k;
		r->z = a->z + (b->z - a->z) * k;
		r->w = a->w + (b->w - a->w) * k;
	}

	float invLen = 1.0f / sqrtf(r->x * r->x + r->y * r->y + r->z * r->z + r->w * r->w);

	r->x *= invLen;
	r->y *= invLen;
	r->z *= invLen;
	r->w *= invLen;
}

static inline void vec4_quat_slerp(vec4 *a, vec4 *b, float k, vec4 *r) {
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
}


static inline void vec4_set_quat_euler(vec4 *q, vec3 *euler) {
	vec4 roll = { sinf(euler->x * 0.5f), 0, 0, cosf(euler->x * 0.5f) };
	vec4 pitch = { 0, sinf(euler->y * 0.5f), 0, cosf(euler->y * 0.5f) };
	vec4 yaw = { 0, 0, sinf(euler->z * 0.5f), cosf(euler->z * 0.5f) };

	//y * x * z
	vec4_quat_mul(&pitch, &roll, q);
	vec4_quat_mul(q, &yaw, q);
}

static inline void vec4_set_quat_axis_angle(vec4 *q, vec3 *axis, float angle) {
	float t = sinf(angle * 0.5f);

	q->x = axis->x * t;
	q->y = axis->y * t;
	q->z = axis->z * t;
	q->w = cosf(angle * 0.5f);
}





static inline void mat4_mul_mat4(mat4 *a, mat4 *b, mat4 *r) {
	float *rp = r->p;
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
}


static inline void mat4_set_identity(mat4* m) {
	memset(m, 0, sizeof(mat4));
	m->m00 = 1;
	m->m11 = 1;
	m->m22 = 1;
	m->m33 = 1;
}

static inline void mat4_set_zero(mat4 *m) {
	memset(m, 0, sizeof(mat4));
}

static inline void mat4_set_quat(mat4 *m, vec4 *q) {
	float x2 = q->x + q->x, y2 = q->y + q->y, z2 = q->z + q->z;
	float xx = q->x * x2, xy = q->x * y2, xz = q->x * z2;
	float yy = q->y * y2, yz = q->y * z2, zz = q->z * z2;
	float wx = q->w * x2, wy = q->w * y2, wz = q->w * z2;


	m->m00 = 1 - (yy + zz);
	m->m01 = xy + wz;
	m->m02 = xz - wy;
	m->m03 = 0;

	m->m10 = xy - wz;
	m->m11 = 1 - (xx + zz);
	m->m12 = yz + wx;
	m->m13 = 0;

	m->m20 = xz + wy;
	m->m21 = yz - wx;
	m->m22 = 1 - (xx + yy);
	m->m23 = 0;

	m->m30 = 0;
	m->m31 = 0;
	m->m32 = 0;
	m->m33 = 1;
}


static inline void mat4_translate(mat4 *m, vec3 *v) {
	m->m30 += v->x;
	m->m31 += v->y;
	m->m32 += v->z;
}

static inline void mat4_rotate(mat4 *m, vec4 *q) {
	mat4 t;
	mat4_set_quat(&t, q);

	mat4 r;
	mat4_mul_mat4(&t, m, &r);

	*m = r;
}

static inline void mat4_scale(mat4 *m, vec3 *v) {
	m->m00 *= v->x;
	m->m10 *= v->x;
	m->m20 *= v->x;
	m->m01 *= v->y;
	m->m11 *= v->y;
	m->m21 *= v->y;
	m->m02 *= v->z;
	m->m12 *= v->z;
	m->m22 *= v->z;
}


static inline void mat4_inverse(mat4 *m) {
	float d = m->m03 * m->m12 * m->m21 * m->m30 - m->m02 * m->m13 * m->m21 * m->m30 - m->m03 * m->m11 * m->m22 * m->m30 + m->m01 * m->m13 * m->m22 * m->m30 +
		m->m02 * m->m11 * m->m23 * m->m30 - m->m01 * m->m12 * m->m23 * m->m30 - m->m03 * m->m12 * m->m20 * m->m31 + m->m02 * m->m13 * m->m20 * m->m31 +
		m->m03 * m->m10 * m->m22 * m->m31 - m->m00 * m->m13 * m->m22 * m->m31 - m->m02 * m->m10 * m->m23 * m->m31 + m->m00 * m->m12 * m->m23 * m->m31 +
		m->m03 * m->m11 * m->m20 * m->m32 - m->m01 * m->m13 * m->m20 * m->m32 - m->m03 * m->m10 * m->m21 * m->m32 + m->m00 * m->m13 * m->m21 * m->m32 +
		m->m01 * m->m10 * m->m23 * m->m32 - m->m00 * m->m11 * m->m23 * m->m32 - m->m02 * m->m11 * m->m20 * m->m33 + m->m01 * m->m12 * m->m20 * m->m33 +
		m->m02 * m->m10 * m->m21 * m->m33 - m->m00 * m->m12 * m->m21 * m->m33 - m->m01 * m->m10 * m->m22 * m->m33 + m->m00 * m->m11 * m->m22 * m->m33;

	if (d != 0) {
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

		*m = t;
	}
}

static inline void mat4_transpose(mat4 *m) {
	float t;

	t = m->m01; m->m01 = m->m10; m->m10 = t;
	t = m->m02; m->m02 = m->m20; m->m20 = t;
	t = m->m03; m->m03 = m->m30; m->m30 = t;
	t = m->m12; m->m12 = m->m21; m->m21 = t;
	t = m->m13; m->m13 = m->m31; m->m31 = t;
	t = m->m23; m->m23 = m->m32; m->m32 = t;
}

#endif 










