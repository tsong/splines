#include "clearcommand.h"

ClearCommand::ClearCommand(DisplayWidget &displayWidget, QUndoCommand *parent)
    : QUndoCommand(parent), m_displayWidget(displayWidget)
{
    m_originalPoints = displayWidget.m_controlPoints;
}

ClearCommand::~ClearCommand() {

}

int ClearCommand::id() const {
    return CLEAR_COMMAND_ID;
}

bool ClearCommand::mergeWith(const QUndoCommand *other) {
    return other->id() == id();
}

void ClearCommand::redo() {
    m_displayWidget.m_controlPoints.clear();
    m_displayWidget.repaint();
}

void ClearCommand::undo() {
    m_displayWidget.m_controlPoints = m_originalPoints;
    m_displayWidget.repaint();
}
