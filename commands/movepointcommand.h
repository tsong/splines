#ifndef MOVEPOINTCOMMAND_H
#define MOVEPOINTCOMMAND_H

#include <QUndoCommand>
#include "utils/vector.h"
#include "displaywidget.h"

using namespace std;

class DisplayWidget;

class MovePointCommand : public QUndoCommand {

public:
    MovePointCommand(int id, uint position, Vector2f newPoint, DisplayWidget &displayWidget, QUndoCommand *parent = 0);
    ~MovePointCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    int m_id;
    uint m_position;
    Vector2f m_newPoint;
    Vector2f m_originalPoint;
    DisplayWidget &m_displayWidget;

};
#endif // MOVEPOINTCOMMAND_H
