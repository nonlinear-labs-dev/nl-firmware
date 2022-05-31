#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "testing/TestHelper.h"
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE("PARAMETER VALUE ASSERT")
{
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();

  THEN("Stetting rough Value leds to correct Assert")
  {

    for(auto v : { 0.2, 0.0, 0.1, 0.17635456, 0.1235425, 0.1234525643, 0.8563, 0.99, 1.0, -1.0 })
      eb->forEachParameter([&](Parameter* p) {
        p->setCPFromHwui(transaction, v);
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, v);
      });
  }
}

TEST_CASE("unique_ptr can be access if referenced before creation")
{
  struct Foo {
    void bar() {
      m_foo++;
    }

    int m_foo = 0;
  };
  
  std::unique_ptr<Foo> m_ptrContainer{nullptr};
  auto& reference = m_ptrContainer;

  WHEN("Foo is not created")
  {
    CHECK(m_ptrContainer.get() == nullptr);
    CHECK(reference.get() == nullptr);
  }

  WHEN("Foo is created")
  {
    m_ptrContainer = std::make_unique<Foo>();

    CHECK(m_ptrContainer.get() != nullptr);
    CHECK(reference.get() != nullptr);
    CHECK(reference.get() == m_ptrContainer.get());
  }
}
