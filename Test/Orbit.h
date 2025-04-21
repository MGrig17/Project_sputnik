/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include "PointsCloud.h"

/**
  * This class stores trajectory of a satellite.
  */
class Orbit : public PointsCloud {
public:
    Orbit() : PointsCloud() {
        //timeFirst = libsgp4::DateTime(0);
        //timeStep = 0;
        //polar.clear();
    }
    Orbit(int N_) : PointsCloud(N_) {
        //timeFirst = libsgp4::DateTime(0);
        //timeStep = 0;
        //polar.clear();
    }
    Orbit(const Orbit& o) {
        cout << "Clone constructor is called" << endl;
        N = o.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = o.get_points()[i].point;
            points[i].mask = o.get_points()[i].mask;
            r3[i].num1 = o.get_r3()[i].num1;
            r3[i].num2 = o.get_r3()[i].num2;
            r3[i].num3 = o.get_r3()[i].num3;
        }
        //polar = o.polar;
        //timeStep = o.timeStep;
        //timeFirst = o.timeFirst;
        cout << "Cloning has been completed" << endl;
    }

    Orbit& operator= (const Orbit& o) {
        cout << "Custom assignment operator is called" << endl;
        delete[] points;
        delete[] r3;
        N = o.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = o.get_points()[i].point;
            points[i].mask = o.get_points()[i].mask;
            r3[i].num1 = o.get_r3()[i].num1;
            r3[i].num2 = o.get_r3()[i].num2;
            r3[i].num3 = o.get_r3()[i].num3;
        }
        //polar = o.polar;
        //timeStep = o.timeStep;
        //timeFirst = o.timeFirst;
        cout << "Custom assignment has been completed" << endl;
    }

    /*void SkyProject(const float* obs, const libsgp4::DateTime& timeStart, const libsgp4::DateTime& timeEnd) {
        libsgp4::DateTime time = timeStart;
        while (time < timeEnd) {
            libsgp4::TimeSpan dt_raw = time - timeFirst;
            float dt_imd = dt_raw.TotalSeconds(); //dt_intermediate
            float dt_rfd = (long)dt_imd % (int)(timeStep * N); //dt_refined
            int i = dt_rfd / timeStep; //r3 array index of element which corresponds to the satellite position at 'time'
        }
    }*/
    //void set_timeStep(float timeStep_) { timeStep = timeStep_; }
    //float get_timeStep() { return timeStep; }
    //void set_timeFirst(libsgp4::DateTime timeFirst_) { timeFirst = timeFirst_; }
    //libsgp4::DateTime get_timeFirst() { return timeFirst; }
    ~Orbit() {
        cout << "Orbit destructor is called" << endl;
    }

private:
    //vector<GraphPoint> polar;
    //vector<libsgp4::DateTime> polar_t; //datetimes corresponding to points in polar.
    //float timeStep; //time interval in seconds between two consequent points
    //libsgp4::DateTime timeFirst; //datetime corresponding to the first point in r3 array
};
