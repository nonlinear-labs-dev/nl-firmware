#include <testing/TestHelper.h>
#include <parameters/names/ParameterDB.h>
#include "parameter_declarations.h"

TEST_CASE("Macro Labels are sortable A < B < C < D < E < F")
{
  auto& db = ParameterDB::get();
  auto mca = db.getLongName({C15::PID::MC_A, VoiceGroup::Global});
  auto mcb = db.getLongName({C15::PID::MC_B, VoiceGroup::Global});
  auto mcc = db.getLongName({C15::PID::MC_C, VoiceGroup::Global});
  auto mcd = db.getLongName({C15::PID::MC_D, VoiceGroup::Global});
  auto mce = db.getLongName({C15::PID::MC_E, VoiceGroup::Global});
  auto mcf = db.getLongName({C15::PID::MC_F, VoiceGroup::Global});

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
