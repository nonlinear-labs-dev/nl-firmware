#pragma once

#include <playground.h>
#include "Uncopyable.h"
#include <future>

class BackgroundJob : public Uncopyable
{
  public:
    template<typename tBG, typename tFG>
      BackgroundJob (tBG bg, tFG cb)
      {
        m_future = std::async (std::launch::async, [ = ]()
        {
          auto ret = bg ();

          m_callbackConnection = Glib::MainContext::get_default()->signal_idle().connect (
              sigc::bind (
                  sigc::mem_fun (this, &BackgroundJob::doCallback<tFG, decltype (ret) >),
                  cb, ret));
        });
      }

    virtual ~BackgroundJob ()
    {
      m_callbackConnection.disconnect ();
    }

    virtual void onJobFinished ()
    {
    }
    ;

  private:
    template<typename tCB, typename tRet>
      bool doCallback (tCB cb, tRet ret)
      {
        onJobFinished ();
        m_callbackConnection.disconnect ();
        m_future = std::future<void> ();
        cb (ret);
        return false;
      }

    std::future<void> m_future;
    sigc::connection m_callbackConnection;
};
