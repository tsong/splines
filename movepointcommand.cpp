#include "movepointcommand.h"

MovePointCommand::MovePointCommand(int id, uint position, Vector2f newPoint, DisplayWidget &displayWidget, QUndoCommand *parent)
    : QUndoCommand(parent), m_id(id), m_position(position), m_newPoint(newPoint), m_displayWidget(displayWidget)
{
    m_originalPoint = m_displayWidget.m_controlPoints[position];
}

MovePointCommand::~MovePointCommand() {

}

int MovePointCommand::id() const {
    return m_id;
}

bool MovePointCommand::mergeWith(const QUndoCommand *other) {
    if (other->id() != id())
        return false;

    m_newPoint = static_cast<const MovePointCommand*>(other)->m_newPoint;
    return true;
}

void MovePointCommand::redo() {
    //m_displayWidget.m_controlPoints[m_position] = m_newPoint;
    //m_displayWidget.repaint();
    m_displayWidget.movePoint(m_position, m_newPoint);
}

void MovePointCommand::undo() {
    //m_displayWidget.m_controlPoints[m_position] = m_originalPoint;
    //m_displayWidget.repaint();
    m_displayWidget.movePoint(m_position, m_originalPoint);
}
