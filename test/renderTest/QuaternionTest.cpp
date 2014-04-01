#include <Unittest++.h>
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
    CHECK_EQUAL(true, equals(axis.x, v.x, Math::EPSILON));
    CHECK_EQUAL(true, equals(axis.y, v.y, Math::EPSILON));
    CHECK_EQUAL(true, equals(axis.z, v.z, Math::EPSILON));
    CHECK_EQUAL(true, equals(theta, Math::PI, Math::EPSILON));
}

TEST(QuaternionSlerp)
{
    using namespace der;
    Quaternion q1, q2;
    q1.rotation_from_axis_angle(1.0f, 2.0f, 3.0f, Math::PI);
    q2.rotation_from_axis_angle(0.5f, 1.0f, 0.0f, Math::PI);

    Quaternion result = slerp(q1, q2, 0.0f);
    CHECK_EQUAL(true, q1.equals(result, Math::EPSILON));

    result = slerp(q1, q2, 0.0f);
    CHECK_EQUAL(false, q2.equals(result, Math::EPSILON));

    result = slerp(q1, q2, 1.0f);
    CHECK_EQUAL(true, q2.equals(result, Math::EPSILON));

    q1.rotation_from_axis_angle(1.0f, 0.0f, 0.0f, Math::PI);
    result = slerp(q1, Quaternion::identity, 0.5f);
    //CHECK_EQUAL(true, result.equals(, Math::EPSILON));
}

}
