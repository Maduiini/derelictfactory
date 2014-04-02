#include <UnitTest++.h>
#include <math/Math.h>

namespace
{

TEST(QuaternionAxisAngle)
{
    using namespace der;

    Vector3 axis(1.0f, 2.0f, 3.0f);
    axis.normalize();
    const float angle = Math::PI * 0.33f;

    Quaternion q;
    q.rotation_from_axis_angle(axis, angle);

    Vector3 axis2;
    float angle2;
    q.get_axis_angle(axis2, angle2);
    CHECK_EQUAL(axis, axis2);
    CHECK_CLOSE(angle, angle2, Math::EPSILON);
}


TEST(QuaternionSlerp)
{
    using namespace der;

    Quaternion q1, q2;
    q1.rotation_from_axis_angle(1.0f, 2.0f, 3.0f, Math::PI);
    q2.rotation_from_axis_angle(0.5f, 1.0f, 0.0f, Math::PI);

    Quaternion result = slerp(q1, q2, 0.0f);
    CHECK_EQUAL(q1, result);

    // As CHECK_EQUAL uses == operator, we can't check for inequality directly.
    // Also, the math primitives' == and != operators use epsilon comparison.
    result = slerp(q1, q2, 0.0f);
    CHECK_EQUAL(true, q2 != result);

    result = slerp(q1, q2, 1.0f);
    CHECK_EQUAL(q2, result);

    // Slerp between rotations around same angle.
    q1.rotation_from_axis_angle(1.0f, 0.0f, 0.0f, Math::PI / 4);
    q2.rotation_from_axis_angle(1.0f, 0.0f, 0.0f, 0.0f);
    result = slerp(q1, q2, 0.5f);

    Quaternion expected_q;
    expected_q.rotation_from_axis_angle(1.0f, 0.0f, 0.0f, (Math::PI / 4) * 0.5f);
    CHECK_EQUAL(expected_q, result);


    // Again, slerp between rotations around same angle.
    q1.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, Math::PI / 4);
    q2.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, Math::PI / 2);
    const float t = 0.58f;
    result = slerp(q1, q2, t);
    const float expected_angle = lerp(Math::PI / 4, Math::PI / 2, t);

    expected_q.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, expected_angle);
    CHECK_EQUAL(expected_q, result);


    Vector3 axis; float angle;
    result.get_axis_angle(axis, angle);
    CHECK_EQUAL(Vector3(0.0f, 1.0f, 0.0f), axis);
    CHECK_CLOSE(expected_angle, angle, Math::EPSILON);
}

}
