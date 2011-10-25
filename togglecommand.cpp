#include "togglecommand.h"

ToggleCommand::ToggleCommand(DisplayWidget &displayWidget, bool &variable, QUndoCommand *parent)
    : QUndoCommand(parent), m_displayWidget(displayWidget), m_variable(variable), m_originalValue(variable)
{
}

ToggleCommand::~ToggleCommand() {}

int ToggleCommand::id() const {
    return TOGGLE_COMMAND_ID;
}

bool ToggleCommand::mergeWith(const QUndoCommand *) {
    return false;
}

void ToggleCommand::redo() {
    m_variable = !m_originalValue;
    m_displayWidget.repaint();
}

void ToggleCommand::undo() {
    m_variable = m_originalValue;
    m_displayWidget.repaint();
}

