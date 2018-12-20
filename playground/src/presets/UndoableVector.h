#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
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

  static constexpr size_t invalidPosition = std::numeric_limits<size_t>::max();

  UndoableVector(ChangeCB &&c, CloneFactory &&f)
      : m_onChange(c)
      , m_factory(f)
      , m_selection(Uuid::none())
  {
  }

  UndoableVector &operator=(const UndoableVector &other)
  {
    Checker checker(this);
    m_elements.clear();
    other.forEach([&](auto e) { m_elements.emplace_back(m_factory(e)); });

    if(auto p = other.getSelected())
      m_selection = at(getIndexOf(p))->getUuid();
    else if(!empty())
      m_selection = first()->getUuid();
    else
      m_selection = Uuid::none();

    return *this;
  }

  Element *getSelected() const
  {
    return find(m_selection);
  }

  const Uuid &getSelectedUuid() const
  {
    return m_selection;
  }

  bool empty() const
  {
    return m_elements.empty();
  }

  Element *at(size_t idx) const
  {
    return getElements().at(idx).get();
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

  void select(UNDO::Transaction *transaction, const Uuid &uuid, std::function<void()> cb = nullptr)
  {
    Checker checker(this);
    if(m_selection != uuid)
    {
      transaction->addSimpleCommand([this, cb, swap = UNDO::createSwapData(uuid)](auto) {
        Checker checker(this);
        swap->swapWith(m_selection);
        m_onChange();
        if(cb)
          cb();
      });
    }
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
    Checker checker(this);
    auto raw = p.get();
    auto swapData = UNDO::createSwapData(std::move(p));
    pos = std::min(pos, size());

    transaction->addSimpleCommand(
        [=](auto) {
          Checker checker(this);
          auto it = std::next(m_elements.begin(), pos);
          it = m_elements.insert(it, ElementPtr());
          swapData->swapWith(*it);
          m_onChange();
        },
        [=](auto) {
          Checker checker(this);
          auto it = std::next(m_elements.begin(), pos);
          swapData->swapWith(*it);
          m_elements.erase(it);
          m_onChange();
        });

    return raw;
  }

  void move(UNDO::Transaction *transaction, const Element *toMove, const Element *before)
  {
    Checker checker(this);
    if(toMove != before)
    {
      auto isSelected = getSelected() == toMove;
      auto e = release(transaction, toMove);
      auto idx = before ? getIndexOf(before) : size();
      adopt(transaction, idx, e);

      if(isSelected)
        select(transaction, toMove->getUuid());
    }
  }

  void remove(UNDO::Transaction *transaction, const Uuid &uuid)
  {
    Checker checker(this);
    auto it = find_if(m_elements.begin(), m_elements.end(), [&](const auto &b) { return b->getUuid() == uuid; });

    if(it != m_elements.end())
    {
      if(uuid == m_selection)
        fixSelection(transaction);

      auto pos = std::distance(m_elements.begin(), it);
      auto swapData = UNDO::createSwapData(ElementPtr(nullptr));

      transaction->addSimpleCommand(
          [=](auto) {
            Checker checker(this);
            auto it = std::next(m_elements.begin(), pos);
            ElementPtr e = std::move(*it);
            m_elements.erase(it);
            swapData->swapWith(e);
            m_onChange();
          },
          [=](auto) {
            Checker checker(this);
            ElementPtr e;
            swapData->swapWith(e);
            auto it = std::next(m_elements.begin(), pos);
            m_elements.insert(it, std::move(e));
            m_onChange();
          });
    }
  }

  void fixSelection(UNDO::Transaction *transaction)
  {
    Checker checker(this);
    auto doomedElementUuid = m_selection;
    auto newSelectionPos = getPreviousPosition(doomedElementUuid);

    if(newSelectionPos == invalidPosition)
      newSelectionPos = getNextPosition(doomedElementUuid);

    if(newSelectionPos != invalidPosition)
    {
      select(transaction, at(newSelectionPos)->getUuid());
    }
    else
    {
      select(transaction, Uuid::none());
    }
  }

  Element *release(UNDO::Transaction *transaction, const Uuid &uuid)
  {
    Checker checker(this);
    auto it = find_if(m_elements.begin(), m_elements.end(), [&](const auto &b) { return b->getUuid() == uuid; });
    return release(transaction, it);
  }

  Element *release(UNDO::Transaction *transaction, const Element *e)
  {
    Checker checker(this);
    auto it = find_if(m_elements.begin(), m_elements.end(), [&](auto &p) { return p.get() == e; });
    return release(transaction, it);
  }

  Element *release(UNDO::Transaction *transaction, typename Elements::iterator it)
  {
    Checker checker(this);
    if(it != m_elements.end())
    {
      auto pos = std::distance(m_elements.begin(), it);
      auto it = std::next(m_elements.begin(), pos);
      Element *theElement = it->get();

      if(theElement->getUuid() == m_selection)
      {
        fixSelection(transaction);
      }

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
    Checker checker(this);
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
    Checker checker(this);

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
    try
    {
      if(auto pos = getIndexOf(uuid))
        return pos - 1;
    }
    catch(...)
    {
    }

    if(empty())
      return invalidPosition;

    return 0;
  }

  size_t getNextPosition(const Uuid &uuid) const
  {
    try
    {
      auto pos = getIndexOf(uuid) + 1;
      if(pos < size())
        return pos;
    }
    catch(...)
    {
    }

    if(empty())
      return invalidPosition;

    return size() - 1;
  }

 private:
  struct Checker
  {
    Checker(UndoableVector *p)
        : m_p(p)
    {
      m_p->check();
    }

    ~Checker()
    {
      m_p->check();
    }

    UndoableVector *m_p;
  };
  void check()
  {
#if _DEVELOPMENT_PC
    if(!empty())
    {
      //assert(getSelected());
      assert(std::none_of(m_elements.begin(), m_elements.end(), [&](auto &b) { return b.get() == nullptr; }));
      std::unordered_set<std::string> uuids;
      std::for_each(m_elements.begin(), m_elements.end(), [&](auto &c) { uuids.insert(c->getUuid().raw()); });
      assert(uuids.size() == size());
    }
#endif
  }
  ChangeCB m_onChange;
  CloneFactory m_factory;
  Elements m_elements;
  Uuid m_selection;
};