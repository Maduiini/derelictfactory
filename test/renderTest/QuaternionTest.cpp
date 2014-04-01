#include <UnitTest++.h>
#include <math/Math.h>

namespace
{

TEST(QuaternionAxisAngle)
{
    using namespace der;
    Quaternion q1;

    Vector3 v(1.0f, 2.0f, 3.0f);
    q1.rotation_from_axis_angle(v, Math::PI);

    Vector3 axis; float theta;
    q1.get_axis_angle(axis.x, axis.y, axis.z, theta);
    CHECK_EQUAL(true, axis.equals(v));
    CHECK_EQUAL(true, equals(theta, Math::PI));
}

/// Rounds off the last 3 significand bits.
/// Used to test approximate equality of two floats.
float approx(float x)
{
    int i = *reinterpret_cast<int*>(&x);
    i &= 0xfffffff8;
    float y = *reinterpret_cast<float*>(&i);
    return y;
}

TEST(QuaternionSlerp)
{
    using namespace der;
    Quaternion q1, q2;
    q1.rotation_from_axis_angle(1.0f, 2.0f, 3.0f, Math::PI);
    q2.rotation_from_axis_angle(0.5f, 1.0f, 0.0f, Math::PI);

    Quaternion result = slerp(q1, q2, 0.0f);
    CHECK_EQUAL(true, q1.equals(result));

    result = slerp(q1, q2, 0.0f);
    CHECK_EQUAL(false, q2.equals(result));

    result = slerp(q1, q2, 1.0f);
    CHECK_EQUAL(true, q2.equals(result));

    Quaternion expected_q;
    q1.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, Math::PI / 4);
    q2.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, 0.0f);
    result = slerp(q1, q2, 0.5f);
    expected_q.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, (Math::PI / 4) * 0.5f);
    CHECK_EQUAL(expected_q, result);

    q2.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, Math::PI / 2);
    const float t = 0.58f;
    result = slerp(q1, q2, t);
    const float expected_angle = lerp(Math::PI / 4, Math::PI / 2, t);

    expected_q.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, expected_angle);
    CHECK_EQUAL(expected_q, result);

    Vector3 axis; float angle;
    result.get_axis_angle(axis, angle);
    CHECK_EQUAL(Vector3(0.0f, 1.0f, 0.0f), axis);
    CHECK_EQUAL(approx(expected_angle), approx(angle));
}

}
