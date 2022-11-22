#pragma once

/******************************************************************************/
/** @file       macro_assignment.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      further abstraction for mc assignments, compatible with enum
                classes
    @note       - instanciate with enum classes Macro_Controls and
                  Modulateable_Parameters as template parameters
                - object is operational for numeric and enum class types
                  (see parameter_info.h, namespace C15::Parameters::...)
                - traverse MC assignment list from parent scope by something like:
                  for(int32_t i = MC_A.first(mcId); MC_A.running(); i = MC_A.next())
                  { ... }
    @todo
*******************************************************************************/

#include "mappable_list.h"

template <class MC, class MP> class MC_Assignment
{
 public:
  inline MC_Assignment()
  {
  }
  inline void reset()
  {
    m_list.reset();
  }
  inline void reset(const uint32_t _mcId)
  {
    m_list.reset(_mcId);
  }
  inline void reset(const MC _mcId)
  {
    m_list.reset(static_cast<uint32_t>(_mcId));
  }
  inline void reassign(const uint32_t _paramId, const uint32_t _mcId)
  {
    m_list.reassign(_paramId, _mcId);
  }
  inline void reassign(const MP _paramId, const MC _mcId)
  {
    m_list.reassign(static_cast<uint32_t>(_paramId), static_cast<uint32_t>(_mcId));
  }
  inline int32_t first(const uint32_t _mcId)
  {
    return m_list.first(_mcId);
  }
  inline int32_t first(const MC _mcId)
  {
    return m_list.first(static_cast<uint32_t>(_mcId));
  }
  inline bool running()
  {
    return m_list.running();
  }
  inline int32_t next()
  {
    return m_list.next();
  }

 private:
  MappableList<
    static_cast<uint32_t>(MC::_OPTIONS_),
    static_cast<uint32_t>(MC::None),
    static_cast<uint32_t>(MP::_LENGTH_)
  > m_list;
};
