#include "addpointcommand.h"

AddPointCommand::AddPointCommand(Vector2f point, DisplayWidget &displayWidget, QUndoCommand *parent)
    : QUndoCommand(parent), m_point(point), m_displayWidget(displayWidget)
{
}

AddPointCommand::~AddPointCommand() {

}

int AddPointCommand::id() const {
    return ADD_POINT_COMMAND_ID;
}

bool AddPointCommand::mergeWith(const QUndoCommand *) {
    return false;
}

void AddPointCommand::redo() {
    m_displayWidget.m_controlPoints.push_back(m_point);
    m_insertIterator = m_displayWidget.m_controlPoints.end();
    m_insertIterator--;
    m_displayWidget.repaint();
}

void AddPointCommand::undo() {
    m_displayWidget.m_controlPoints.erase(m_insertIterator);
    m_displayWidget.repaint();
}
