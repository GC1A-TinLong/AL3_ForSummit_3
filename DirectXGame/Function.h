#pragma once
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include <assert.h>
#include <cmath>
#include <algorithm>

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

struct Vector2Int {
	int x, y;
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};
struct Ray {
	Vector3 origin;
	Vector3 diff;
};
struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Plane {
	Vector3 normal;
	float distance;
};

struct Triangle {
	Vector3 vertics[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};

Vector3& operator+=(Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(float scalar, const Vector3& v);
Vector3 operator*(const Vector3& v, float scalar);

Vector2Int operator-(const Vector2Int& v1, const Vector2Int& v2);
float Length(const Vector2Int& v);
Vector2 Normalize(const Vector2Int& v);

float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
Vector3 Project(const Vector3& v1, const Vector3& v2);
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Inverse(const Matrix4x4& m);
Matrix4x4 Transpose(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Matrix4x4 MakeRotateXMatrix(const float& radian);
Matrix4x4 MakeRotateYMatrix(const float& radian);
Matrix4x4 MakeRotateZMatrix(const float& radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

bool IsCollideSphere(const Sphere& sphere1, const Sphere& sphere2);
Vector3 Perpendicular(const Vector3& vector);
bool IsCollideSpherePlane(const Sphere& sphere, const Plane& plane);
bool IsCollideLinePlane(const Segment& segment, const Plane& plane);
bool IsCollideTriangleLine(const Triangle& triangle, const Segment& segment);
bool IsCollideAABB(const AABB& a, const AABB& b);
bool IsCollideAABBSphere(const AABB& aabb, const Sphere& sphere);
bool IsCollideAABBSegment(const AABB& aabb, const Segment& segment);
bool IsCollideOBBSphere(OBB& obb, const Sphere& sphere, Matrix4x4& rotateMatrix);