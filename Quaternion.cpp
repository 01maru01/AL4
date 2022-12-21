#include "Quaternion.h"
#include <math.h>

void Quaternion::SetQuaternion(Vector3D v, float angle)
{
    v.normalize();

    float rad = sinf(angle / 2);
    x = v.x * rad;
    y = v.y * rad;
    z = v.z * rad;
    w = cosf(angle / 2);

    Normalize();
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
