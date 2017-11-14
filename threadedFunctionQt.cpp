#include "threadedFunctionQt.hpp"

#ifdef DEBUGJOUVEN
#include "essentialQtso/essentialQt.hpp"
#endif

#include <atomic>

//if it's greater than 0 other QThreads are still running
std::atomic_uint_fast32_t threadCounterQt_ato(0);

uint_fast32_t qThreadCount_f()
{
    return threadCounterQt_ato;
}

//void decreaseThread_f()
//{
//    threadCounterQt_ato = threadCounterQt_ato - 1;
////#ifdef DEBUGJOUVEN
////    QOUT_TS("(threadedFunction_c::decreaseThread_f) threadCounterQt_ato "  << threadCounterQt_ato << endl);
////#endif
//}

threadedFunction_c::threadedFunction_c(std::function<void()> func_par_con, QObject * parent_par)
    : QThread(parent_par), func_pri(func_par_con)
{
    threadCounterQt_ato++;
}

threadedFunction_c::threadedFunction_c(QObject *parent_par) : threadedFunction_c({}, parent_par)
{}

threadedFunction_c::~threadedFunction_c()
{
    threadCounterQt_ato--;
}

void threadedFunction_c::run()
{
    func_pri();
}
