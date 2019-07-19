#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Emulator.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_sourceCode_textChanged();

private:
    Ui::MainWindow* ui;
    momiji::Emulator m_emulator;
};

#endif // MAINWINDOW_H
