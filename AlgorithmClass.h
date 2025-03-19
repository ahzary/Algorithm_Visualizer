#ifndef ALGORITHMCLASS_H
#define ALGORITHMCLASS_H
#include "gmap.h"
#include <memory>


#ifdef BUILD_DLL  // Defined only when compiling DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

class DLL_EXPORT  Algorithm {
public:
    virtual ~Algorithm() = default;
    virtual void algorithmRun() = 0;
    virtual void resetAlgorithm() = 0;
    virtual void set_Steps_per_seconds(int stps) = 0;

protected:
    std::shared_ptr<GMap> Map ;
    int steps_per_seconds=1;
    int stepsTaken = 0;
    int nodesVisited = 0;
    int executionTime = 0; // in seconds
    bool pause = false;
    QString name;


};

#ifdef BUILD_DLL
extern "C" DLL_EXPORT Algorithm* createAlgorithm();
#endif

#endif // ALGORITHMCLASS_H
