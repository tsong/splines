#ifndef TOGGLECOMMAND_H
#define TOGGLECOMMAND_H

#include <QUndoCommand>
#include "displaywidget.h"

#define TOGGLE_COMMAND_ID 0xFF00

using namespace std;

class DisplayWidget;

class ToggleCommand : public QUndoCommand {

public:
    ToggleCommand(DisplayWidget &displayWidget, bool &variable, QUndoCommand *parent = 0);
    ~ToggleCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    DisplayWidget &m_displayWidget;
    bool &m_variable;
    bool m_originalValue;

};

#endif // TOGGLECOMMAND_H
