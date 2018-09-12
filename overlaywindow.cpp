#include "overlaywindow.h"
#include "ui_overlaywindow.h"
#include <QWindow>
#include <QMenu>
#include <QCursor>
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QPen>
#include <tesseract/baseapi.h>
#include <QScreen>

#include <iostream>

OverlayWindow::OverlayWindow(int screenId, QWidget *parent) :
    QMainWindow(parent),
    mouseState(MouseState::RELEASED),
    mouseDragStart(),
    ui(new Ui::OverlayWindow),
    screenId(screenId)
{
    ui->setupUi(this);
}

OverlayWindow::~OverlayWindow()
{
    delete ui;
}

void OverlayWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        emit closeSignaled();
    }

    event->accept();
}

void OverlayWindow::showEvent(QShowEvent *event) {

    setCursor(Qt::CrossCursor);

    auto screenObject = QApplication::screens().at(screenId);
    auto screenGeometry = screenObject->geometry();
    screen = screenObject->grabWindow(QApplication::desktop()->winId(),
                                      screenGeometry.x(),
                                      screenGeometry.y(),
                                      screenGeometry.width(),
                                      screenGeometry.height());


    event->accept();
}

void OverlayWindow::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.drawPixmap(0, 0, width(), height(), screen);
    if (mouseState == MouseState::PRESSED) {
        QPoint mousePos(mapFromGlobal(QCursor::pos()));
        QPen pen(Qt::DashLine);
        pen.setWidth(3);
        pen.setColor(QColor::fromRgb(255, 0, 0));
        p.setPen(pen);
        p.drawRect(mouseDragStart.x(),
                   mouseDragStart.y(),
                   mousePos.x() - mouseDragStart.x(),
                   mousePos.y() - mouseDragStart.y());
    }

    event->accept();
}

void OverlayWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        setWindowState(Qt::WindowFullScreen);

        event->accept();
    }
}

void OverlayWindow::closeEvent(QCloseEvent *event) {
    unsetCursor();

    event->accept();
}

void OverlayWindow::mousePressEvent(QMouseEvent *event) {
    mouseDragStart = event->pos();
    mouseState = MouseState::PRESSED;

    event->accept();
}

void OverlayWindow::mouseReleaseEvent(QMouseEvent *event) {
    mouseState = MouseState::RELEASED;

    QPoint mousePos(mapFromGlobal(QCursor::pos()));
    emit screenCaptured(screen.toImage(),
                        QRect(mouseDragStart.x(),
                              mouseDragStart.y(),
                              mousePos.x() - mouseDragStart.x(),
                              mousePos.y() - mouseDragStart.y()));

    event->accept();

    emit closeSignaled();
}

void OverlayWindow::mouseMoveEvent(QMouseEvent *event) {
    repaint();

    event->accept();
}
