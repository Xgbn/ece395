#ifndef PLOT_H
#define PLOT_H
#include "gnuplot-iostream.h"
#include <mutex>

void plotDot(double x, double y, double z);

class DotPlot {
public:
    DotPlot();
    DotPlot(double x, double y, double z);
    void updateDot(double x, double y, double z);
    void run();
private:
    Gnuplot gp;
    double x;
    double y;
    double z;
    std::mutex pos_lock;

};

#endif