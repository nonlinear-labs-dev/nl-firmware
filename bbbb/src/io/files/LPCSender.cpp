#include <io/files/LPCSender.h>
#include "Application.h"
#include "Options.h"

LPCSender::LPCSender() :
    super(Application::get().getOptions()->getLPCSendFileName())
{
}

LPCSender::~LPCSender()
{
}

