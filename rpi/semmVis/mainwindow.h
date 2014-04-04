#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "playboardview.h"
#include "playboardscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initializeVisualization( uint32_t playboardWidth, uint32_t playboardHeight,
                                  uint32_t meepleWidth, uint32_t meepleHeight );

    PlayboardView* view( ){ return playboardView; }
private slots:

//private:
public:
    Ui::MainWindow *ui;

    PlayboardView* playboardView;
};

#endif // MAINWINDOW_H
