#pragma once

#include "playground.h"

class SearchQuery
{
  public:
    enum class Mode
    {
      And, Or
    };

    SearchQuery (const Glib::ustring &query, Mode mode);
    SearchQuery (const Glib::ustring &query, const Glib::ustring &mode);
    virtual ~SearchQuery ();

    bool iterate (function<bool (const Glib::ustring &)> cb) const;

    static void registerTests ();

  private:
    std::list<Glib::ustring> m_query;
    Mode m_mode;
};

