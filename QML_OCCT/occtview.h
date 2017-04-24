#ifndef OCCTVIEW_H
#define OCCTVIEW_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickFramebufferObject>
#include <QtGui/QOpenGLFunctions>
#include <QMutex>
#include <QPoint>

#include <AIS_InteractiveContext.hxx>

//Renderer class
class occtRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:

    enum ContextAction
    {
        NoAction, Panning, Rotation, Zooming
    };


    occtRenderer()
    {
        m_isInit = false;
        m_action = NoAction;
    }

    ~occtRenderer(){}

    void setViewportSize(const QSize &size)
    {
        m_viewportSize = size;
        resize();
    }

    void setWindow(QQuickWindow *window) { m_window = window; }

    void resize();

    void mouseMove(int x, int y);

    void onRightButtonPressed(int x, int y);
    void onRightButtonReleased(int x, int y);

    void onMiddleButtonPressed(int x, int y);
    void onMiddleButtonReleased(int x, int y);

    void onWheel(int angleDelta);

protected:
    void Init();
    void makeTestCone();

public slots:
    void paint();

private:
    QSize m_viewportSize;
    QQuickWindow *m_window;
    bool m_isInit;
    ContextAction m_action;

    QMutex m_mutex;        //mutex for interconnection with rendering thread
    QPoint m_startPoint, m_endPoint, m_cursor;


    //OCC viewer.
    Handle(V3d_Viewer) myViewer;

    //OCC view.
    Handle(V3d_View) myView;

    //OCC context.
    Handle(AIS_InteractiveContext) myContext;

};

//View item
class occtView : public QQuickItem
{
    Q_OBJECT

public:
    occtView();

    void setSize(QSize size);

public slots:
    void sync();
    void cleanup();

    Q_INVOKABLE void mouseMove(int x, int y);

    Q_INVOKABLE void onRightButtonPressed(int x, int y);
    Q_INVOKABLE void onRightButtonReleased(int x, int y);

    Q_INVOKABLE void onMiddleButtonPressed(int x, int y);
    Q_INVOKABLE void onMiddleButtonReleased(int x, int y);

    Q_INVOKABLE void onWheel(int angleDelta);

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    occtRenderer *m_renderer;
};

#endif // OCCTVIEW_H
