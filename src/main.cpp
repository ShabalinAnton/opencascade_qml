#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>

#include "OcctView.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

    qmlRegisterType<occtView>("OpenCascade", 7, 3, "OcctView");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
