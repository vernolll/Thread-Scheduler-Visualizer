#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "gui/GUIController.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

#if defined(Q_OS_WIN)
    app.setWindowIcon(QIcon(":/resources/icons/app_icon.ico"));
#elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    app.setWindowIcon(QIcon(":/resources/icons/app_icon.svg"));
#else
    app.setWindowIcon(QIcon(":/resources/icons/app_icon.svg"));
#endif

    if (app.windowIcon().isNull()) 
    {
        qWarning() << "WARNING: Application icon failed to load!";
    }

    QQmlApplicationEngine engine;

    gui::GUIController guiController;
    engine.rootContext()->setContextProperty("guiController", &guiController);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}