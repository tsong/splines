#include "addpointcommand.h"

AddPointCommand::AddPointCommand(Vector2f point, BSpline &spline, QUndoCommand *parent)
    : QUndoCommand(parent), m_point(point), m_spline(spline)
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
    m_spline.insertPoint(m_spline.numberOfPoints(), m_point);
}

void AddPointCommand::undo() {
    m_spline.deletePoint(m_spline.numberOfPoints()-1);
}
