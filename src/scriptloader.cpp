#include "scriptloader.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include <stdexcept>
#include <QDir>
#include <QCoreApplication>
//#include <QApplication>

// Private implementation class
class scriptLoader::Private {
public:
    QMutex mutex;
    std::atomic<bool> running{false};
    std::atomic<bool> paused{false};
    std::atomic<int>  stepDelay{20};
    QWaitCondition pauseCondition;
};

scriptLoader::scriptLoader(std::shared_ptr<GMap> map, QObject *parent)
    : QObject(parent), Map(map) {

    d = std::unique_ptr<Private>(new Private());

    // python setup
    QString exeDir = QCoreApplication::applicationDirPath();
    QString pythonDir = exeDir + "/python";
    qputenv("PYTHONHOME", pythonDir.toUtf8());
    QString pythonPath = pythonDir + ";" + pythonDir + "/python311.zip";
    qputenv("PYTHONPATH", pythonPath.toUtf8());
    Py_SetPythonHome(Py_DecodeLocale(pythonDir.toStdString().c_str(), nullptr));

    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

}
scriptLoader::~scriptLoader() {
    stopAlgorithm();
    unloadPython();
    if (Py_IsInitialized()){
        Py_Finalize();
    }
}

bool scriptLoader::loadAlgorithm(int choice) {

    try{
    switch (choice){
    case 0: // load local choice
    {
        QString filename_ = getPath();
        qDebug() << "Raw filename:" << filename_;

        if (filename_.isEmpty()) {
            qDebug() << "Operation cancelled by user";
            return false;  // Now properly scoped
        }
        if (!QFile::exists(filename_)) {
            qDebug() << "File does not exist";
            return false;
        }
        if(algorithmLoaded){
            unloadPython();
        }
        loadPython(filename_);
        algorithmLoaded = true;
        return true;
    }
        break;
    case 1: // load A* choice
    {
        QString filename_ = QDir::currentPath() + "/algorithms/A_star.py";
        qDebug() << "Raw filename:" << filename_;

        if (filename_.isEmpty()) {
            qDebug() << "Operation cancelled by user";
            return false;  // Now properly scoped
        }
        if (!QFile::exists(filename_)) {
            qDebug() << "File does not exist";
            return false;
        }
        if(algorithmLoaded){
            unloadPython();
        }
        loadPython(filename_);
        algorithmLoaded = true;
        return true;
    }
        break;
    case 2: // load Dijkstra choice
    {
        QString filename_ = QDir::currentPath() + "/algorithms/Dijkstra.py";
        qDebug() << "Raw filename:" << filename_;

        if (filename_.isEmpty()) {
            qDebug() << "Operation cancelled by user";
            return false;  // Now properly scoped
        }
        if (!QFile::exists(filename_)) {
            qDebug() << "File does not exist";
            return false;
        }
        if(algorithmLoaded){
            unloadPython();
        }
        loadPython(filename_);
        algorithmLoaded = true;
        return true;
    }
        break;
    case 3: // load Breadth-First Search(multi-node) choice
    {
        QString filename_ = QDir::currentPath() + "/algorithms/Breadth-First Search(multi-node).py";
        qDebug() << "Raw filename:" << filename_;

        if (filename_.isEmpty()) {
            qDebug() << "Operation cancelled by user";
            return false;  // Now properly scoped
        }
        if (!QFile::exists(filename_)) {
            qDebug() << "File does not exist";
            return false;
        }
        if(algorithmLoaded){
            unloadPython();
        }
        loadPython(filename_);
        algorithmLoaded = true;
        return true;
    }
        break;
    case 4: // load Breadth-First Search choice
    {
        QString filename_ = QDir::currentPath() + "/algorithms/Breadth-First Search.py";
        qDebug() << "Raw filename:" << filename_;

        if (filename_.isEmpty()) {
            qDebug() << "Operation cancelled by user";
            return false;  // Now properly scoped
        }
        if (!QFile::exists(filename_)) {
            qDebug() << "File does not exist";
            return false;
        }
        if(algorithmLoaded){
            unloadPython();
        }
        loadPython(filename_);
        algorithmLoaded = true;
        return true;
    }
        break;
    case 5: // load Depth-First Search choice
    {
        QString filename_ = QDir::currentPath() + "/algorithms/Depth-First Search.py";
        qDebug() << "Raw filename:" << filename_;

        if (filename_.isEmpty()) {
            qDebug() << "Operation cancelled by user";
            return false;  // Now properly scoped
        }
        if (!QFile::exists(filename_)) {
            qDebug() << "File does not exist";
            return false;
        }
        if(algorithmLoaded){
            unloadPython();
        }
        loadPython(filename_);
        algorithmLoaded = true;
        return true;
    }
        break;
    default:
        return false;
        break;
    }
        }
    catch (...){
        qDebug() << "[scriptLoader] loadAlgorithm error with choice:" << choice;
        }
}
void scriptLoader::loadPython(const QString &scriptPath){

    steps = 0;
    nodes = 0;
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
    pyAlgoName = PyObject_GetAttrString(pyAlgorithm, "getName");
    if (!pyAlgoName || !PyCallable_Check(pyAlgoName)) {
        qDebug() << "[loadPython]: no name was found";

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
    Py_XDECREF(pyAlgoName);
    Py_XDECREF(stepFunc);
    Py_XDECREF(pyAlgorithm);
    Py_XDECREF(pyMap);
    Py_XDECREF(pyStart);
    Py_XDECREF(pyEnd);

    stepFunc = pyAlgorithm = pyMap = pyStart = pyEnd = nullptr;

}

QString scriptLoader::getPath(){

    QString filename = QFileDialog::getOpenFileName(nullptr,
                                                    "Open python script",
                                                    "algorithms/",
                                                    "Python Files (*.py)");

    if (filename.isEmpty()){ return QString();}
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
            Step();
        }
        catch (const std::exception& e) {
            qDebug() << "Algorithm error: " << e.what();
            stopAlgorithm();
            return;
        }
        QThread::msleep(d->stepDelay);
    }
}
void scriptLoader::Step(){
    //if(!d->running){return;}
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
    //QApplication::processEvents();
    Py_XDECREF(result);
    PyObject* isRunning = PyObject_GetAttrString(pyAlgorithm, "isRunning");
    PyObject* check =  PyObject_CallObject(isRunning, nullptr);
    steps++;
    emit stepsTaken(steps);
    // Update running state
    {
        QMutexLocker stateLocker(&d->mutex);
        d->running = PyObject_IsTrue(check);
        if(!PyObject_IsTrue(check)){
            emit endReached();
        }
    }
    Py_XDECREF(isRunning);
    Py_XDECREF(check);
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
        if(type == 3){
            nodes++;
        }else if(type == 2){
            pathDistance++;
        }
    }
    emit nodesVisited(nodes);
    emit pathDistanceSignal(pathDistance);

}

