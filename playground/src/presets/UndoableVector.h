#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <tools/Uuid.h>
#include "libundo/undo/SwapData.h"
#include "libundo/undo/Transaction.h"
#include <assert.h>

template <typename Element> class UndoableVector
{
 public:
  using ElementPtr = std::unique_ptr<Element>;
  using Elements = std::vector<ElementPtr>;
  using ChangeCB = std::function<void()>;
  using CloneFactory = std::function<Element *(const Element *)>;

  UndoableVector(ChangeCB &&c, CloneFactory &&f)
      : m_onChange(c)
      , m_factory(f)
  {
  }

  UndoableVector &operator=(const UndoableVector &other)
  {
    m_elements.clear();
    other.forEach([&](auto e) { m_elements.emplace_back(m_factory(e)); });
    return *this;
  }

  bool empty() const
  {
    return m_elements.empty();
  }

  Element *first() const
  {
    return m_elements.at(0).get();
  }

  size_t getIndexOf(const Uuid &uuid) const
  {
    auto it = std::find_if(m_elements.begin(), m_elements.end(), [&](const auto &b) { return b->getUuid() == uuid; });

    if(it != m_elements.end())
      return std::distance(m_elements.begin(), it);

    throw std::out_of_range("Element is not in vector");
  }

  size_t getIndexOf(const Element *e) const
  {
    auto it = std::find_if(m_elements.begin(), m_elements.end(), [&](auto &p) { return p.get() == e; });

    if(it != m_elements.end())
      return std::distance(m_elements.begin(), it);

    throw std::out_of_range("Element is not in vector");
  }

  size_t size() const
  {
    return m_elements.size();
  }

  void forEach(std::function<void(Element *)> cb) const
  {
    std::for_each(m_elements.begin(), m_elements.end(), [cb](auto &p) { cb(p.get()); });
  }

  Element *find(const Uuid &uuid) const
  {
    using namespace std;
    auto ret = find_if(m_elements.begin(), m_elements.end(), [&](const auto &b) { return b->getUuid() == uuid; });

    if(ret != m_elements.end())
      return ret->get();

    return nullptr;
  }

  Element *findNear(const Uuid &uuid, int seek) const
  {
    using namespace std;
    auto ret = find_if(m_elements.begin(), m_elements.end(), [&](const auto &b) { return b->getUuid() == uuid; });

    while(seek > 0 && ret != m_elements.end())
    {
      seek--;
      std::advance(ret, 1);
    }

    while(seek < 0 && ret != m_elements.begin())
    {
      seek++;
      std::advance(ret, -1);
    }

    if(seek == 0 && ret != m_elements.end())
      return ret->get();

    return nullptr;
  }

  Element *append(UNDO::Transaction *transaction, ElementPtr p)
  {
    return insert(transaction, size(), std::move(p));
  }

  Element *prepend(UNDO::Transaction *transaction, ElementPtr p)
  {
    return insert(transaction, 0, std::move(p));
  }

  Element *insert(UNDO::Transaction *transaction, size_t pos, ElementPtr p)
  {
    auto raw = p.get();
    auto swapData = UNDO::createSwapData(std::move(p));
    pos = std::min(pos, size());

    transaction->addSimpleCommand(
        [=](auto) {
          auto it = std::next(m_elements.begin(), pos);
          it = m_elements.insert(it, ElementPtr());
          swapData->swapWith(*it);
          m_onChange();
        },
        [=](auto) {
          auto it = std::next(m_elements.begin(), pos);
          swapData->swapWith(*it);
          m_elements.erase(it);
          m_onChange();
        });

    return raw;
  }

  void move(UNDO::Transaction *transaction, const Element *toMove, const Element *before)
  {
    auto isToMove = [=](auto &b) { return b->getUuid() == toMove->getUuid(); };
    auto fromIt = find_if(m_elements.begin(), m_elements.end(), isToMove);

    if(fromIt != m_elements.end())
    {
      auto toIt = m_elements.begin();

      if(before)
      {
        auto isBefore = [=](auto &b) { return b->getUuid() == before->getUuid(); };
        toIt = std::find_if(m_elements.begin(), m_elements.end(), isBefore);
      }

      auto fromPos = std::distance(m_elements.begin(), fromIt);
      auto toPos = std::distance(m_elements.begin(), toIt);
      auto swapData = UNDO::createSwapData(fromPos, toPos);

      transaction->addSimpleCommand([swapData, this](auto) {
        auto &fromPos = swapData->template get<0>();
        auto &toPos = swapData->template get<1>();
        auto fromIt = std::next(m_elements.begin(), fromPos);
        auto toIt = std::next(m_elements.begin(), toPos);
        m_elements.insert(toIt, std::move(*fromIt));
        auto isEmpty = [](auto &b) { return static_cast<bool>(b); };
        auto itEmpty = std::find_if(m_elements.begin(), m_elements.end(), isEmpty);
        m_elements.erase(itEmpty);
        std::swap(fromPos, toPos);
        m_onChange();
      });
    }
  }

  void remove(UNDO::Transaction *transaction, const Uuid &uuid)
  {
    auto it = find_if(m_elements.begin(), m_elements.end(), [&](const auto &b) { return b->getUuid() == uuid; });

    if(it != m_elements.end())
    {
      auto pos = std::distance(m_elements.begin(), it);
      auto swapData = UNDO::createSwapData(ElementPtr(nullptr));

      transaction->addSimpleCommand(
          [=](auto) {
            auto it = std::next(m_elements.begin(), pos);
            ElementPtr e = std::move(*it);
            m_elements.erase(it);
            swapData->swapWith(e);
            m_onChange();
          },
          [=](auto) {
            ElementPtr e;
            swapData->swapWith(e);
            auto it = std::next(m_elements.begin(), pos);
            m_elements.insert(it, std::move(e));
            m_onChange();
          });
    }
  }

  Element *release(UNDO::Transaction *transaction, const Uuid &uuid)
  {
    auto it = find_if(m_elements.begin(), m_elements.end(), [&](const auto &b) { return b->getUuid() == uuid; });
    return release(transaction, it);
  }

  Element *release(UNDO::Transaction *transaction, const Element *e)
  {
    auto it = find_if(m_elements.begin(), m_elements.end(), [&](auto &p) { return p.get() == e; });
    return release(transaction, it);
  }

  Element *release(UNDO::Transaction *transaction, typename Elements::iterator it)
  {
    if(it != m_elements.end())
    {
      auto pos = std::distance(m_elements.begin(), it);
      auto it = std::next(m_elements.begin(), pos);
      Element *theElement = it->get();

      transaction->addSimpleCommand(
          [=](auto) {
            auto it = std::next(m_elements.begin(), pos);
            auto ptr = it->release();
            m_elements.erase(it);
            m_onChange();
          },
          [=](auto) {
            auto it = std::next(m_elements.begin(), pos);
            m_elements.insert(it, ElementPtr(theElement));
            m_onChange();
          });

      return theElement;
    }
    return {};
  }

  void adopt(UNDO::Transaction *transaction, size_t pos, Element *p)
  {
    pos = std::min(pos, size());

    transaction->addSimpleCommand(
        [=](auto) {
          auto it = std::next(m_elements.begin(), pos);
          m_elements.insert(it, ElementPtr(p));
          m_onChange();
        },
        [=](auto) {
          auto it = std::next(m_elements.begin(), pos);
          it->release();
          m_elements.erase(it);
          m_onChange();
        });
  }

  void sort(UNDO::Transaction *transaction, const std::vector<Element *> &order)
  {
#ifdef _DEVELOPMENT_PC
    g_assert(order.size() == m_elements.size());

    for(auto e : order)
    {
      auto it = std::find_if(m_elements.begin(), m_elements.end(), [&](auto &p) { return p.get() == e; });
      g_assert(it != m_elements.end());
    }
#endif

    auto swap = UNDO::createSwapData(order);

    transaction->addSimpleCommand([=](auto) {
      std::vector<Element *> newOrder;
      std::transform(m_elements.begin(), m_elements.end(), std::back_inserter(newOrder),
                     [&](auto &e) { return e.get(); });
      swap->swapWith(newOrder);

      for(size_t i = 0; i < newOrder.size(); i++)
      {
        m_elements[i].release();
        m_elements[i].reset(newOrder[i]);
      }
    });
  }

  const std::vector<ElementPtr> &getElements() const
  {
    return m_elements;
  }

  size_t getPreviousPosition(const Uuid &uuid) const
  {
    if(uuid.empty() || size() == 0)
    {
      assert(size() == 0);
      return std::numeric_limits<size_t>::max();
    }

    auto pos = getIndexOf(uuid);

    if(pos == 0)
      return std::numeric_limits<size_t>::max();

    return pos - 1;
  }

  size_t getNextPosition(const Uuid &uuid) const
  {
    if(uuid.empty() || size() == 0)
    {
      assert(size() == 0);
      return std::numeric_limits<size_t>::max();
    }

    return std::min(getIndexOf(uuid) + 1, size() - 1);
  }

 private:
  ChangeCB m_onChange;
  CloneFactory m_factory;
  Elements m_elements;
};
