#include <proxies/hwui/panel-unit/MacroControlAssignmentStateMachine.h>

MacroControlAssignmentStateMachine::MacroControlAssignmentStateMachine()
    : m_modParamId { ParameterId::invalid() }
{
  registerTraversal(MacroControlAssignmentStates::Initial, MacroControlAssignmentEvents::MCPressedWhileUnselected,
                    MacroControlAssignmentStates::Selected);
  registerTraversal(MacroControlAssignmentStates::Initial, MacroControlAssignmentEvents::MCPressedWhileSelected,
                    MacroControlAssignmentStates::Armed);
  registerTraversal(MacroControlAssignmentStates::Selected, MacroControlAssignmentEvents::ModParamPressed,
                    MacroControlAssignmentStates::Assign);
  registerTraversal(MacroControlAssignmentStates::Selected, MacroControlAssignmentEvents::MCReleased,
                    MacroControlAssignmentStates::Initial);
  registerTraversal(MacroControlAssignmentStates::Assign, MacroControlAssignmentEvents::ModParamReleased,
                    MacroControlAssignmentStates::Selected);
  registerTraversal(MacroControlAssignmentStates::Assign, MacroControlAssignmentEvents::MCReleased,
                    MacroControlAssignmentStates::Initial);
  registerTraversal(MacroControlAssignmentStates::Armed, MacroControlAssignmentEvents::ModParamPressed,
                    MacroControlAssignmentStates::Assign);
  registerTraversal(MacroControlAssignmentStates::Armed, MacroControlAssignmentEvents::MCReleased,
                    MacroControlAssignmentStates::SelectSource);
}

MacroControlAssignmentStateMachine::~MacroControlAssignmentStateMachine()
{
}

void MacroControlAssignmentStateMachine::setCurrentMCParameter(int mcParamId)
{
  m_mcParamId = mcParamId;
}

int MacroControlAssignmentStateMachine::getCurrentMCParameter() const
{
  return m_mcParamId;
}

void MacroControlAssignmentStateMachine::setCurrentModulateableParameter(const ParameterId& id)
{
  m_modParamId = id;
}

const ParameterId& MacroControlAssignmentStateMachine::getCurrentModulateableParameter() const
{
  return m_modParamId;
}
