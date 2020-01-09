#pragma once

#include <nltools/Types.h>
#include <list>
#include <vector>
#include <sigc++/trackable.h>
#include <glibmm/refptr.h>
#include <giomm/filemonitor.h>

namespace FileTools
{
  class RecursiveDirectoryMonitor : public sigc::trackable
  {
   protected:
    using tFile = Glib::RefPtr<Gio::File>;
    using tCallBack = std::function<void(const tFile&, const tFile&, Gio::FileMonitorEvent)>;
    using tFileCallBack = std::function<void(const tFile&)>;
    using tFileList = std::list<tFile>;
    using tMonitor = Glib::RefPtr<Gio::FileMonitor>;

    tCallBack m_callBack;
    tFile m_rootFolder;
    std::vector<tMonitor> m_monitors;

   public:
    RecursiveDirectoryMonitor(const tFile& rootFolder, tCallBack callback);
    void rebuildDirectoryList();
    void addMonitor(const tFile& file);
    void onFileChanged(const tFile& oldFile, const tFile& newFile, Gio::FileMonitorEvent monitorEvent);
    void recurseDirectory(const tFile& start, tFileCallBack filter);
    tFileList getAllDirectorysInDirectory(const tFile& folder);
    tFileList getAllFilesInFolder(const tFile& folder);

   protected:
    tFile getFileFromFileInfo(const tFile& currentFolder, const Glib::RefPtr<Gio::FileInfo>& fileInfo);
  };
}
