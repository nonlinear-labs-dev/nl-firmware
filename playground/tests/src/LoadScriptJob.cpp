#include "LoadScriptJob.h"
#include "Tests.h"
#include "Options.h"

LoadScriptJob::LoadScriptJob (Job *parent) :
    super (parent)
{
}

LoadScriptJob::~LoadScriptJob ()
{
}

string LoadScriptJob::getContent () const
{
  return m_content;
}

void LoadScriptJob::implRun ()
{
  Tests *tests = static_cast<Tests*> (getRoot ());
  string script = tests->getOptions ()->getScript ();

  gchar *contents = nullptr;
  gsize length = 0;
  GError *error = nullptr;

  if (g_file_get_contents (script.c_str (), &contents, &length, &error))
  {
    m_content = contents;
    getParent ()->onChildFinished (this, true);
    g_free(contents);
  }
  else
  {
    WARN(error->message);
    g_error_free (error);
    getParent ()->onChildFinished (this, false);
  }
}
