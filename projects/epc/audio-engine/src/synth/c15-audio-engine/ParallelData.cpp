#include "ParallelData.h"
#include <nltools/logging/Log.h>
#include <glib.h>

void testSimdSize()
{
  g_assert((ParallelData<int, 0>::simdSize == 0));
  g_assert((ParallelData<int, 1>::simdSize == 1));
  g_assert((ParallelData<int, 2>::simdSize == 1));
  g_assert((ParallelData<int, 3>::simdSize == 1));
  g_assert((ParallelData<int, 4>::simdSize == 1));
  g_assert((ParallelData<int, 5>::simdSize == 2));
}

void testCompareOperator()
{
  ParallelData<float, 12> a;
  ParallelData<float, 12> b;

  using E = std::tuple<float, float, int>;
  E s[] = { { 30, 20, -1 }, { -10, -9.8, 0 }, { -10, -9.2, -1 }, { 0, 0, 0 },    { 0, 5, -1 },   { 30, 20, -1 },
            { 30, 20, -1 }, { 30, 20, -1 },   { 30, 20, -1 },    { 30, 20, -1 }, { 30, 20, -1 }, { 30, 20, -1 } };

  for(int i = 0; i < 12; i++)
  {
    a[i] = std::get<0>(s[i]);
    b[i] = std::get<1>(s[i]);
  }

  ParallelData<uint32_t, 12> c = std::abs(a - b) > 0.5f;

  for(int i = 0; i < 12; i++)
  {
    g_assert(c[i] == std::get<2>(s[i]));
  }
}

void testRound()
{
  ParallelData<float, 12> a;
  a[0] = -12;
  a[1] = -11.6;
  a[2] = -11.3;
  a[3] = -0.6;
  a[4] = -0.2;
  a[5] = 0;
  a[6] = 12;
  a[7] = 11.6;
  a[8] = 11.3;
  a[9] = 0.6;
  a[10] = 0.2;
  a[11] = 0;

  auto rounded = std::round<int>(a);
  g_assert(rounded[0] == -12);
  g_assert(rounded[1] == -12);
  g_assert(rounded[2] == -11);
  g_assert(rounded[3] == -1);
  g_assert(rounded[4] == 0);
  g_assert(rounded[5] == 0);
  g_assert(rounded[6] == 12);
  g_assert(rounded[7] == 12);
  g_assert(rounded[8] == 11);
  g_assert(rounded[9] == 1);
  g_assert(rounded[10] == 0);
  g_assert(rounded[11] == 0);
}

void testParallelData()
{
  testSimdSize();
  testCompareOperator();
  testRound();
}
