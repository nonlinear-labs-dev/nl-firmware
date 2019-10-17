#pragma once
#include <glibmm/ustring.h>
#include <nltools/StringTools.h>

namespace nltools
{
    inline Glib::ustring handle_eptr(std::exception_ptr eptr)
    {
      try
      {
        if(eptr)
        {
          std::rethrow_exception(eptr);
        }
      }
      catch(const std::out_of_range &e)
      {
        return Glib::ustring("Caught std::out_of_range ") + e.what();
      }
      catch(const std::runtime_error &e)
      {
        return Glib::ustring("Caught runtime_error ") + e.what();
      }
      catch(const std::exception &e)
      {
        return Glib::ustring("Caught exception ") + e.what();
      }
      catch(...)
      {
        return "Unknown Exception!";
      }
      return "";
    }

    template <typename... tArgs> void throwException(const tArgs &... args)
    {
      auto str = nltools::string::concat(args...);
      throw std::runtime_error(str);
    }
}