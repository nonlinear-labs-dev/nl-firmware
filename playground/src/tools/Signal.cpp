#include "Signal.h"
#include <testing/TestDriver.h>
#include <device-settings/DebugLevel.h>
#include <sigc++/trackable.h>
#include <glibmm/main.h>
#include <map>

static TestDriver<Signal<void, bool>> tests;

template <> void Signal<void, bool>::registerTests()
{
  class Sender : public sigc::trackable
  {
   public:
    Signal<void, bool> sig;
  };

  class Receiver : public sigc::trackable
  {
   public:
    Receiver(int &r)
        : received(r)
    {
    }
    void fn(bool v)
    {
      received++;
    }

    int &received;
  };

  g_test_add_func("/Signals/normalLifeCycle", []() {
    int received = 0;
    Sender *s = new Sender();
    Receiver *r = new Receiver(received);
    s->sig.connectAndInit(mem_fun(r, &Receiver::fn), false);
    g_main_iteration(FALSE);
    delete r;
    delete s;
    g_assert(received == 1);
  });

  g_test_add_func("/Signals/senderDiesFirst", []() {
    int received = 0;
    Sender *s = new Sender();
    Receiver *r = new Receiver(received);
    s->sig.connectAndInit(mem_fun(r, &Receiver::fn), false);
    delete s;
    g_main_iteration(FALSE);
    delete r;
    g_assert(received == 0);
  });

  g_test_add_func("/Signals/receiverDiesFirst", []() {
    int received = 0;
    Sender *s = new Sender();
    Receiver *r = new Receiver(received);
    s->sig.connectAndInit(mem_fun(r, &Receiver::fn), false);
    delete r;
    g_main_iteration(FALSE);
    delete s;
    g_assert(received == 0);
  });

  g_test_add_func("/Signals/reconnectBeforeInitCBWasReceived", []() {
    class Sender : public sigc::trackable
    {
     public:
      Signal<void, Sender *> sig;
    };

    class Receiver : public sigc::trackable
    {
     public:
      Receiver()
      {
      }
      void fn(const Sender *v)
      {
        received[v]++;
      }

      std::map<const Sender *, int> received;
    };

    auto senderA = std::make_unique<Sender>();
    auto senderB = std::make_unique<Sender>();
    auto r = std::make_unique<Receiver>();

    auto connection = senderA->sig.connectAndInit(mem_fun(r.get(), &Receiver::fn), senderA.get());
    connection.disconnect();
    connection = senderB->sig.connectAndInit(mem_fun(r.get(), &Receiver::fn), senderB.get());
    g_main_iteration(FALSE);
    g_assert(r->received[senderA.get()] == 0);
    g_assert(r->received[senderB.get()] == 1);
  });
}

SignalBase::SignalBase()
    : m_initCallbackScheduled(false)
{
}

SignalBase::~SignalBase()
{
  m_initCallbackConnection.disconnect();
  m_deferedCallbackConnection.disconnect();
}

void SignalBase::scheduleInitCallback(const void *cookie, SignalBase::tCallback &&cb)
{
  m_initRecords.push_back({ std::move(cb), cookie });

  if(!m_initCallbackScheduled.exchange(true))
  {
    m_initCallbackConnection = Glib::MainContext::get_default()->signal_idle().connect(
        sigc::mem_fun(this, &SignalBase::doTheCallbacks), Glib::PRIORITY_HIGH);
  }
}

bool SignalBase::doTheCallbacks()
{
  m_initCallbackScheduled.exchange(false);

  tInitRecords initRecords;
  swap(initRecords, m_initRecords);

  m_initCallbackConnection.disconnect();

  for(const auto &r : initRecords)
    if(findCookie(r.cookie))
      r.cb();

  return false;
}

void SignalBase::deferedSend()
{
  if(!m_deferedCallbackConnection.connected())
  {
    auto ctx = Glib::MainContext::get_default();
    m_deferedCallbackConnection
        = ctx->signal_idle().connect(sigc::mem_fun(this, &SignalBase::emitDefered), Glib::PRIORITY_DEFAULT);
  }
}

bool SignalBase::emitDefered()
{
  m_deferedCallbackConnection.disconnect();

  tCallback c;
  std::swap(c, m_deferedSend);

  if(c)
    c();

  return false;
}
