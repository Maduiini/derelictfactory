#include <UnitTest++.h>
#include <math/Math.h>

namespace
{

/// Test for quaternion - axis-angle conversions
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



/// A test case for slerp of two quaternions.
/// The quaternions are given in axis-angle form, and the
/// expected result must be know before hand.
struct SlerpTestCase
{
    SlerpTestCase(const der::Vector3 &axis1, float a1,
                  const der::Vector3 &axis2, float a2,
                  float t_, const der::Quaternion &e)
        : q1(), q2()
        , t(t_), expected(e)
    {
        using namespace der;

        const Vector3 ax1 = axis1.normalized();
        const Vector3 ax2 = axis2.normalized();
        q1.rotation_from_axis_angle(ax1, a1);
        q2.rotation_from_axis_angle(ax2, a2);
    }

    der::Quaternion q1;
    der::Quaternion q2;
    float t;

    der::Quaternion expected;
};

/// An array of test cases
const SlerpTestCase g_tests[] = {
    SlerpTestCase(der::Vector3(0.1f, 1.0f, 0.1f), 30.0f * der::Math::DEG_2_RAD,
                  der::Vector3(0.1f, 1.0f, 0.1f), -30.0f * der::Math::DEG_2_RAD, 0.5f,
                  der::Quaternion(0.0f, 0.0f, 0.0f, 1.0f)),

    SlerpTestCase(der::Vector3(0.1f, 1.0f, 0.1f), 30.0f * der::Math::DEG_2_RAD,
                  der::Vector3(0.1f, 1.0f, 0.1f), 30.0f * der::Math::DEG_2_RAD, 0.122f,
                  der::Quaternion(0.0256f, 0.2562f, 0.0256f, 0.9659f)),

    SlerpTestCase(der::Vector3(1.0f, 0.0f, 0.0f), -12.0f * der::Math::DEG_2_RAD,
                  der::Vector3(0.0f, 1.0f, 0.0f), 8.0f * der::Math::DEG_2_RAD, 0.8f,
                  der::Quaternion(-0.0209f, 0.0558f, 0.0f, 0.9982f)),

    SlerpTestCase(der::Vector3(0.1f, 1.0f, 0.1f), 30.0f * der::Math::DEG_2_RAD,
                  der::Vector3(0.1f, -2.0f, 0.1f), 40.0f * der::Math::DEG_2_RAD, 0.683f,
                  der::Quaternion(0.0206f, -0.1550f, 0.0206f, 0.9874f)),

    SlerpTestCase(der::Vector3(0.1f, 1.0f, 0.1f), 30.0f * der::Math::DEG_2_RAD,
                  der::Vector3(0.0f, 1.0f, 2.0f), -8.0f * der::Math::DEG_2_RAD, 0.822f,
                  der::Quaternion(0.0046f, 0.0205f, -0.0469f, 0.9986f)),
};

/// Test for quaternion slerp
TEST(QuaternionSlerp)
{
    using namespace der;

    Quaternion q1, q2;
    q1.rotation_from_axis_angle(1.0f, 2.0f, 3.0f, Math::PI);
    q2.rotation_from_axis_angle(0.5f, 1.0f, 0.0f, Math::PI);

    Quaternion result = slerp(q1, q2, 0.0f);
    CHECK_EQUAL(q1, result);

    result = slerp(q1, q2, -0.26f);
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


    // Again, slerp between rotations around same axis.
    q1.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, Math::PI / 4);
    q2.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, Math::PI / 2);
    const float t = 0.58f;
    result = slerp(q1, q2, t);
    const float expected_angle = lerp(Math::PI / 4, Math::PI / 2, t);

    expected_q.rotation_from_axis_angle(0.0f, 1.0f, 0.0f, expected_angle);
    CHECK_EQUAL(expected_q, result);

    // Check the axis angle of the result
    Vector3 axis; float angle;
    result.get_axis_angle(axis, angle);
    CHECK_EQUAL(Vector3(0.0f, 1.0f, 0.0f), axis);
    CHECK_CLOSE(expected_angle, angle, Math::EPSILON);

    // Go through the test cases
    for (const SlerpTestCase &test : g_tests)
    {
        const Quaternion result = slerp(test.q1, test.q2, test.t);
        CHECK_EQUAL(test.expected, result);
    }
}

}
