
#include "Math.h"
#include "../Log.h"

namespace der
{

    bool equals(float a, float b)
    {
        return equals(a, b, Math::EPSILON);
    }

    bool equals(float a, float b, float epsilon)
    {
        const float d = a - b;
        return -epsilon <= d && d <= epsilon;
    }

} // der
