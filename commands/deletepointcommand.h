#ifndef DELETEPOINTCOMMAND_H
#define DELETEPOINTCOMMAND_H

#include <QUndoCommand>

#include <QUndoCommand>
#include "vector.h"
#include "displaywidget.h"

#define DELETE_POINT_COMMAND_ID 0xFF01

using namespace std;

class DisplayWidget;

class DeletePointCommand : public QUndoCommand {

public:
    DeletePointCommand(uint position, DisplayWidget &displayWidget, QUndoCommand *parent = 0);
    ~DeletePointCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    DisplayWidget &m_displayWidget;
    uint m_position;
    Vector2f m_point;

};

#endif // DELETEPOINTCOMMAND_H
