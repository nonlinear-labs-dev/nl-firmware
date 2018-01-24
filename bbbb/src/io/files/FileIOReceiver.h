#pragma once

#include <io/Receiver.h>

class FileIOReceiver : public Receiver
{
  public:
    FileIOReceiver(const char *path, size_t blockSize);
    virtual ~FileIOReceiver();

  private:
    bool onChunk(Glib::IOCondition condition);

    void onFileOpened(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::File> buttonsFile);
    void readStream(Glib::RefPtr<Gio::FileInputStream> stream);
    void onButtonsFileRead(Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::FileInputStream> stream);

    Glib::RefPtr<Gio::Cancellable> m_cancel;
    size_t m_blockSize;
};

