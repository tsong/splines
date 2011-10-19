#include "deletepointcommand.h"

DeletePointCommand::DeletePointCommand(uint position, DisplayWidget &displayWidget, QUndoCommand *parent)
    :  QUndoCommand(parent), m_displayWidget(displayWidget), m_position(position)
{
    m_point = m_displayWidget.m_controlPoints[position];
}

DeletePointCommand::~DeletePointCommand() {

}

int DeletePointCommand::id() const {
    return DELETE_POINT_COMMAND_ID;
}

bool DeletePointCommand::mergeWith(const QUndoCommand *) {
    return false;
}

void DeletePointCommand::redo() {
    vector<Vector2f> &points = m_displayWidget.m_controlPoints;
    m_displayWidget.m_controlPoints.erase(points.begin() + m_position);
    m_displayWidget.repaint();
}

void DeletePointCommand::undo() {
    vector<Vector2f> &points = m_displayWidget.m_controlPoints;
    points.insert(points.begin() + m_position, m_point);
    m_displayWidget.repaint();
}

