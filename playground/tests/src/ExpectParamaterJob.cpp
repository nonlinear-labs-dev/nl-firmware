#include "ExpectParamaterJob.h"
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

ExpectParamaterJob::ExpectParamaterJob(Job *parent, const string &cmd)
    : super(parent)
{
  m_ID = parseInt(cmd, "id", 0);
  m_value = parseFloat(cmd, "value", 0);
}

ExpectParamaterJob::~ExpectParamaterJob()
{
}

void ExpectParamaterJob::implRun()
{
  DEBUG("Expect parameter " << m_ID << " to be " << m_value);

  bool result = false;
  SoupMessage *msg = soup_message_new("GET", URL("/get-update?updateID=0"));
  soup_session_send_message(getSession(), msg);

  xmlDoc *doc = xmlParseMemory(msg->response_body->data, msg->response_body->length);

  std::stringstream path;
  path << "/nonlinear-world/preset-manager/edit-buffer/parameter-group/parameter[@id='" << m_ID << "']/value";

  string pathStr = path.str();

  xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
  xmlXPathObject *xpathObj = xmlXPathEvalExpression((xmlChar *) pathStr.c_str(), xpathCtx);

  if(xpathObj && xpathObj->nodesetval)
  {
    switch(xpathObj->nodesetval->nodeNr)
    {
      case 0:
        WARN("Did not find the parameter " << m_ID << " in the update document.");
        break;

      case 1:
      {
        string res = (const char *) xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]);
        float gotValue = std::stod(res);
        float diff = fabs(gotValue - m_value);
        result = diff == 0.0f;

        if(result)
        {
          TRACE("parameter " << m_ID << "=" << gotValue << " - as expected");
        }
        else
        {
          TRACE("parameter " << m_ID << "=" << gotValue << " - expected " << m_value);
        }
        break;
      }

      default:
        WARN("Huh? Found the parameter " << m_ID << " for " << xpathObj->nodesetval->nodeNr << " times!?");
        break;
    }
  }
  else
  {
    WARN("Did not find the parameter " << m_ID << " in the update document or could not parse doc.");
  }

  if(xpathObj)
    xmlXPathFreeObject(xpathObj);

  xmlXPathFreeContext(xpathCtx);
  xmlFreeDoc(doc);
  g_object_unref(msg);

  getParent()->onChildFinished(this, result);
}
