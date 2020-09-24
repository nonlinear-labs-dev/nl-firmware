#pragma once

namespace Gio
{
  class File;
  class FileInfo;
  class InputStream;
  class AsyncResult;
  class Cancellable;
  class DataInputStream;
  class DataOutputStream;
  class ZlibCompressor;
  class FileMonitor;
}

namespace Glib
{
  class Bytes;
  class IOChannel;
  class ustring;
  class MainContext;
  class MainLoop;

  template <class T_CppObject> class RefPtr;
}
