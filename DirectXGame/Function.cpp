#include "Function.h"

Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 operator*(float scalar, const Vector3& v) {
	Vector3 result{};
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

Vector3 operator*(const Vector3& v, float scalar) {
	Vector3 result{};
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

Vector2Int operator-(const Vector2Int& v1, const Vector2Int& v2) { return {v1.x - v2.x, v1.y - v2.y}; }

float Length(const Vector2Int& v) { return sqrtf(float(v.x * v.x + v.y * v.y)); }

Vector2 Normalize(const Vector2Int& v) {
	assert(Length(v));
	return {
	    v.x / Length(v),
	    v.y / Length(v),
	};
}

float Dot(const Vector3& v1, const Vector3& v2) { return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

float Length(const Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

Vector3 Normalize(const Vector3& v) {
	assert(Length(v));
	return {v.x / Length(v), v.y / Length(v), v.z / Length(v)};
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) { return {(v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x)}; }

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 normalV2 = Normalize(v2);
	return {
	    (v1.x * v2.x * normalV2.x) / Length(v2),
	    (v1.y * v2.y * normalV2.y) / Length(v2),
	    (v1.z * v2.z * normalV2.z) / Length(v2),
	};
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) { return point + segment.diff; }

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result{};
	float det = (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) -
	            (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) - (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -
	            (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) +
	            (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) + (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) +
	            (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) -
	            (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) - (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -
	            (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) - (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) +
	            (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) + (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	result.m[0][0] = ((m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[1][3] * m.m[2][1] * m.m[3][2]) - (m.m[1][3] * m.m[2][2] * m.m[3][1]) -
	                  (m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2])) /
	                 det;
	result.m[0][1] = (-(m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[2][1] * m.m[3][2]) + (m.m[0][3] * m.m[2][2] * m.m[3][1]) +
	                  (m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2])) /
	                 det;
	result.m[0][2] = ((m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[3][2]) - (m.m[0][3] * m.m[1][2] * m.m[3][1]) -
	                  (m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2])) /
	                 det;
	result.m[0][3] = (-(m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) - (m.m[0][3] * m.m[1][1] * m.m[2][2]) + (m.m[0][3] * m.m[1][2] * m.m[2][1]) +
	                  (m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2])) /
	                 det;

	result.m[1][0] = (-(m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[1][3] * m.m[2][0] * m.m[3][2]) + (m.m[1][3] * m.m[2][2] * m.m[3][0]) +
	                  (m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2])) /
	                 det;
	result.m[1][1] = ((m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) + (m.m[0][3] * m.m[2][0] * m.m[3][2]) - (m.m[0][3] * m.m[2][2] * m.m[3][0]) -
	                  (m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2])) /
	                 det;
	result.m[1][2] = (-(m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) - (m.m[0][3] * m.m[1][0] * m.m[3][2]) + (m.m[0][3] * m.m[1][2] * m.m[3][0]) +
	                  (m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2])) /
	                 det;
	result.m[1][3] = ((m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) + (m.m[0][3] * m.m[1][0] * m.m[2][2]) - (m.m[0][3] * m.m[1][2] * m.m[2][0]) -
	                  (m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2])) /
	                 det;

	result.m[2][0] = ((m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[1][3] * m.m[2][0] * m.m[3][1]) - (m.m[1][3] * m.m[2][1] * m.m[3][0]) -
	                  (m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1])) /
	                 det;
	result.m[2][1] = (-(m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) - (m.m[0][3] * m.m[2][0] * m.m[3][1]) + (m.m[0][3] * m.m[2][1] * m.m[3][0]) +
	                  (m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1])) /
	                 det;
	result.m[2][2] = ((m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) + (m.m[0][3] * m.m[1][0] * m.m[3][1]) - (m.m[0][3] * m.m[1][1] * m.m[3][0]) -
	                  (m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1])) /
	                 det;
	result.m[2][3] = (-(m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) - (m.m[0][3] * m.m[1][0] * m.m[2][1]) + (m.m[0][3] * m.m[1][1] * m.m[2][0]) +
	                  (m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1])) /
	                 det;

	result.m[3][0] = (-(m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) - (m.m[1][2] * m.m[2][0] * m.m[3][1]) + (m.m[1][2] * m.m[2][1] * m.m[3][0]) +
	                  (m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1])) /
	                 det;
	result.m[3][1] = ((m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) + (m.m[0][2] * m.m[2][0] * m.m[3][1]) - (m.m[0][2] * m.m[2][1] * m.m[3][0]) -
	                  (m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1])) /
	                 det;
	result.m[3][2] = (-(m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) - (m.m[0][2] * m.m[1][0] * m.m[3][1]) + (m.m[0][2] * m.m[1][1] * m.m[3][0]) +
	                  (m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1])) /
	                 det;
	result.m[3][3] = ((m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) + (m.m[0][2] * m.m[1][0] * m.m[2][1]) - (m.m[0][2] * m.m[1][1] * m.m[2][0]) -
	                  (m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1])) /
	                 det;

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;
	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float  w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 MakeRotateXMatrix(const float& radian) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = cosf(radian);
	result.m[1][2] = sinf(radian);
	result.m[2][1] = -sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 MakeRotateYMatrix(const float& radian) {
	Matrix4x4 result{};
	result.m[0][0] = cosf(radian);
	result.m[0][2] = -sinf(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 MakeRotateZMatrix(const float& radian) {
	Matrix4x4 result{};
	result.m[0][0] = cosf(radian);
	result.m[0][1] = sinf(radian);
	result.m[1][0] = -sinf(radian);
	result.m[1][1] = cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeTranslateMatrix(translate);
	Matrix4x4 rotateXmatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYmatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZmatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZmatrix = rotateXmatrix * rotateYmatrix * rotateZmatrix;
	result.m[0][0] = scale.x * rotateXYZmatrix.m[0][0];
	result.m[0][1] = scale.x * rotateXYZmatrix.m[0][1];
	result.m[0][2] = scale.x * rotateXYZmatrix.m[0][2];
	result.m[1][0] = scale.y * rotateXYZmatrix.m[1][0];
	result.m[1][1] = scale.y * rotateXYZmatrix.m[1][1];
	result.m[1][2] = scale.y * rotateXYZmatrix.m[1][2];
	result.m[2][0] = scale.z * rotateXYZmatrix.m[2][0];
	result.m[2][1] = scale.z * rotateXYZmatrix.m[2][1];
	result.m[2][2] = scale.z * rotateXYZmatrix.m[2][2];
	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result{};
	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result{};
	assert(nearClip != 0);
	result.m[0][0] = (1 / tanf(fovY / 2)) / aspectRatio;
	result.m[1][1] = 1 / tanf(fovY / 2);
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = -(nearClip * farClip) / (farClip - nearClip);
	return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -(height / 2.0f);
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2.0f);
	result.m[3][1] = top + (height / 2.0f);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;
	return result;
}

bool IsCollideSphere(const Sphere& sphere1, const Sphere& sphere2) {
	float distance = Length(sphere2.center - sphere1.center);
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	}
	return false;
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return {-vector.y, vector.x, 0.0f};
	}
	return {0.0f, -vector.z, vector.y};
}

