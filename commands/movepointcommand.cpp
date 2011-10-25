#include "movepointcommand.h"

MovePointCommand::MovePointCommand(int id, uint position, Vector2f newPoint, BSpline &spline, QUndoCommand *parent)
    : QUndoCommand(parent), m_id(id), m_position(position), m_newPoint(newPoint), m_spline(spline)
{
    m_originalPoint = m_spline.getPoints()[position];
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
    m_spline.movePoint(m_position, m_newPoint);
}

void MovePointCommand::undo() {
    m_spline.movePoint(m_position, m_originalPoint);
}
