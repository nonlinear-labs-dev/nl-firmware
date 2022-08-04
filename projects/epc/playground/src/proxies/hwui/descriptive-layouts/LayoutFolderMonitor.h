#pragma once

#include <tools/Signal.h>
#include <tools/RecursiveDirectoryMonitor.h>

class LayoutFolderMonitor : public sigc::trackable
{
 public:
  LayoutFolderMonitor();

  void start();

  void bruteForce();
  sigc::connection onChange(std::function<void()> cb);

 private:
  void onFileChanged(const Glib::RefPtr<Gio::File> &, const Glib::RefPtr<Gio::File> &, int);
  Signal<void> m_onChange;
  Glib::RefPtr<Gio::File> m_rootFolder;
  FileTools::RecursiveDirectoryMonitor m_recMonitor;
};
