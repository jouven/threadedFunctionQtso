#include "threadedFunctionQt.hpp"

#ifdef DEBUGJOUVEN
#include "essentialQtso/essentialQt.hpp"
#endif

#include <QObject>

#include <deque>
#include <atomic>

class threadManager_c : public QObject
{
    Q_OBJECT

    std::deque<threadedFunction_c*> threadStartFunctionQueue_pri;

    //doesn't take into account the mainthread, so in total it would be the main thread +1
    std::atomic_int_fast32_t maxQThreads_pri = 1;
    //if it's greater than 0 other QThreads are still running
    std::atomic_int_fast32_t threadCounterQt_pri = 0;

    void startThread_f(threadedFunction_c* thread_ptr_par);
public:

    int_fast32_t maxQThreads_f() const;
    void setMaxQThreads_f(const int_fast32_t& maxQThreads_par_con);
    int_fast32_t threadCounterQt_f() const;

    void threadTryingToStart_f(threadedFunction_c* thread_ptr_par);
//public Q_SLOTS:

private Q_SLOTS:
    void threadFinished_f();
};

int_fast32_t threadManager_c::maxQThreads_f() const
{
    return maxQThreads_pri;
}

void threadManager_c::setMaxQThreads_f(const int_fast32_t& maxQThreads_par_con)
{
    if (maxQThreads_par_con > 0)
    {
        maxQThreads_pri = maxQThreads_par_con;
    }
}
int_fast32_t threadManager_c::threadCounterQt_f() const
{
    return threadCounterQt_pri;
}

//https://en.wikipedia.org/wiki/Function_pointer
//void threadManager_c::threadTryingToStart_f(threadedFunction_c* threadFuncion_ptr_par, void(threadedFunction_c::*trueStart_fptr)())
void threadManager_c::threadTryingToStart_f(threadedFunction_c* thread_ptr_par)
{
//#ifdef DEBUGJOUVEN
//    QOUT_TS("(threadManager_c::threadTryingToStart_f(std::function<void()> trueStartFunc_par) threadCounterQt_pri "  << threadCounterQt_pri << endl);
//    QOUT_TS("(threadManager_c::threadTryingToStart_f(std::function<void()> trueStartFunc_par) maxQThreads_pri "  << maxQThreads_pri << endl);
//#endif
    if (threadCounterQt_pri < maxQThreads_pri)
    {
        startThread_f(thread_ptr_par);
    }
    else
    {
        threadStartFunctionQueue_pri.emplace_back(thread_ptr_par);
    }
}

void threadManager_c::threadFinished_f()
{
    threadCounterQt_pri = threadCounterQt_pri - 1;
    if (threadStartFunctionQueue_pri.empty())
    {
    }
    else
    {
//#ifdef DEBUGJOUVEN
//    QOUT_TS("(threadManager_c::threadFinished_f() threadStartFunctionQueue_pri.front() " << threadStartFunctionQueue_pri.front() << endl);
//#endif
        startThread_f(threadStartFunctionQueue_pri.front());
        threadStartFunctionQueue_pri.pop_front();
    }
}

void threadManager_c::startThread_f(threadedFunction_c* thread_ptr_par)
{
//#ifdef DEBUGJOUVEN
//    QOUT_TS("(threadManager_c::increaseRunningThreadCount_f() thread_ptr_par "  << thread_ptr_par << endl);
//#endif
    QObject::connect(thread_ptr_par, &QThread::finished, this, &threadManager_c::threadFinished_f);
    threadCounterQt_pri = threadCounterQt_pri + 1;
    thread_ptr_par->QThread::start();
}

threadManager_c& threadManager_f()
{
    static threadManager_c threadManager_sta;
    return threadManager_sta;
}

threadedFunction_c::threadedFunction_c(
        std::function<void()> func_par
        , const bool startEventLoop_par_con
        , QObject * parent_par)
    : QThread(parent_par)
    , func_pri(func_par)
    , startEventLoop_pri(startEventLoop_par_con)
{}

threadedFunction_c::threadedFunction_c(QObject *parent_par) : threadedFunction_c({}, parent_par)
{}

//threadedFunction_c::~threadedFunction_c()
//{
//    threadManager_f().threadCounterQt_pub--;
//#ifdef DEBUGJOUVEN
//        QOUT_TS("(threadedFunction_c::~threadedFunction_c) threadCounterQt_ato "  << threadManager_f().threadCounterQt_pub << endl);
//#endif
//}

int_fast32_t threadedFunction_c::qThreadCount_f()
{
    return threadManager_f().threadCounterQt_f();
}

int_fast32_t threadedFunction_c::maxConcurrentQThreads_f()
{
    return threadManager_f().maxQThreads_f();
}

void threadedFunction_c::setMaxConcurrentQThreads_f(const int_fast32_t maxQThreads_par_con)
{
    threadManager_f().setMaxQThreads_f(maxQThreads_par_con);
}

void threadedFunction_c::start()
{
//#ifdef DEBUGJOUVEN
//    QOUT_TS("(threadedFunction_c::start) "  << endl);
//#endif
    threadManager_f().threadTryingToStart_f(this);
}

void threadedFunction_c::run()
{
    if (func_pri)
    {
        func_pri();
    }
    if (startEventLoop_pri)
    {
        exec();
    }
}

//explanation time: this is because a qt inherited class with Q_OBJECT macro is declared in this cpp
//otherwise it gives a "undefined reference to vtable cpp" error on the executable project linking this lib
#include "threadedFunctionQt.moc"

