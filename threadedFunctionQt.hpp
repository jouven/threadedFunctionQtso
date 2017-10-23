#ifndef THREADEDFUNCTIONQTSO_THREADEDFUNCTION_HPP
#define THREADEDFUNCTIONQTSO_THREADEDFUNCTION_HPP

#include <QThread>

#include <functional>

//number of qthreads, this number is directly related to the use of the threadedFunction_c
//the class increases and decreases it
extern uint_fast32_t QThreadCount_f();

//like a regular QThread it must be deleted manually
class threadedFunction_c : public QThread
{
    Q_OBJECT

    void run() override;
    std::function<void()> func_pri;
public:
    //this allows to run a regular function in a thread
    threadedFunction_c(std::function<void()> func_par_con, QObject *parent_par = nullptr);
    //this allows to count the qthreads spawned, the above ctor also counts too
    //this helps controlling how many qthreads are running at all times (performance)
    //and to wait before quitting if any qthreads are still running
    threadedFunction_c(QObject * parent_par = nullptr);
    ~threadedFunction_c();
};

#endif // THREADEDFUNCTIONQTSO_THREADEDFUNCTION_HPP
