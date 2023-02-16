#include <testing/TestHelper.h>
#include <parameters/names/ParameterDB.h>
#include "parameter_declarations.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "NL chars are sorted properly")
{
  auto chars
      = u8"\uE000\uE001\uE002\uE003\uE200\uE201\uE071\uE072\uE081\uE082\uE0BA\uE0BB\uE0C0\uE0C1\uE0CA\uE0F0\uE0F1\uE0F2"
        u8"\uE0F3\uE0F4\uE0F5\uE20A\uE20B\uE20C\uE20D\uE20E\uE20F\uE210\uE300\uE301\uE302\uE303\uE30E";

  auto numChars = g_utf8_strlen(chars, -1);
  CHECK(numChars == 33);

  for(int i = 0; i < numChars; i++)
  {
    auto left = g_utf8_offset_to_pointer(chars, i);

    Glib::ustring leftStr(left, left + 4);

    for(int j = i; j < numChars; j++)
    {
      auto right = g_utf8_offset_to_pointer(chars, j);
      Glib::ustring rightStr(right, right + 4);

      if(i == j)
      {
        CHECK(leftStr == rightStr);
      }
      else
      {
        CHECK(leftStr < rightStr);
        CHECK(leftStr != rightStr);
        CHECK(rightStr > leftStr);
      }
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Macro Labels are sortable A < B < C < D < E < F")
{
  auto eb = TestHelper::getEditBuffer();
  ParameterDB db(*eb);
  auto mca = db.getLongName({ C15::PID::MC_A, VoiceGroup::Global });
  auto mcb = db.getLongName({ C15::PID::MC_B, VoiceGroup::Global });
  auto mcc = db.getLongName({ C15::PID::MC_C, VoiceGroup::Global });
  auto mcd = db.getLongName({ C15::PID::MC_D, VoiceGroup::Global });
  auto mce = db.getLongName({ C15::PID::MC_E, VoiceGroup::Global });
  auto mcf = db.getLongName({ C15::PID::MC_F, VoiceGroup::Global });

  CHECK(mca == mca);
  CHECK(mcb == mcb);
  CHECK(mcc == mcc);
  CHECK(mcd == mcd);
  CHECK(mce == mce);
  CHECK(mcf == mcf);

  CHECK(mca != mcb);
  CHECK(mca != mcc);
  CHECK(mca != mcd);
  CHECK(mca != mce);
  CHECK(mca != mcf);

  CHECK(mcb != mca);
  CHECK(mcb != mcc);
  CHECK(mcb != mcd);
  CHECK(mcb != mce);
  CHECK(mcb != mcf);

  CHECK(mcc != mca);
  CHECK(mcc != mcb);
  CHECK(mcc != mcd);
  CHECK(mcc != mce);
  CHECK(mcc != mcf);

  CHECK(mcd != mca);
  CHECK(mcd != mcb);
  CHECK(mcd != mcc);
  CHECK(mcd != mce);
  CHECK(mcd != mcf);

  CHECK(mce != mca);
  CHECK(mce != mcb);
  CHECK(mce != mcc);
  CHECK(mce != mcd);
  CHECK(mce != mcf);

  CHECK(mcf != mca);
  CHECK(mcf != mcb);
  CHECK(mcf != mcc);
  CHECK(mcf != mcd);
  CHECK(mcf != mce);

  CHECK(mca < mcb);
  CHECK(mcb < mcc);
  CHECK(mcc < mcd);
  CHECK(mcd < mce);
  CHECK(mce < mcf);

  CHECK(mcf > mca);
}
