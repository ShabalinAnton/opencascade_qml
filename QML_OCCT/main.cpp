#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtQuick/QQuickView>
#include "occtview.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<occtView>("OCCTUnderQML", 1, 0, "OCCTView");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
