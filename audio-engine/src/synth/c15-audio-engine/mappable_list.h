#pragma once

/******************************************************************************/
/** @file       mappable_list.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      based on a doubly-linked list, but capable of handling multiple
                entry and exit points and therefore multiple "sublists"
    @todo
*******************************************************************************/

#include <stdint.h>

// specify number of sublists and overall length (number of assignable items)
template<uint32_t Sublists, uint32_t Length>
class MappableList
{
public:
    // constructor
    inline MappableList()
    {
        reset();
    }
    // reset methods: complete or sublist only
    inline void reset()
    {
        // manipulate entry and exit points
        for(int32_t i = 0; i < Sublists; i++)
        {
            const bool s = i == 0;
            m_head[i] = s ? 0 : -1;
            m_tail[i] = s ? m_last : -1;
        }
        // place every element to sublist 0 (serving as "unassigned" sublist)
        for(int32_t i = 0; i < Length; i++)
        {
            m_prev[i] = i - 1;
            m_next[i] = i + 1;
            m_list[i] = 0;
            m_index = m_next[m_last] = -1;
        }
    }
    inline void reset(const uint32_t _listId)
    {
        // reassigning every sublist element to sublist zero
        while(m_head[_listId] != -1)
        {
            reassign(m_head[_listId], 0);
        }
    }
    // reassign method: move an element from one list to another
    inline void reassign(const uint32_t _elementId, const uint32_t _listId)
    {
        // only if element actually changes sublist affiliation
        if(m_list[_elementId] != _listId)
        {
            removeElement(_elementId);
            addElement(_elementId, _listId);
        }
    }
    // traversal methods: facilitate sublist iteration from parent scope
    inline int32_t first(const uint32_t _listId)
    {
        // provide the sublist entry point at start of traversal
        m_index = m_head[_listId];
        return m_index;
    }
    inline bool running()
    {
        // check validity of sublist traversal at current element
        return m_index != -1;
    }
    inline int32_t next()
    {
        // provide the next sublist element
        m_index = m_next[m_index];
        return m_index;
    }
private:
    // private data arrays: prev and next are pseudo-pointers, list stores element affiliation, head and tail are entry/exit points
    int32_t m_prev[Length], m_next[Length], m_list[Length], m_head[Sublists], m_tail[Sublists], m_index = {};
    const int32_t m_last = Length - 1;
    // basic methods in order to facilitate reassign method
    inline void addElement(const uint32_t _elementId, const uint32_t _listId)
    {
        // two possible scenarios
        if(m_head[_listId] == -1)
        {
            // either first element of currently empty sublist
            m_head[_listId] = m_tail[_listId] = _elementId;
            m_prev[_elementId] = m_next[_elementId] = -1;
        }
        else
        {
            // or next element of non-empty sublist (so, there is a last element of sublist)
            const int32_t last = m_tail[_listId];
            m_next[last] = m_tail[_listId] = _elementId;
            m_prev[_elementId] = last;
            m_next[_elementId] = -1;
        }
        // either way, element now belongs to sublist
        m_list[_elementId] = _listId;
    }
    inline void removeElement(const uint32_t _elementId)
    {
        // first, gather some information about element neighbors and sublist affiliation
        const int32_t prev = m_prev[_elementId], next = m_next[_elementId], listId = m_list[_elementId];
        // four possible scenarios
        const uint32_t condition = (prev == -1 ? 1 : 0) + (next == -1 ? 2 : 0);
        switch(condition)
        {
        case 0:
            // two neighbors, element is in sublist of at least length 3
            m_next[prev] = next;
            m_prev[next] = prev;
            break;
        case 1:
            // one neighbor, element is first in sublist of at least length 2
            m_head[listId] = next;
            m_prev[next] = -1;
            break;
        case 2:
            // one neighbor, element is last in sublist of at least length 2
            m_tail[listId] = prev;
            m_next[prev] = -1;
            break;
        case 3:
            // no neighbors, element is last in sublist
            m_head[listId] = m_tail[listId] = -1;
            break;
        }
        // either way, element now disappears
        m_prev[_elementId] = m_next[_elementId] = m_list[_elementId] = -1;
    }
};
