#include <testing/TestHelper.h>
#include <nltools/system/SerializedAsyncCommands.h>

using namespace std::chrono_literals;

constexpr auto c_testFileDir = "/tmp/SerializedAsyncCommands-tests";

TEST_CASE("SerializedAsyncCommands: all commands are executed")
{
  SerializedAsyncCommands commands;

  system(nltools::string::concat("rm -rf ", c_testFileDir).c_str());
  system(nltools::string::concat("mkdir ", c_testFileDir).c_str());

  std::vector<std::string> files;
  for(int i = 0; i < 10; i++)
    files.push_back(nltools::string::concat(c_testFileDir, "/file-", i));

  int counter = 0;

  for(auto &file : files)
    commands.schedule({ "touch", file }, [&](auto) { counter++; }, [](auto) { REQUIRE(false); });

  TestHelper::doMainLoop(0s, 1s, [&] { return counter == 10; });

  for(auto &file : files)
    CHECK(std::filesystem::exists(file));
}

TEST_CASE("SerializedAsyncCommands: all commands are ordered")
{
  SerializedAsyncCommands commands;

  system(nltools::string::concat("rm -rf ", c_testFileDir).c_str());
  system(nltools::string::concat("mkdir ", c_testFileDir).c_str());

  auto testFile = nltools::string::concat(c_testFileDir, "/", "test-file");
  int counter = 0;

  for(int i = 0; i < 10; i++)
  {
    commands.schedule({ "sh", "-c", nltools::string::concat("echo ", i, " > ", testFile) }, [&](auto) {},
                      [](auto) { REQUIRE(false); });
    commands.schedule({ "cat", testFile },
                      [&, i = i](auto res) {
                        REQUIRE(i == std::stoi(res));
                        counter++;
                      },
                      [](auto) { CHECK(false); });
  }

  TestHelper::doMainLoop(0s, 1s, [&] { return counter == 10; });
}

TEST_CASE("SerializedAsyncCommands: operation resumes after finishing first block")
{
  SerializedAsyncCommands commands;
  int counter = 0;

  for(int i = 0; i < 10; i++)
    commands.schedule({ "echo" }, [&](auto) { counter++; }, [](auto) { CHECK(false); });

  TestHelper::doMainLoop(0s, 1s, [&] { return counter == 10; });

  for(int i = 0; i < 10; i++)
    commands.schedule({ "echo" }, [&](auto) { counter++; }, [](auto) { CHECK(false); });

  TestHelper::doMainLoop(0s, 1s, [&] { return counter == 20; });
}
