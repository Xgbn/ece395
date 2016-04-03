
#include "plot.h"
#include <unistd.h>


DotPlot::DotPlot() {
    gp << "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 pi -1 ps 1.5\n";
    gp << "set yrange [-100:100]\n";
    gp << "set xrange [-100:100]\n";
    gp << "set zrange [-100:100]\n";
    return;
}

DotPlot::DotPlot(double x, double y, double z) {
    gp << "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 pi -1 ps 1.5\n";
    gp << "set yrange [-100:100]\n";
    gp << "set xrange [-100:100]\n";
    gp << "set zrange [-100:100]\n";
    pos_lock.lock();
    this->x = x;
    this->y = y;
    this->z = z;
    pos_lock.unlock();
    return;
}

void DotPlot::updateDot(double x, double y, double z) {
    pos_lock.lock();
    this->x = x;
    this->y = y;
    this->z = z;
    pos_lock.unlock();
    return;
}
void DotPlot::run() {
    while(1) {
        pos_lock.lock();
        gp << "splot '-' w linespoints ls 1\n";
        gp << x << ' '<< y << ' ' << z << std::endl;
        gp << 'e' << std::endl;
        pos_lock.unlock();
        usleep(10000);
    }
    return;
}



