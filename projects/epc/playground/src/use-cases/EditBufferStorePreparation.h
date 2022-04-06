#pragma once
#include <vector>

class EditBuffer;

class EditBufferStorePreparation
{
 public:
  explicit EditBufferStorePreparation(EditBuffer& eb);
  ~EditBufferStorePreparation();

 private:
  EditBuffer& m_eb;
  std::vector<double> m_oldPositions;
};