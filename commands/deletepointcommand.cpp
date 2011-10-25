#include "deletepointcommand.h"

DeletePointCommand::DeletePointCommand(uint position, BSpline &spline, QUndoCommand *parent)
    :  QUndoCommand(parent), m_spline(spline), m_position(position)
{
    m_point = m_spline.getPoints()[position];
    m_originalKnots = m_spline.getKnots();
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
    m_spline.deletePoint(m_position);
}

void DeletePointCommand::undo() {
    m_spline.insertPoint(m_position, m_point);
    m_spline.setKnots(m_originalKnots);
}

