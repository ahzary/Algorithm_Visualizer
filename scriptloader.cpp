#include "scriptloader.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include <stdexcept>

// Private implementation class
class scriptLoader::Private {
public:
    QMutex mutex;
    std::atomic<bool> running{false};
    std::atomic<bool> paused{false};
    QWaitCondition pauseCondition;
};

scriptLoader::scriptLoader(std::shared_ptr<GMap> map, QObject *parent)
    : QObject(parent), Map(map) {

    d = std::unique_ptr<Private>(new Private());

    if (Py_IsInitialized()){
        Py_Finalize();
    }
    Py_Initialize();

}
scriptLoader::~scriptLoader() {
    stopAlgorithm();
    unloadPython();
    if (Py_IsInitialized()){
        Py_Finalize();
    }
}

void scriptLoader::loadAlgorithm(int choice) {

    QString filename_ = getPath();
    if (filename_.isEmpty())
        return;
    loadPython(filename_);
}

void scriptLoader::loadPython(const QString &scriptPath){

    //get the script path and name and put them in std::string format
    //std::string py_path = QFileInfo(scriptPath).absoluteFilePath().toStdString();
    std::string py_path = scriptPath.toStdString();
    std::string py_name = QFileInfo(scriptPath).baseName().toStdString();

    // Import importlib
    PyObject* importlib = PyImport_ImportModule("importlib.util");
    if (!importlib) {
        PyErr_Print();
        qDebug() << "[loadPython]: Failed to import importlib.util";
        return;
    }

    // Get function references
    PyObject* specFunc = PyObject_GetAttrString(importlib, "spec_from_file_location");
    PyObject* moduleFunc = PyObject_GetAttrString(importlib, "module_from_spec");
    if (!specFunc || !moduleFunc) {
        PyErr_Print();
        qDebug() << "[loadPython]: Failed to retrieve functions from importlib";
        Py_XDECREF(importlib);
        return;
    }

    //load the python algorithm class script
    PyObject* spec = PyObject_CallFunction(specFunc,"ss", py_name.c_str(), py_path.c_str());
    if (!spec) {
        PyErr_Print();
        qDebug() << "[loadPython]: Failed to create module spec";
        Py_XDECREF(specFunc);
        Py_XDECREF(moduleFunc);
        Py_XDECREF(importlib);
        return;
    }

    PyObject* module = PyObject_CallFunction(moduleFunc, "O", spec);
    PyObject* loader = PyObject_GetAttrString(spec, "loader");
    PyObject_CallMethod(loader, "exec_module", "O", module);

    qDebug() << "[loadPython]: Loader LOADED";

    // Get Algorithm from the python module
    PyObject* pAlgorithm_class = PyObject_GetAttrString(module, "Algorithm");
    if (!pAlgorithm_class || !PyCallable_Check(pAlgorithm_class)) {
        qDebug() << "Failed to get class Algorithm\n";
        PyErr_Print();
        return;
    }
    qDebug() << "[loadPython]: pAlgorithm_class loaded";
    // prepare the map to send it to python
    mapPyTransform();
    if(!pyMap){
        qDebug() << "[loadPython]: failed to load pyMap";
        PyErr_Print();
        return;
    }
    qDebug() << "[loadPython]: pyMap loaded";
    pyAlgorithm = PyObject_CallFunctionObjArgs(pAlgorithm_class, pyMap,pyStart, pyEnd, nullptr);

    if (!pyAlgorithm) {
        qDebug() << "[loadPython]: Failed to initialize Algorithm instance\n";
        PyErr_Print();
        return;
    }
    qDebug() << "[loadPython]: class loaded";
    stepFunc = PyObject_GetAttrString(pyAlgorithm, "step");
    if (!stepFunc || !PyCallable_Check(stepFunc)) {
        qDebug() << "[loadPython]: Failed to get step function\n";
        PyErr_Print();
        return;
    }


    //startAlgorithm();

    // For temporary objects only (not members):
    allPyObjects.emplace_back(importlib);
    allPyObjects.emplace_back(specFunc);
    allPyObjects.emplace_back(moduleFunc);
    allPyObjects.emplace_back(spec);
    allPyObjects.emplace_back(module);
    allPyObjects.emplace_back(loader);
    allPyObjects.emplace_back(pAlgorithm_class);

}
void scriptLoader::unloadPython(){

    // Cleanup temp objs
    for(int i=0;i<allPyObjects.size();i++){
        Py_XDECREF(allPyObjects[i]);
    }
    allPyObjects.clear();

    // clean up class members
    Py_XDECREF(stepFunc);
    Py_XDECREF(pyAlgorithm);
    Py_XDECREF(pyMap);
    Py_XDECREF(pyStart);
    Py_XDECREF(pyEnd);

    stepFunc = pyAlgorithm = pyMap = pyStart = pyEnd = nullptr;

    // what should be cleared
    /*
    Py_XDECREF(pyEnd);
    Py_XDECREF(pyStart);
    Py_XDECREF(stepFunc);
    Py_XDECREF(pyMap);
    Py_XDECREF(pyAlgorithm);
    Py_XDECREF(pAlgorithm_class);
    Py_XDECREF(loader);
    Py_XDECREF(module);
    Py_XDECREF(spec);
    Py_XDECREF(moduleFunc);
    Py_XDECREF(specFunc);
    Py_XDECREF(importlib);
*/
}