bool IsCollideSpherePlane(const Sphere& sphere, const Plane& plane) {
	float distance = (Dot(plane.normal, sphere.center) - plane.distance);
	if (fabsf(distance) - sphere.radius <= 0) {
		return true;
	}
	return false;
}

bool IsCollideLinePlane(const Segment& segment, const Plane& plane) {
	float dot = Dot(plane.normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	} // when perpendicular -> never colliding

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	if (t >= 0 && t <= 1.0f) {
		return true;
	}
	return false;
}

bool IsCollideTriangleLine(const Triangle& triangle, const Segment& segment) {
	// cross(vector1,vector2)↓
	Vector3 normal = Normalize(Cross((triangle.vertics[1] - triangle.vertics[0]), (triangle.vertics[2] - triangle.vertics[1])));
	float dot = Dot(normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	} // when perpendicular -> never colliding
	float t = (Dot(triangle.vertics[0], normal) - Dot(segment.origin, normal)) / dot;

	Vector3 p = segment.origin + t * segment.diff;
	Vector3 v01 = triangle.vertics[1] - triangle.vertics[0];
	Vector3 v12 = triangle.vertics[2] - triangle.vertics[1];
	Vector3 v20 = triangle.vertics[0] - triangle.vertics[2];
	Vector3 v0p = p - triangle.vertics[0];
	Vector3 v1p = p - triangle.vertics[1];
	Vector3 v2p = p - triangle.vertics[2];

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	if (Dot(cross01, normal) >= 0.0f && Dot(cross12, normal) >= 0.0f && Dot(cross20, normal) >= 0.0f) {
		return true;
	}
	return false;
}

