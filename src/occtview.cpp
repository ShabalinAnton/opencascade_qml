#include <windows.h>
#include <WNT_Window.hxx>

#include "OcctView.h"

OcctView::OcctView(QQuickItem* parent) :
	QQuickItem(parent)
{
	QObject::connect(this, &QQuickItem::windowChanged, this, &OcctView::onWindowChanged, Qt::DirectConnection);
}

void OcctView::onWindowChanged(QQuickWindow* window)
{
	if (!window)
		return;

	QObject::connect(window, &QQuickWindow::beforeSynchronizing, this, &OcctView::onSynchronizing, Qt::DirectConnection);
	QObject::connect(window, &QQuickWindow::sceneGraphInvalidated, this, &OcctView::onInvalidating, Qt::DirectConnection);

	window->setClearBeforeRendering(false);
}

void OcctView::onSynchronizing()
{
	// If the viewer is not yet initialized, initialize it.
	if (m_viewer.IsNull())
	{
		this->initializeViewer(Aspect_Drawable(this->window()->winId()));
		QObject::connect(this->window(), &QQuickWindow::beforeRendering, this, &OcctView::onRendering, Qt::DirectConnection);
	}

	// Get the control position and size.
	QPoint viewportPos = this->mapToGlobal(QPointF(0, 0)).toPoint();
	QSize viewportSize = this->size().toSize();

	// Check if the viewport needs to be resized.
	if (viewportPos.x() != m_viewportPos.x() || viewportPos.y() != m_viewportPos.y())
		m_view->MustBeResized();
	if (viewportSize.width() != m_viewportSize.width() || viewportSize.height() != m_viewportSize.height())
	{
		m_view->MustBeResized();
		m_view->Invalidate();
	}

	// Store the current pos and size.
	m_viewportPos = viewportPos;
	m_viewportSize = viewportSize;
}

void OcctView::onInvalidating()
{
	m_view.Nullify();
	m_context.Nullify();
	m_viewer.Nullify();
}

void OcctView::onRendering()
{
	if (m_view.IsNull())
		return;

	m_mutex.lock();
	// TODO: Handle user input.
	m_mutex.unlock();

	// Redraw the view.
	m_view->Redraw();
}

void OcctView::initializeViewer(const Aspect_Drawable& drawable)
{
	Q_ASSERT(m_viewer.IsNull());

	// Request device and render context.
	auto deviceContext = wglGetCurrentDC();
	auto renderContext = wglGetCurrentContext();

	if (drawable == nullptr || deviceContext == nullptr || renderContext == nullptr)
		return;

	// Setup display driver.
	Handle(Aspect_DisplayConnection) display;
	Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(display, Standard_False);
	driver->ChangeOptions().buffersNoSwap = Standard_True;
	//aDriver->ChangeOptions().glslWarnings  = Standard_True;

	// Create and setup the viewer.
	m_viewer = new V3d_Viewer(driver);
	m_viewer->SetDefaultBackgroundColor(Quantity_NOC_GRAY50);
	m_viewer->SetDefaultLights();
	m_viewer->SetLightOn();

	// Create and setup interactivity context.
	m_context = new AIS_InteractiveContext(m_viewer);
	m_context->SetDisplayMode(AIS_Shaded, false);

	// Create and setup view.
	Handle(WNT_Window) window = new WNT_Window(drawable);
	m_view = m_viewer->CreateView();

	m_view->SetImmediateUpdate(Standard_False);
	m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(renderContext));
	m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.1, V3d_ZBUFFER);
}