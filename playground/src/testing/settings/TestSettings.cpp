#include "TestSettings.h"

void TestSettings::handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring &path)
{
}

TestSettings::TestSettings()
    : Settings(nullptr)
{
}