bool IsCollideAABB(const AABB& a, const AABB& b) {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) && 
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
	    (a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	} else {
		return false;
	}
}

bool IsCollideAABBSphere(const AABB& aabb, const Sphere& sphere) {
	Vector3 closestPoint{std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)};
	float distance = Length(closestPoint - sphere.center);

	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool IsCollideAABBSegment(const AABB& aabb, const Segment& segment) {
	float txMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float txMax = (aabb.min.x + aabb.max.x - segment.origin.x) / segment.diff.x;
	float tyMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tyMax = (aabb.min.y + aabb.max.y - segment.origin.y) / segment.diff.y;
	float tzMin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tzMax = (aabb.min.z + aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = min(txMin, txMax);
	float tNearY = min(tyMin, tyMax);
	float tNearZ = min(tzMin, tzMax);
	float tFarX = max(txMin, txMax);
	float tFarY = max(tyMin, tyMax);
	float tFarZ = max(tzMin, tzMax);

	// setting it to infinite for however small/big it is, it still able to track down the number (closest/furthest)
	float tmin = -std::numeric_limits<float>::infinity();
	float tmax = +std::numeric_limits<float>::infinity();

	if (segment.diff.x != 0) {
		tmin = max(tmin, tNearX);
		tmax = min(tmax, tFarX);
	} else {
		if (segment.origin.x < aabb.min.x || segment.origin.x > aabb.min.x + aabb.max.x) {
			return false;
		}
	}
	if (segment.diff.y != 0) {
		tmin = max(tmin, tNearY);
		tmax = min(tmax, tFarY);
	} else {
		if (segment.origin.y < aabb.min.y || segment.origin.y > aabb.min.y + aabb.max.y) {
			return false;
		}
	}
	if (segment.diff.z != 0) {
		tmin = max(tmin, tNearZ);
		tmax = min(tmax, tFarZ);
	} else {
		if (segment.origin.z < aabb.min.z || segment.origin.z > aabb.min.z + aabb.max.z) {
			return false;
		}
	}

	return tmin <= tmax && tmax >= 0;
}

bool IsCollideOBBSphere(OBB& obb, const Sphere& sphere, Matrix4x4& rotateMatrix) {
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];

	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];

	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	Matrix4x4 obbWorldMatrix = {
	   obb.orientations[0].x,obb.orientations[0].y,obb.orientations[0].z,0,
		obb.orientations[1].x,obb.orientations[1].y,obb.orientations[1].z,0,
		obb.orientations[2].x,obb.orientations[2].y,obb.orientations[2].z,0,
		0,0,0,1.0f
	};
	Matrix4x4 inverseObbWorldMatrix = Transpose(obbWorldMatrix);

	Vector3 centerInObbLocalSpace = Transform(sphere.center, inverseObbWorldMatrix);

	AABB aabbObbLocal = {obb.size * -1.0f, aabbObbLocal.min + obb.size};
	Sphere sphereObbLocal = {centerInObbLocalSpace, sphere.radius};

	return IsCollideAABBSphere(aabbObbLocal, sphereObbLocal);
}