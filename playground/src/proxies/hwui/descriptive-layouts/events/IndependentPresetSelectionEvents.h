#pragma once

#include <proxies/hwui/descriptive-layouts/EventProvider.h>

namespace DescriptiveLayouts
{
class EventSourceBase;

class IndependentPresetSelectionEvents : public EventProvider
{
public:
    IndependentPresetSelectionEvents();
    ~IndependentPresetSelectionEvents() override;

    connection connect(EventSources source, const Callback &cb) override;
    void fire(EventSinks e) override;

private:
    void bruteForce();
    void sanitize();
    void setString(EventSources e, const std::string &str);

    template<typename T, typename ...P> void createEventSources(P... e);

private:
    constexpr static size_t invalid = -1;
    size_t selectedBankNumber = invalid;
    size_t selectedPresetNumber = invalid;
    std::list<sigc::connection> connections;
    UpdateDocumentContributor::tUpdateID lastSeenPresetManagerRevision = -1;

    std::map<EventSources, std::unique_ptr<EventSourceBase>> eventSources;
};
}
