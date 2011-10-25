#ifndef CLEARCOMMAND_H
#define CLEARCOMMAND_H

#include <QUndoCommand>
#include "utils/vector.h"
#include "displaywidget.h"

#define CLEAR_COMMAND_ID 0xFF04

using namespace std;

class DisplayWidget;

class ClearCommand : public QUndoCommand {

public:
    ClearCommand(DisplayWidget &displayWidget, QUndoCommand *parent = 0);
    ~ClearCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    vector<Vector2f> m_originalPoints;
    DisplayWidget &m_displayWidget;

};

#endif // CLEARCOMMAND_H
