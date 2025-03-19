#include "scriptloader.h"
#include <QFileDialog>
#include <QFileInfo>

scriptLoader::scriptLoader(std::shared_ptr<GMap> map, QObject *parent)
    : QObject(parent), Map(map) {

    Py_Initialize();

}
scriptLoader::~scriptLoader() {
    if (libHandle) {
        FreeLibrary(libHandle);
    }
    if (Py_IsInitialized()){
        Py_Finalize();
    }
}

void scriptLoader::loadAlgorithm(int choice) {

    loadPython(getPath());
}

void scriptLoader::loadPython(const QString &scriptPath){

    //get the script path and name and put them in std::string format
    std::string py_path = QFileInfo(scriptPath).absolutePath().toStdString();
    std::string py_name = QFileInfo(scriptPath).baseName().toStdString();

    // add the script path to python system path so it can be found
    PyObject* sysPath = PySys_GetObject("path");
    PyList_Append(sysPath, PyUnicode_DecodeFSDefault(py_path.c_str()));
    PyObject* pModule = PyImport_ImportModule(py_name.c_str());

    if (!pModule) {
        qDebug() << "Failed to import Python module:" << QString::fromStdString(py_name);
        PyErr_Print();
        return;
    }

    // define function name
    PyObject* pFunc = PyObject_GetAttrString(pModule, "step");

    // check if the function exist
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pArg = PyLong_FromLong(42);  // Example: Passing integer 42
        PyObject* pValue = PyObject_CallObject(pFunc, PyTuple_Pack(1, pArg));

        if (!pValue) {
            qDebug() << "Python function call failed.";
            PyErr_Print();
        } else {
            long result = PyLong_AsLong(pValue);
            qDebug() << "Python function returned:" << result;
        }
        Py_XDECREF(pArg);
        Py_XDECREF(pValue);
    } else {
        qDebug() << "Function not found or not callable.";
        PyErr_Print();
    }
    // Cleanup

    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);

    unloadPython(scriptPath);
}

void scriptLoader::unloadPython(const QString &scriptPath) {
    std::string py_path = QFileInfo(scriptPath).absolutePath().toStdString();  // Get directory path

    PyObject* sysPath = PySys_GetObject("path");
    if (!sysPath || !PyList_Check(sysPath)) {
        qDebug() << "Failed to get Python sys.path";
        return;
    }

    PyObject* pyDir = PyUnicode_DecodeFSDefault(py_path.c_str());
    Py_ssize_t pathSize = PyList_Size(sysPath);
    for (Py_ssize_t i = 0; i < pathSize; i++) {
        PyObject* item = PyList_GetItem(sysPath, i);  // Borrowed reference, no need to DECREF
        if (PyObject_RichCompareBool(item, pyDir, Py_EQ)) {
            PySequence_DelItem(sysPath, i);
            qDebug() << "Removed path from Python sys.path:" << QString::fromStdString(py_path);
            break;  // Stop after removing
        }
    }

    Py_DECREF(pyDir);
}

QString scriptLoader::getPath(){

    QString filename = QFileDialog::getOpenFileName(nullptr,
                                                    "Open python script",
                                                    "",
                                                    "Python Files (*.py)");

    return filename;
}
void scriptLoader::mapPyTransform(){
    //saftey layer
    if (!Map) {
        qDebug() << "Error: No map is loaded";
        return;
    }

    int gridSize = Map->getGridSize();
    PyObject* pyMap = PyList_New(gridSize);
    for(int i=0;i<gridSize;i++){

        PyObject* pyMapRow = PyList_New(gridSize);
        for(int j=0;j<gridSize;j++){
            PyList_SetItem(pyMapRow, j, PyLong_FromLong(Map->type_grid[i][j]));
        }
        PyList_SetItem(pyMap, i, pyMapRow);
    }
    //send to python script

    //clean memory
    Py_DECREF(pyMap);

}
