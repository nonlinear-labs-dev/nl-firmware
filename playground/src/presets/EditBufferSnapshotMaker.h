#pragma once

#include "playground.h"
#include <libundo/undo/Scope.h>

class EditBuffer;
class Parameter;

class EditBufferSnapshotMaker : public sigc::trackable
{
 public:
  static EditBufferSnapshotMaker &get();

  void addSnapshotIfRequired(UNDO::Transaction *transaction);

 private:
  EditBufferSnapshotMaker();

  struct Record
  {
    Parameter *parameter;
    tControlPositionValue snapshotValue;
  };

  using tParams = std::vector<Record>;

  tParams collectDirtyParameters(EditBuffer *editBuffer);
  void addSnapshot(UNDO::Transaction *transaction, tParams &&params);
  void sortParametersByModulationFlow(EditBufferSnapshotMaker::tParams &params);

  int m_lastKnownUpdateID = 0;
};
