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

    q1.rotation_from_axis_angle(1.0f, 0.0f, 0.0f, Math::PI);
    result = slerp(q1, Quaternion::identity, 0.5f);
    //CHECK_EQUAL(true, result.equals( ... ));
}

}
