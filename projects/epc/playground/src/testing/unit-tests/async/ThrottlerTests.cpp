#include <testing/TestHelper.h>
#include <nltools/threading/Throttler.h>
#include <thread>

TEST_CASE("Throttler does not do callback if Object is already destroyed")
{
  auto ctx = Glib::MainContext::create();
  auto ctxPtr = ctx.get();
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
    TestHelper::doMainLoopIterationOnContext(ctxPtr);
    CHECK(passes == 0);
  }

  WHEN("throttler is deleted and doMainLoopIteration is called and enough time has passed")
  {
    throttler.reset(nullptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    TestHelper::doMainLoopIterationOnContext(ctxPtr);
    CHECK(passes == 0);
  }
}