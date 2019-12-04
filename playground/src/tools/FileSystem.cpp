#include <tools/FileSystem.h>

void FileSystem::deleteFolder(Glib::RefPtr<Gio::File> folder)
{
  Glib::RefPtr<Gio::FileEnumerator> enumerator = folder->enumerate_children();
  std::list<Glib::RefPtr<Gio::FileInfo>> toDelete;

  while(auto file = enumerator->next_file())
  {
    auto type = file->get_file_type();

    if(type == Gio::FILE_TYPE_SYMBOLIC_LINK || type == Gio::FILE_TYPE_REGULAR)
    {
      auto name = file->get_name();
      auto file = folder->get_child(name);
      file->remove();
    }
    else if(type == Gio::FILE_TYPE_DIRECTORY)
    {
      auto name = file->get_name();
      auto file = folder->get_child(name);
      deleteFolder(file);
      file->remove();
    }
  }
  folder->remove();
}

bool FileSystem::isNameAUUID(const Glib::ustring &name)
{
  auto fileName = name.substr(0, name.find("."));
  return fileName.size() == 36;
}
