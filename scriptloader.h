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
//#include "AlgorithmClass.h"
#include <windows.h>
#include <QDebug>

class scriptLoader : public QObject
{
    Q_OBJECT

public:
    explicit scriptLoader(std::shared_ptr<GMap> map, QObject *parent = nullptr);
    ~scriptLoader();
    void loadAlgorithm(int choice);
    void checkLanguage();
    void compileCpp(const QString &filePath);
    void loadDll(const QString &dllPath);
    void runAlgorithm();



private:
    std::shared_ptr<GMap> Map ;
    //std::shared_ptr<Algorithm> alg;

    QString compiledDllPath;
    HINSTANCE libHandle = nullptr;
    void loadPython(const QString &scriptPath);
    void unloadPython(const QString &scriptPath);
    void mapPyTransform();
    QString getPath();
};

#endif // SCRIPTLOADER_H
