#include <serialization/RecallEditBufferSerializer.h>
#include <xml/FileOutStream.h>
#include <presets/EditBuffer.h>
#include <catch.hpp>
#include "testing/TestHelper.h"

TEST_CASE("Recall write and Read")
{
  auto eb = TestHelper::getEditBuffer();

  RecallEditBufferSerializer serializer(eb);

  auto ebHashBefore = eb->getHash();

  SECTION("a")
  {
    auto fos = std::make_unique<FileOutStream>("/tmp/recalldata.xml", false);
    auto fosPtr = fos.get();
    XmlWriter writer { std::move(fos) };
    serializer.write(writer);
    fosPtr->commit();

    {
      auto fis = std::make_shared<FileInStream>("/tmp/recalldata.xml", false);
      auto scope = TestHelper::createTestScope();
      XmlReader reader { *fis.get(), scope->getTransaction() };
      serializer.read(reader);
    }

    auto ebAfter = eb->getHash();
    REQUIRE(ebHashBefore == ebAfter);
  }
}
