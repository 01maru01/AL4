#include "Quaternion.h"
#include <math.h>

Quaternion SetQuaternion(const Vector3D& v, float angle)
{
    Vector3D vec = v;
    vec.normalize();

    Quaternion ans;
    float rad = sinf(angle / 2);
    ans.x = v.x * rad;
    ans.y = v.y * rad;
    ans.z = v.z * rad;
    ans.w = cosf(angle / 2);

    Normalize(ans);
    return ans;
}

Quaternion Quaternion::SetQuaternion(Vector3D v)
{
    Quaternion r;
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    r.w = 0;

    Quaternion q = *this;
    r = CalcProduct(q, r);
    r = CalcProduct(r, q.GetConjugation());

    return r;
}

void Quaternion::SetVec(const Vector3D& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

Quaternion Quaternion::CalcProduct(const Quaternion& q, const Quaternion& r)
{
    Quaternion ans;
    ans.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
    ans.x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
    ans.y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
    ans.z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;

    return ans;
}

Quaternion Quaternion::GetIdentity()
{
    Quaternion ans;
    ans.x = 0;
    ans.y = 0;
    ans.z = 0;
    ans.w = 1;

    return ans;
}

Quaternion Quaternion::GetConjugation()
{
    Quaternion ans;
    ans.x = -x;
    ans.y = -y;
    ans.z = -z;
    ans.w = w;

    return ans;
}

void Quaternion::Normalize()
{
    float len = sqrtf(w * w + x * x + y * y + z * z);

    if (len != 0.0f)
    {
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }
}

Matrix Quaternion::GetRotMatrix()
{
    Matrix mat;
    //mat.m[0][0] = w * w + x * x - y * y - z * z;
    mat.m[0][0] = 1 - (2 * y * y) - (2 * z * z);
    mat.m[0][1] = (2 * x * y) + (2 * w * z);
    mat.m[0][2] = (2 * x * z) - (2 * w * y);

    mat.m[1][0] = (2 * x * y) - (2 * w * z);
    mat.m[1][1] = 1 - (2 * x * x) - (2 * z * z);
    mat.m[1][2] = (2 * y * z) + (2 * w * x);

    mat.m[2][0] = (2 * x * z) + (2 * w * y);
    mat.m[2][1] = (2 * y * z) - (2 * w * x);
    mat.m[2][2] = 1 - (2 * x * x) - (2 * y * y);
    //mat.m[2][2] = w * w - x * x - y * y + z * z;

    return mat;
}

Quaternion operator-(const Quaternion& q)
{
    return Quaternion(-q.w, -q.x, -q.y, -q.z);
}

Quaternion& Quaternion::operator/=(float s)
{
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

Quaternion& Quaternion::operator*=(float s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

Quaternion& Quaternion::operator+=(Quaternion q)
{
    x += q.x;
    y += q.y;
    z += q.z;
    w += q.w;
    return *this;
}

Quaternion& Quaternion::operator-=(Quaternion q)
{
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;
    return *this;
}

float Quaternion::Dot(const Quaternion& q)
{
    return x * q.x + y * q.y + z * q.z + w * q.w;
}

const Quaternion operator*(const Quaternion& q, float s)
{
    Quaternion temp(q);
    return temp *= s;
}

const Quaternion operator*(float s, const Quaternion& q)
{
    return q * s;
}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion temp(q1);
    return temp += q2;
}

const Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion ans = q1;
    return ans -= q2;
}

Quaternion Multiply(const Quaternion& q, const Quaternion& r)
{
    Quaternion ans;
    ans.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
    ans.x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
    ans.y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
    ans.z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;

    return ans;
}

Quaternion IdentityQuaternion()
{
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion Conjugate(const Quaternion& quaternion)
{
    Quaternion ans;
    ans.x = -quaternion.x;
    ans.y = -quaternion.y;
    ans.z = -quaternion.z;
    ans.w = quaternion.w;

    return ans;
}

float Norm(const Quaternion& quaternion)
{
    return sqrtf(quaternion.w * quaternion.w + quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
}

Quaternion Normalize(const Quaternion& quaternion)
{
    float len = Norm(quaternion);

    Quaternion ans = quaternion;
    if (len != 0.0f)
    {
        ans.x /= len;
        ans.y /= len;
        ans.z /= len;
        ans.w /= len;
    }

    return ans;
}

Quaternion Inverse(const Quaternion& quaternion)
{
    Quaternion ans = quaternion;
    float norm = Norm(ans);
    ans /= norm;
    ans /= norm;
    return ans;
}

Quaternion MakeAxisAngle(const Vector3D& axis, float angle)
{
    Quaternion ans;
    Vector3D axis_ = axis;
    axis_.normalize();

    float rad = sinf(angle / 2);
    ans.x = axis_.x * rad;
    ans.y = axis_.y * rad;
    ans.z = axis_.z * rad;
    ans.w = cosf(angle / 2);

    ans.Normalize();

    return ans;
}

Vector3D RotateVector(const Vector3D& vec, const Quaternion& quaternion)
{
    Quaternion vecQ;
    vecQ.SetVec(vec);
    Quaternion q = quaternion;
    q.Normalize();
    Quaternion qConjugate = q.GetConjugation();
    vecQ = Multiply(q, vecQ);
    vecQ = Multiply(vecQ, qConjugate);

    return vecQ.GetVec();
}

Matrix MakeRotateMatrix(const Quaternion& quaternion)
{
    Matrix mat;
    mat.m[0][0] = quaternion.w * quaternion.w + quaternion.x * quaternion.x - quaternion.y * quaternion.y - quaternion.z * quaternion.z;
    mat.m[0][0] = 1 - (2 * quaternion.y * quaternion.y) - (2 * quaternion.z * quaternion.z);
    mat.m[0][1] = (2 * quaternion.x * quaternion.y) + (2 * quaternion.w * quaternion.z);
    mat.m[0][2] = (2 * quaternion.x * quaternion.z) - (2 * quaternion.w * quaternion.y);

    mat.m[1][0] = (2 * quaternion.x * quaternion.y) - (2 * quaternion.w * quaternion.z);
    mat.m[1][1] = 1 - (2 * quaternion.x * quaternion.x) - (2 * quaternion.z * quaternion.z);
    mat.m[1][2] = (2 * quaternion.y * quaternion.z) + (2 * quaternion.w * quaternion.x);

    mat.m[2][0] = (2 * quaternion.x * quaternion.z) + (2 * quaternion.w * quaternion.y);
    mat.m[2][1] = (2 * quaternion.y * quaternion.z) - (2 * quaternion.w * quaternion.x);
    mat.m[2][2] = 1 - (2 * quaternion.x * quaternion.x) - (2 * quaternion.y * quaternion.y);
    mat.m[2][2] = quaternion.w * quaternion.w - quaternion.x * quaternion.x - quaternion.y * quaternion.y + quaternion.z * quaternion.z;

    return mat;
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
    Quaternion q0_ = q0;
    float dot = q0_.Dot(q1);

    if (dot < 0.0f) {
        q0_ = -q0;
        dot = -dot;
    }

    float scale0 = (1.0f - t);
    float scale1 = t;
    if (dot >= 1.0f - 0.001f) {
        return q0_ * (1.0f - t) + t * q1;
    }
    float theta = (float)acos(dot);

    scale0 = (float)(sin(theta * scale0)) / sinf(theta);
    scale1 = (float)(sin(theta * scale1)) / sinf(theta);

    return q0_ * scale0 + scale1 * q1;
}

Quaternion CameraSlerp(const Quaternion& q0, const Quaternion& q1, float t)
{
    Quaternion q0_ = q0;
    float dot = q0_.Dot(q1);

    float scale0 = (1.0f - t);
    float scale1 = t;
    if (dot >= 1.0f - 0.001f) {
        return q0_ * (1.0f - t) + t * q1;
    }
    float theta = (float)acos(dot);

    scale0 = (float)(sin(theta * scale0)) / sinf(theta);
    scale1 = (float)(sin(theta * scale1)) / sinf(theta);

    return q0_ * scale0 + scale1 * q1;
}



Quaternion DirectionToDirection(const Vector3D& u, const Vector3D& v)
{
    Vector3D vecU = u;

    float dot = vecU.dot(v);
    Vector3D cross = vecU.cross(v);
    cross.normalize();

    float theta = (float)acos(dot);

    return MakeAxisAngle(cross, theta);
}
