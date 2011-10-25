#ifndef ADDPOINTCOMMAND_H
#define ADDPOINTCOMMAND_H

#include <QUndoCommand>
#include "utils/vector.h"
#include "bspline.h"

#define ADD_POINT_COMMAND_ID 0xFF00

using namespace std;

class DisplayWidget;

class AddPointCommand : public QUndoCommand {

public:
    AddPointCommand(Vector2f point, BSpline &spline, QUndoCommand *parent = 0);
    ~AddPointCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    Vector2f m_point;
    BSpline &m_spline;

};

#endif // ADDPOINTCOMMAND_H