void scriptLoader::startAlgorithm() {

    if (d->running) {
        if(d->paused){
            qDebug() << "Algorithm is resuming.";
            resumeAlgorithm();
            return;
        }
        qDebug() << "Algorithm is already running.";
        if (!thread || !thread->isRunning()) {
            qDebug() << "Thread is not running, resetting state.";
             d->running = false;
        } else {
            return;
        }
    }

    QMutexLocker locker(&d->mutex);

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
    }
    thread = nullptr;
    unloadPython();
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
QString scriptLoader::getAlgName(){
    PyObject* pyName = PyObject_CallObject(pyAlgoName, nullptr);
    if (!pyName) {
        qDebug() << "[getAlgName]: Python function call failed!";
        PyErr_Print();
        return QString();
    }
    // Convert Python string to UTF-8
    const char* cStr = PyUnicode_AsUTF8(pyName);
    return QString::fromUtf8(cStr);
}
void scriptLoader::setStepDelay(double delay){
    QMutexLocker locker(&d->mutex);

    d->stepDelay = static_cast<int>(delay * 1000.0); // seconds to milliseconds
}

bool scriptLoader::isRunning(){

    return d->running;
}
bool scriptLoader::isPaused(){
    return d->paused;
}
bool scriptLoader::isLoaded() const{
    return algorithmLoaded;
}
int scriptLoader::getPathDistance() const{
    return pathDistance;
}
