
#ifndef H_DER_DEBUG_H
#define H_DER_DEBUG_H

#ifdef DER_DEBUG
#define DER_ASSERT(x) if (!(x)) der::assert(#x, __FILE__, __LINE__)
#define DER_ASSERT_EQ(x, y) if (!((x) == (y))) der::assert(#x " == " #y, __FILE__, __LINE__)
#else
#define DER_ASSERT(x)
#define DER_ASSERT_EQ(x, y)
#endif // DER_DEBUG

namespace der
{

    void assert(const char * const msg, const char * const file, int line);

} // der

#endif // H_DER_DEBUG_H

