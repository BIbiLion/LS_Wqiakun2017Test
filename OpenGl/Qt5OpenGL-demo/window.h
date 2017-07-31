#ifndef WINDOW_H
#define WINDOW_H

#include "abstractscene.h"
#include "basicusagescene.h"
#include <QWindow>

class QOpenGLContext;
#include <QOpenGLFunctions>
class Window : public QWindow , protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Window(QScreen *screen = 0);
    ~Window();
    
signals:
    
public slots:

protected slots:
    void resizeGl();
    void paintGl();
    void updateScene();

    void wheelEvent(QWheelEvent * ev);

private:
    void printContextInfos();

    QOpenGLContext *mContext;
    QScopedPointer<BasicUsageScene> mScene;
    
    void initializeGl();
};

#endif // WINDOW_H
