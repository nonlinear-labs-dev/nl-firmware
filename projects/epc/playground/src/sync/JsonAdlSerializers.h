#pragma once

#include <presets/UndoableVector.h>
#include <tools/Uuid.h>
#include <ParameterId.h>
#include <GroupId.h>
#include <parameters/Parameter.h>

namespace nlohmann
{
  template <typename A, typename B> struct adl_serializer<UndoableVector<A, B>>
  {
    static void to_json(json &j, const UndoableVector<A, B> &v)
    {
      v.forEach([&](B *a) { j.push_back(a->getUuid()); });
    }
  };

  template <typename T> struct adl_serializer<IntrusiveList<T>>
  {
    static void to_json(json &j, const IntrusiveList<T> &t)
    {
      t.template forEach([&j](const auto &x) { j.push_back(x); });
    }
  };

  template <> struct adl_serializer<Glib::ustring>
  {
    static void to_json(json &j, const Glib::ustring &v)
    {
      j = v.raw();
    }
  };

  template <> struct adl_serializer<Uuid>
  {
    static void to_json(json &j, const Uuid &v)
    {
      j = v.raw();
    }
  };

  template <> struct adl_serializer<ParameterId>
  {
    static void to_json(json &j, const ParameterId &v)
    {
      j = v.toString();
    }
  };

  template <> struct adl_serializer<GroupId>
  {
    static void to_json(json &j, const GroupId &v)
    {
      j = v.toString();
    }
  };

}
