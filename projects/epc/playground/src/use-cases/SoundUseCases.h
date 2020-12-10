#pragma once

class EditBuffer;

class SoundUseCases
{
 public:
  explicit SoundUseCases(EditBuffer* eb);

 private:
  EditBuffer* m_editBuffer;
};