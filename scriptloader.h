#ifndef SCRIPTLOADER_H
#define SCRIPTLOADER_H

#undef slots
#define PY_SSIZE_T_CLEAN  // Required for Python 3 API
#include <Python.h>
#define slots

#include "gmap.h"
#include <qvector.h>
#include <memory>
#include <QPair>
#include <QObject>
#include <QProcess>
#include <windows.h>
#include <QThread>
#include <QDebug>

class scriptLoader : public QObject
{
    Q_OBJECT

public:
    explicit scriptLoader(std::shared_ptr<GMap> map, QObject *parent = nullptr);
    ~scriptLoader();
    void loadAlgorithm(int choice);
    void checkLanguage();
    void loadDll(const QString &dllPath);
    void algorithmRunning();

    void startAlgorithm();
    void stopAlgorithm();
    void pauseAlgorithm();
    void resumeAlgorithm();


private:

    class Private;
    std::unique_ptr<Private> d;


    std::vector<PyObject*> allPyObjects;
    QThread* thread = nullptr;
    std::shared_ptr<GMap> Map ;
    QString getPath();
    //bool running = false;
    //bool isPaused = false;
    // object to interact with python script
    PyObject* pyAlgorithm = nullptr;
    PyObject* stepFunc = nullptr;
    PyObject* pyMap = nullptr;
    PyObject* pyStart = nullptr;
    PyObject* pyEnd = nullptr;

    std::vector<std::vector<int>> processedData;
    void loadPython(const QString &scriptPath);
    void unloadPython();
    void mapPyTransform();
    void pyStepCallBack(PyObject* callBack);
    void MapUpdate(const std::vector<std::vector<int>>& data);


};

#endif // SCRIPTLOADER_H
