#include "SendEditBufferScopeGuard.h"
#include <Application.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <libundo/undo/Transaction.h>

SendEditBufferScopeGuard::SendEditBufferScopeGuard(UNDO::Transaction* transaction, bool sendToAE)
    : sendToAudioEngine { sendToAE }
    , m_transaction { transaction }
{
  m_transaction->addSimpleCommand(
      [send = sendToAudioEngine](auto s)
      {
        if(Application::exists())
        {
          if(s == UNDO::Transaction::UNDOING)
            Application::get().getAudioEngineProxy()->thawParameterMessages(send);
          else
            Application::get().getAudioEngineProxy()->freezeParameterMessages();
        }
      });
}

SendEditBufferScopeGuard::~SendEditBufferScopeGuard()
{
  m_transaction->addSimpleCommand(
      [send = sendToAudioEngine](auto s)
      {
        if(Application::exists())
        {
          if(s == UNDO::Transaction::REDOING || s == UNDO::Transaction::DOING)
            Application::get().getAudioEngineProxy()->thawParameterMessages(send);
          else
            Application::get().getAudioEngineProxy()->freezeParameterMessages();
        }
      });
}