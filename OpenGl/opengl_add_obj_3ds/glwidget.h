#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>

namespace Ui {
class GLWidget;
}

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QGLWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    Ui::GLWidget *ui;

    // Frustum
    QMatrix4x4 matrix;
};

#endif // GLWIDGET_H
