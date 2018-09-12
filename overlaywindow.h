#ifndef OVERLAYWINDOW_H
#define OVERLAYWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QKeyEvent>
#include <QShowEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QEvent>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>

namespace Ui {
class OverlayWindow;
}

class OverlayWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit OverlayWindow(int screenId, QWidget *parent = 0);
    virtual ~OverlayWindow();

    void keyPressEvent(QKeyEvent *event);
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


signals:
    void screenCaptured(QImage image, QRect selection);
    void closeSignaled();

private:
    enum class MouseState {
        PRESSED,
        RELEASED
    };
    MouseState mouseState;
    QPoint mouseDragStart;

    Ui::OverlayWindow *ui;
    int const screenId;
    QPixmap screen;
};

#endif // OVERLAYWINDOW_H

