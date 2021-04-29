#include <testing/TestHelper.h>
#include <libundo/undo/Scope.h>
#include <libundo/undo/Transaction.h>
#include <http/UpdateDocumentMaster.h>
#include <thread>

TEST_CASE("Timestamped Undo")
{
  using namespace std::chrono_literals;

  class UpdateDocumentMasterMock : public UpdateDocumentMaster
  {
    void writeDocument(Writer &writer, tUpdateID knownRevision) const override
    {
    }
  };

  UpdateDocumentMasterMock master;
  UNDO::Scope scope(&master);

  scope.startTransaction("a").reset();
  std::this_thread::sleep_for(1ms);
  scope.startTransaction("b").reset();
  std::this_thread::sleep_for(1ms);
  auto bTime = std::chrono::system_clock::now();
  scope.startTransaction("c").reset();
  std::this_thread::sleep_for(1ms);
  scope.startTransaction("d").reset();
  auto dTime = std::chrono::system_clock::now();
  std::this_thread::sleep_for(1ms);
  scope.startTransaction("e").reset();
  scope.undo();  // d
  scope.undo();  // c
  scope.startTransaction("c1").reset();
  auto c1Time = std::chrono::system_clock::now();
  scope.startTransaction("c2").reset();
  auto lastTime = std::chrono::system_clock::now();

  REQUIRE(scope.findTransactionAt(bTime)->getName() == "b");
  REQUIRE(scope.findTransactionAt(dTime)->getName() == "d");
  REQUIRE(scope.findTransactionAt(c1Time)->getName() == "c1");
  REQUIRE(scope.findTransactionAt(lastTime)->getName() == "c2");
}

TEST_CASE("Timestamped Undo - find after undo")
{
  using namespace std::chrono_literals;

  class UpdateDocumentMasterMock : public UpdateDocumentMaster
  {
    void writeDocument(Writer &writer, tUpdateID knownRevision) const override
    {
    }
  };

  UpdateDocumentMasterMock master;
  UNDO::Scope scope(&master);

  scope.startTransaction("a").reset();
  std::this_thread::sleep_for(1ms);
  scope.startTransaction("b").reset();
  std::this_thread::sleep_for(1ms);
  auto b1Time = std::chrono::system_clock::now();
  scope.startTransaction("c").reset();
  std::this_thread::sleep_for(1ms);
  scope.undo();  // b
  std::this_thread::sleep_for(1ms);
  auto b2Time = std::chrono::system_clock::now();
  scope.startTransaction("c").reset();
  std::this_thread::sleep_for(1ms);

  REQUIRE(scope.findTransactionAt(b1Time)->getName() == "b");
  REQUIRE(scope.findTransactionAt(b2Time)->getName() == "b");
}