QString scriptLoader::getPath(){

    QString filename = QFileDialog::getOpenFileName(nullptr,
                                                    "Open python script",
                                                    "",
                                                    "Python Files (*.py)");

    if (filename.isEmpty()) return 0;
    return filename;
}
void scriptLoader::mapPyTransform(){
    //saftey layer
    if (!Map) {
        qDebug() << "Error: No map is loaded";
        return;
    }
    int gridSize = Map->getGridSize();
    pyMap = PyList_New(gridSize);
    qDebug() <<"[mapPyTransform]: pyMap created";
    for(int i=0;i<gridSize;i++){

        PyObject* pyMapRow = PyList_New(gridSize);
        for(int j=0;j<gridSize;j++){
            PyList_SetItem(pyMapRow, j, PyLong_FromLong(Map->type_grid[i][j]));
        }
        //refrence steal, no need for xdecref
        PyList_SetItem(pyMap, i, pyMapRow);
    }
    // Set start position
    pyStart = PyList_New(2);
    if (!pyStart) {
        qDebug() << "Failed to allocate pyStart";
        return;
    }
    std::array<int, 2> startPos = Map->getStartSquare();
    PyList_SetItem(pyStart, 0, PyLong_FromLong(startPos[0]));
    PyList_SetItem(pyStart, 1, PyLong_FromLong(startPos[1]));

    // Set end position
    pyEnd = PyList_New(2);
    if (!pyEnd) {
        qDebug() << "Failed to allocate pyEnd";
        return;
    }
    std::array<int, 2> endPos = Map->getEndSquare();
    PyList_SetItem(pyEnd, 0, PyLong_FromLong(endPos[0]));
    PyList_SetItem(pyEnd, 1, PyLong_FromLong(endPos[1]));
    qDebug() << "[mapPyTransform]: mapPyTransform ended";
}
void scriptLoader::algorithmRunning(){
    qDebug()<<"[algorithmRunning]: algorithm running";

    while(d->running){
         QMutexLocker locker(&d->mutex);
        while(d->paused){
            d->pauseCondition.wait(&d->mutex);
        }

    locker.unlock();
        try {
            PyObject* result =  PyObject_CallObject(stepFunc, nullptr);
            static int steppy = 0;
            qDebug() << "[algorithmRunning]: result recieved :" << steppy;
            steppy++;
            if (!result) {
                qDebug() << "Error: step function returned null";
                PyErr_Print();  // Print Python error details
                return;
            }
            pyStepCallBack(result);
            qDebug() << "[algorithmRunning]: going to update map";
            MapUpdate(processedData);
            qDebug() << "[algorithmRunning]: map update done";

            Py_XDECREF(result);
            PyObject* goalReached = PyObject_GetAttrString(pyAlgorithm, "goalReached");
            PyObject* check =  PyObject_CallObject(goalReached, nullptr);

            // Update running state
            {
                QMutexLocker stateLocker(&d->mutex);
                d->running = PyObject_IsTrue(check);
            }
            Py_XDECREF(goalReached);
            Py_XDECREF(check);
        }
        catch (const std::exception& e) {
            qDebug() << "Algorithm error: " << e.what();
            stopAlgorithm();
            return;
        }
        QThread::msleep(50);
    }
}


void scriptLoader::pyStepCallBack(PyObject* callBack){

    /*
     data format:
     [x,y,new type],[x,y,new type],[x,y,new type],
        ]
    */
    if (!PyList_Check(callBack)) {
        qDebug() << "Error: Expected a list of lists.";
        PyErr_Print();
        return;
    }
    int outerSize = PyList_Size(callBack);
    processedData.resize(outerSize);

    for (int i = 0; i < outerSize; i++) {
        PyObject* rowObj = PyList_GetItem(callBack, i); // Borrowed reference

        if (!PyList_Check(rowObj)) {
            qDebug() << "Error: Expected a list inside list.";
            PyErr_Print();
            return;
        }

        int innerSize = PyList_Size(rowObj);
        processedData[i].resize(innerSize);

        for (int j = 0; j < innerSize; j++) {
            PyObject* valueObj = PyList_GetItem(rowObj, j); // Borrowed reference

            if (!PyLong_Check(valueObj)) {
                qDebug() << "Error: Expected an integer inside the list.";
                PyErr_Print();
                return;
            }

            processedData[i][j] = PyLong_AsLong(valueObj);
        }
    }

    qDebug() << "[pyStepCallBack]: Processed Python data successfully";

}

void scriptLoader::MapUpdate(const std::vector<std::vector<int>>& data){

    for(int i=0; i<data.size();i++ ){

        int x = data[i][0];
        int y = data[i][1];
        int type = data[i][2];
        Map->type_grid[x][y] = type;
        Map->Grid[x][y]->type = type;
        Map->Grid[x][y]->update_color();

        }

}

void scriptLoader::startAlgorithm() {

    QMutexLocker locker(&d->mutex);

    if (d->running) {
        qDebug() << "Algorithm is already running.";
        return;
    }

    d->running = true;
    d->paused = false;

    // Create and start thread
    thread = QThread::create([this]() { algorithmRunning(); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();

}
void scriptLoader::stopAlgorithm() {
    if (!d->running) {
        qDebug() << "[stopAlgorithm]: Algorithm is not running.";
        return;
    }

    qDebug() << "[stopAlgorithm]: Stopping algorithm...";

    d->running = false;
    d->paused = false;

    // Wake up any waiting threads
    d->pauseCondition.wakeAll();
    // shutdown thread
    if (thread) {
        thread->quit();
        thread->wait();
        thread = nullptr;
    }
}

void scriptLoader::pauseAlgorithm() {
    QMutexLocker locker(&d->mutex);
    d->paused = true;
}

void scriptLoader::resumeAlgorithm() {
    QMutexLocker locker(&d->mutex);
    d->paused = false;

    // Wake up waiting threads
    d->pauseCondition.wakeAll();
}
