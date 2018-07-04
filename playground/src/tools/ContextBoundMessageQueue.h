#pragma once
#include <glibmm/main.h>
#include <queue>

class ContextBoundMessageQueue {
public:
    using tMessage = std::function<void(void)>;
    explicit ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context);
    void pushMessage(tMessage&& m);
protected:
    Glib::RefPtr<Glib::MainContext> m_context;
};