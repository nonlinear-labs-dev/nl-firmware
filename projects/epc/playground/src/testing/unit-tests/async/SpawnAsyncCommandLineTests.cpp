#include <testing/TestHelper.h>
#include <nltools/system/AsyncCommandLine.h>
#include <thread>
#include <nltools/system/SpawnAsyncCommandLine.h>

void cleanPendingJobs(Application* app) {
  TestHelper::doMainLoop(
      std::chrono::milliseconds(20), std::chrono::seconds(5), [] { return SpawnAsyncCommandLine::removeDone() == 0; },
      app->getMainContext());
  REQUIRE(SpawnAsyncCommandLine::getNumCommands() == 0);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Async Command Line does not block")
{
  cleanPendingJobs(app.get());

  bool blocked = true;
  bool done = false;

  AsyncCommandLine cmd(
      app->getMainContext(), { "sleep", "1" },
      [&](auto o) {
        CHECK_FALSE(blocked);
        done = true;
      },
      [](auto e) {});

  blocked = false;

  TestHelper::doMainLoop(
      std::chrono::milliseconds { 10 }, std::chrono::milliseconds { 2000 }, [&] { return done; },
      app->getMainContext());
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Async Completion will mark job as done")
{
  cleanPendingJobs(app.get());

  auto done = false;
  SpawnAsyncCommandLine::spawn(
      app->getMainContext(), { "sleep", "1" }, [&](auto) { done = true; }, [](auto) {});
  TestHelper::doMainLoop(
      std::chrono::milliseconds { 10 }, std::chrono::milliseconds { 2000 }, [&] { return done; },
      app->getMainContext());
  CHECK(SpawnAsyncCommandLine::removeDone() == 0);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Async Spawn increments job count")
{
  cleanPendingJobs(app.get());

  auto done = false;

  auto old = SpawnAsyncCommandLine::getNumCommands();

  SpawnAsyncCommandLine::spawn(
      app->getMainContext(), { "sleep", "1" }, [](auto) {}, [](auto) {});
  SpawnAsyncCommandLine::spawn(
      app->getMainContext(), { "sleep", "1" }, [&](auto) { done = true; }, [](auto) {});

  auto newCount = SpawnAsyncCommandLine::getNumCommands();
  CHECK(newCount > old);

  TestHelper::doMainLoop(
      std::chrono::milliseconds { 10 }, std::chrono::milliseconds { 2000 }, [&] { return done; },
      app->getMainContext());
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Async Spawn will remove done Jobs")
{
  cleanPendingJobs(app.get());

  auto done = false;

  SpawnAsyncCommandLine::spawn(
      app->getMainContext(), { "echo", "x" }, [&](auto) { done = true; }, [](auto) {});
  CHECK(SpawnAsyncCommandLine::getNumCommands() == 1);

  TestHelper::doMainLoop(
      std::chrono::milliseconds(200), std::chrono::milliseconds(2000), [&] { return done; },
      app->getMainContext());

  SpawnAsyncCommandLine::spawn(
      app->getMainContext(), { "echo", "x" }, [](auto) {}, [](auto) {});
  CHECK(SpawnAsyncCommandLine::getNumCommands() == 1);
}
