#pragma once
#include <string>
#include <glibmm/ustring.h>

class Preset;
class Settings;
class EditBuffer;
class Bank;
class PresetManager;

namespace UNDO {
  class Transaction;
};

class PresetUseCases
{
 public:
  explicit PresetUseCases(Preset* p, Settings& settings);

  void rename(const std::string& newName);
  void setComment(const Glib::ustring& comment);
  void setAttribute(const Glib::ustring& key, const Glib::ustring& value);
  void overwriteWithEditBuffer(EditBuffer& editBuffer);
  void overwriteWithPreset(Preset* src);

 private:
  Preset* m_preset;
  Bank* m_bank;
  PresetManager* m_presetManager;
  EditBuffer* m_editBuffer;
  Settings& m_settings;
};
