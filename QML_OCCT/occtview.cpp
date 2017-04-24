#include "occtview.h"

#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>

#include <QtQuick/qquickwindow.h>

#include "occtwindow.h"

// occ header files.
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>

#include <Aspect_DisplayConnection.hxx>

#include <TopoDS.hxx>
#include <AIS_Shape.hxx>

#include <BRepPrimAPI_MakeCone.hxx>

#include <WNT_Window.hxx>

static Handle(OpenGl_GraphicDriver)& GetGraphicDriver()
{
  static Handle(OpenGl_GraphicDriver) aGraphicDriver;
  return aGraphicDriver;
}

occtView::occtView()
    :m_renderer(0)
{
    connect(this, &QQuickItem::windowChanged, this, &occtView::handleWindowChanged);
}


void occtView::sync()
{
    if (!m_renderer) {
        m_renderer = new occtRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &occtRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setWindow(window());
    m_renderer->paint();
}

void occtView::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
}

void occtView::mouseMove(int x, int y)
{    
    if (m_renderer) m_renderer->mouseMove(x,y);
}

void occtView::onRightButtonPressed(int x, int y)
{
    if (m_renderer) m_renderer->onRightButtonPressed(x,y);
}

void occtView::onRightButtonReleased(int x, int y)
{
    if (m_renderer) m_renderer->onRightButtonReleased(x,y);
}

void occtView::onMiddleButtonPressed(int x, int y)
{
    if (m_renderer) m_renderer->onMiddleButtonPressed(x,y);
}

void occtView::onMiddleButtonReleased(int x, int y)
{
    if (m_renderer) m_renderer->onMiddleButtonReleased(x,y);
}

void occtView::onWheel(int angleDelta)
{
    if (m_renderer) m_renderer->onWheel(angleDelta);
}

void occtView::setSize(QSize size)
{
    if (m_renderer) m_renderer->setViewportSize(size);
}

void occtView::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &occtView::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &occtView::cleanup, Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void occtRenderer::Init()
{
    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection = new Aspect_DisplayConnection();

    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull())
    {        
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);

        //This is important!
        GetGraphicDriver()->ChangeOptions().buffersNoSwap = Standard_True;
    }

    // Create V3dViewer and V3d_View
     myViewer = new V3d_Viewer(GetGraphicDriver(), Standard_ExtString("viewer"));
     // Set up lights etc
     myViewer->SetDefaultLights();
     myViewer->SetLightOn();

     myView = myViewer->CreateView();

    // Get window handle. This returns something suitable for all platforms.
     WId window_handle = (WId) m_window->winId();

     //Creating dummy window
     Handle(occtWindow) wind = new occtWindow(m_viewportSize.width(), m_viewportSize.height());
     wind->SetHandle((Aspect_Handle)window_handle);

     //Getting current OpenGL Context using Qt Platform Interface:
     QPlatformNativeInterface *iface = QGuiApplication::platformNativeInterface();
     HGLRC anContext = (HGLRC)iface->nativeResourceForContext("renderingContext", QOpenGLContext::currentContext());

     //Also it can be done as follows
     //For Win:
     //auto anContext = wglGetCurrentContext();
     //For Linux:
     //auto anContext = glXGetCurrentContext();

     myView->SetWindow(wind, (Aspect_RenderingContext )anContext);
     if (!wind->IsMapped()) wind->Map();

     myView->SetBackgroundColor(Quantity_NOC_GRAY50);
     myView->MustBeResized();
     myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

     // Create AISInteractiveContext
     myContext = new AIS_InteractiveContext(myViewer);
     myContext->SetDisplayMode(AIS_Shaded);

     makeTestCone();
}

void occtRenderer::makeTestCone()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape aTopoReducer = BRepPrimAPI_MakeCone(anAxis, 3.0, 1.5, 5.0).Shape();
    Handle(AIS_Shape) anAisReducer = new AIS_Shape(aTopoReducer);

    anAisReducer->SetColor(Quantity_NOC_BISQUE);

    anAxis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(anAxis, 3.0, 0.0, 5.0).Shape();
    Handle(AIS_Shape) anAisCone = new AIS_Shape(aTopoCone);

    anAisCone->SetColor(Quantity_NOC_CHOCOLATE);

    myContext->Display(anAisReducer);
    myContext->Display(anAisCone);

    myView->FitAll();
}

void occtRenderer::paint()
{
    if (!m_isInit)
    {
        m_isInit = true;
        initializeOpenGLFunctions();
        Init();
    }

    if (!myContext.IsNull())
    {
        m_mutex.lock();

        if (m_action == Rotation && m_cursor != m_startPoint)
        {
            myView->StartRotation(m_startPoint.x(), m_startPoint.y());
            myView->Rotation(m_endPoint.x(), m_endPoint.y());
            m_startPoint = m_endPoint;
        }


        if (m_action == Panning)
        {
            myView->Pan(m_endPoint.x() - m_startPoint.x(), m_startPoint.y() - m_endPoint.y());
            m_startPoint = m_endPoint;
        }

        if (m_action == Zooming)
        {
            myView->StartZoomAtPoint(m_startPoint.x(),m_startPoint.y());
            myView->ZoomAtPoint(m_startPoint.x(),m_startPoint.y(), m_endPoint.x(), m_endPoint.y());
        }

        m_mutex.unlock();

        myView->Redraw();

    }

    //  Generally useful for when mixing with raw OpenGL.
    m_window->resetOpenGLState();
}

void occtRenderer::resize()
{
    if (myView.IsNull()) return;

    m_mutex.lock();

    Handle(occtWindow) aWindow = Handle(occtWindow)::DownCast (myView->Window());
    aWindow->SetSize (m_viewportSize.width(), m_viewportSize.height());
    myView->MustBeResized();

    m_mutex.unlock();
    m_window->update();
}

void occtRenderer::mouseMove(int x, int y)
{
    m_cursor = QPoint(x,y);

    if (m_action == Rotation || m_action == Panning)
    {
        m_mutex.lock();
        m_endPoint = QPoint(x,y);
        m_mutex.unlock();

        m_window->update();
    }

}

void occtRenderer::onRightButtonPressed(int x, int y)
{    
    if (myView.IsNull()) return;

    m_mutex.lock();

    m_startPoint = QPoint(x,y);
    m_action = Panning;

    m_mutex.unlock();
}

void occtRenderer::onRightButtonReleased(int x, int y)
{
    m_mutex.lock();

    m_action = NoAction;
    m_endPoint = QPoint(x,y);

    m_mutex.unlock();
}

void occtRenderer::onMiddleButtonPressed(int x, int y)
{
    if (myView.IsNull()) return;

    m_mutex.lock();
    m_startPoint = QPoint(x,y);
    m_endPoint = QPoint(x,y);
    m_action = Rotation;
    m_mutex.unlock();

}

void occtRenderer::onMiddleButtonReleased(int x, int y)
{
    m_mutex.lock();
    m_action = NoAction;
    m_endPoint = QPoint(x,y);
    m_mutex.unlock();
}

void occtRenderer::onWheel(int angleDelta)
{
    m_mutex.lock();
    m_action = Zooming;

    m_startPoint = m_cursor;

    Standard_Integer aFactor = 5;

    Standard_Integer aX = m_startPoint.x();
    Standard_Integer aY = m_startPoint.y();

    if (angleDelta > 0)
    {
       aX += aFactor;
       aY += aFactor;
    }
    else
    {
       aX -= aFactor;
       aY -= aFactor;
    }
    m_endPoint.setX(aX);
    m_endPoint.setY(aY);

    m_mutex.unlock();

    m_window->update();
}
