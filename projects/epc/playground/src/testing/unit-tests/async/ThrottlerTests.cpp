#include <testing/TestHelper.h>
#include <nltools/threading/Throttler.h>
#include <thread>

#warning "I dont believe in this setup yet"
TEST_CASE("Throttler does not do callback if Object is already destroyed")
{
  int passes = 0;
  auto throttler = std::make_unique<Throttler>(std::chrono::milliseconds(10));
  throttler->doTask([&](){
    passes++;
  });

  WHEN("doMainLoopIteration is not called")
  {
    CHECK(passes == 0);
  }

  WHEN("throttler is deleted and doMainLoopIteration is called and not enough time has passed")
  {
    throttler.reset(nullptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    TestHelper::doMainLoopIteration();
    CHECK(passes == 0);
  }

  WHEN("throttler is deleted and doMainLoopIteration is called and enough time has passed")
  {
    throttler.reset(nullptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    TestHelper::doMainLoopIteration();
    CHECK(passes == 0);
  }
}