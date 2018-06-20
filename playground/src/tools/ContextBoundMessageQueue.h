#pragma once
#include <glibmm/main.h>
#include <queue>

class ContextBoundMessageQueue {
public:
    using tMessage = std::function<void(void)>;
    ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context);
    void pushMessage(tMessage m);
protected:
    Glib::RefPtr<Glib::MainContext> m_context;
};