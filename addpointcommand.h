#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include "vector.h"
#include "displaywidget.h"

#define ADD_POINT_COMMAND_ID 0xFF00

using namespace std;

class DisplayWidget;

class AddPointCommand : public QUndoCommand {

public:
    AddPointCommand(Vector2f point, DisplayWidget &displayWidget, QUndoCommand *parent = 0);
    ~AddPointCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    Vector2f m_point;
    list<Vector2f>::iterator m_insertIterator;
    DisplayWidget &m_displayWidget;

};

#endif // ADDCOMMAND_H
