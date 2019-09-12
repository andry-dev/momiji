#ifndef HELP_WINDOW_H
#define HELP_WINDOW_H

#include <QWidget>
#include <qwidget.h>

namespace Ui
{
    class HelpWindow;
}

class HelpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget* parent = nullptr);
    ~HelpWindow();

private:
    Ui::HelpWindow* ui;
};

#endif
