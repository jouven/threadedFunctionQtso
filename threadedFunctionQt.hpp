#ifndef THREADEDFUNCTIONQTSO_THREADEDFUNCTION_HPP
#define THREADEDFUNCTIONQTSO_THREADEDFUNCTION_HPP

#include <QThread>

#include <functional>

//like QThread but with an internal counter of running threads
//so the amount of threads running at the same thread can be managed
//like a regular QThread it must be deleted manually
class threadedFunction_c : public QThread
{
    Q_OBJECT

    void run() override;

    std::function<void()> func_pri;
    bool startEventLoop_pri = false;
public:
    //allows to run a regular function in a thread
    //if startEventLoop is true, some signal should be connected
    //to stop the qthread or some external pointer needs to "quit" it
    //otherwise "nothing" will stop it
    threadedFunction_c(
            std::function<void()> func_par
            , const bool startEventLoop_par_con = false
            , QObject *parent_par = nullptr
    );
    //this ctor is for classes that inherit threadedFunction_c and override run themselves
    threadedFunction_c(QObject * parent_par = nullptr);

    //number of qthreads, this number is directly related to the use of the threadedFunction_c
    //the "class" increases and decreases it
    static int_fast32_t qThreadCount_f();

    //for "thread objects" created using this class how many can run in parallel, default is 1,
    //doesn't take into account the mainthread, so in TOTAL it's main thread +1
    static int_fast32_t maxConcurrentQThreads_f();
    //default is 1, doesn't take into account the mainthread, so in TOTAL it's main thread +1
    //minumum is 1, max is int32 maximum value
    //recomendation: if the code running on the threads CAN top the cpu usage don't go with
    // a value larger than the processor core count
    static void setMaxConcurrentQThreads_f(const int_fast32_t maxQThreads_par_con = 1);

    //this only hides QThread::start(), but using QThread::start instead
    //will ignore the maxConcurrentQThreads which defeats this class purpose
    void start();
};

#endif // THREADEDFUNCTIONQTSO_THREADEDFUNCTION_HPP
