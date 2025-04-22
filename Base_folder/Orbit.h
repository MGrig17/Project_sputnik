/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include "PointsCloud.h"
#include "sgp4/Observer.h"
#include "sgp4/CoordTopocentric.h"

/**
  * This class stores trajectory of a satellite in space and over sky.
  */
class Orbit : public PointsCloud {
public:
    Orbit(const libsgp4::SGP4& sat_) : PointsCloud(), sat(sat_) {
        //timeFirst = libsgp4::DateTime(0);
        //timeStep = 0;
        //polar.clear();
        polar = nullptr;
        polar_t = nullptr;
        polar_length = 0;
    }
    Orbit(const int& N_, const libsgp4::SGP4& sat_) : PointsCloud(N_), sat(sat_) {
        //timeFirst = libsgp4::DateTime(0);
        //timeStep = 0;
        //polar.clear();
        sat = sat_;
        polar = nullptr;
        polar_t = nullptr;
        polar_length = 0;
    }
    Orbit(const Orbit& o): sat(o.sat) {
        cout << "Clone constructor is called" << endl;
        N = o.get_length();
        polar_length = o.get_polar_length();
        points = new GraphPoint[N];
        polar = new GraphPoint[polar_length];
        polar_t = new libsgp4::DateTime[polar_length];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = o.get_points()[i].point;
            points[i].mask = o.get_points()[i].mask;
            r3[i].num1 = o.get_r3()[i].num1;
            r3[i].num2 = o.get_r3()[i].num2;
            r3[i].num3 = o.get_r3()[i].num3;
        }
        for (int i = 0; i < polar_length; i++) {
            polar[i].point = o.get_polar()[i].point;
            polar[i].mask = o.get_polar()[i].mask;
        }
        //polar = o.polar;
        //timeStep = o.timeStep;
        //timeFirst = o.timeFirst;
        cout << "Cloning has been completed" << endl;
    }

    Orbit& operator= (const Orbit& o) {
        cout << "Custom assignment operator is called" << endl;
        if(points != nullptr)
            delete[] points;
        if(r3 != nullptr)
            delete[] r3;
        if(polar != nullptr) {
            delete[] polar;
            delete[] polar_t;
        }
        N = o.get_length();
        polar_length = o.get_polar_length();
        points = new GraphPoint[N];
        polar = new GraphPoint[polar_length];
        polar_t = new libsgp4::DateTime[polar_length];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = o.get_points()[i].point;
            points[i].mask = o.get_points()[i].mask;
            r3[i].num1 = o.get_r3()[i].num1;
            r3[i].num2 = o.get_r3()[i].num2;
            r3[i].num3 = o.get_r3()[i].num3;
        }
        for (int i = 0; i < polar_length; i++) {
            polar[i].point = o.get_polar()[i].point;
            polar[i].mask = o.get_polar()[i].mask;
        }
        //polar = o.polar;
        //timeStep = o.timeStep;
        //timeFirst = o.timeFirst;
        cout << "Custom assignment has been completed" << endl;
    }

    /**
     * @brief Fills 'polar' with up-to-date physical points related to the orbit's trajectory over the celestial sphere
     * @param Point on the Earth to be used as a middle point for the celestial sphere
     * @param The beginning of the time interval which is to be plotted
     * @param The conclusion of the time interval which is to be plotted
     * @param X-coordinate of the pixel which is to be used as the center of the plot
     * @param Y-coordinate of the said pixel
     */
    void skyProject(libsgp4::Observer& obs, const libsgp4::DateTime& timeStart, const libsgp4::DateTime& timeEnd, const int& x_c, const int& y_c) {
        const libsgp4::TimeSpan ts = timeEnd - timeStart;
        cout << "Time, it needs time: " << ts.TotalMinutes() << " " << timeStep << endl;
        polar_length = (int)ts.TotalMinutes() / timeStep;
        cout << "polar_length = " << polar_length << endl;
        if(polar != nullptr) {
            delete[] polar;
            delete[] polar_t;
        }
        polar = new GraphPoint[polar_length];
        polar_t = new libsgp4::DateTime[polar_length];
        for (int  i = 0; i < polar_length; i++) {
            libsgp4::DateTime time = timeStart + libsgp4::TimeSpan(0, 0, timeStep * 60 * i);
            libsgp4::Eci sat_at = sat.FindPosition(time);
            libsgp4::CoordTopocentric sat_angles = obs.GetLookAngle(sat_at);
            cout << sat_angles.azimuth << " " << sat_angles.elevation << endl;
            GraphPoint gp;
            gp.point.x = x_c + std::sin(sat_angles.azimuth) * (pi / 2 - sat_angles.elevation) * R_earth * 0.04 / (pi / 2);
            gp.point.y = y_c - std::cos(sat_angles.azimuth) * (pi / 2 - sat_angles.elevation) * R_earth * 0.04 / (pi / 2);
            gp.mask = sat_angles.elevation > 0;
            polar[i] = gp;
            polar_t[i] = time;
            cout << "Point data: " << polar[i].point.x << ", " << polar[i].point.y << ", " << polar[i].mask << endl;
        }
        cout << "polar_length = " << polar_length << endl;
    }
    /**
      * @brief Draws the trajectory of the satellite in space and over the celestial sphere. Relies on data stored in 'points' and 'polar' fields.
      * @param HDC to draw in
      * @param red component of polyline color [0..255]
      * @param blue component of polyline color [0..255]
      * @param green component of polyline color [0..255]
      * @retval return code: 0 for normal workflow, -1 for empty 'points'
      */
    int draw(const HDC& hdc, const int& red, const int& green, const int& blue) {
        //cout << "Entered Orbit::draw, N = " << N << ", polar_length = " << polar_length << endl;
        if (N != 0 && polar_length != 0) {
            //cout << "First check passed" << endl;
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(red, green, blue));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            MoveToEx(hdc, (int)points[0].point.x, (int)points[0].point.y, NULL);
            for (size_t i = 1; i < N; i++) {
                if (points[i].mask) {
                    if(points[i - 1].mask && points[i].point.x < x_border && points[i - 1].point.x < x_border)
                        LineTo(hdc, (int)points[i].point.x, (int)points[i].point.y);
                    else
                        MoveToEx(hdc, (int)points[i].point.x, (int)points[i].point.y, NULL);
                }
            }
            if (points[0].mask) {
                if(points[N - 1].mask && points[0].point.x < x_border && points[N - 1].point.x < x_border)
                    LineTo(hdc, (int)points[0].point.x, (int)points[0].point.y);
                else
                    MoveToEx(hdc, (int)points[0].point.x, (int)points[0].point.y, NULL);
            }
            //cout << "3D-orbit drawn" << endl;
            //cout << polar[0].mask << endl;
            //cout << polar[0].point.x << endl;
            //cout << polar[0].point.y << endl;
            /*SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            hPen = CreatePen(PS_SOLID, 2, RGB(255, green, blue));
            hOldPen = (HPEN)SelectObject(hdc, hPen);*/
            MoveToEx(hdc, (int)polar[0].point.x, (int)polar[0].point.y, NULL);
            //cout << "Began with polar plot" << endl;
            for (size_t i = 1; i < polar_length; i++) {
                //cout << "Iteration started" << endl;
                if (polar[i].mask) {
                    if(polar[i - 1].mask)
                        LineTo(hdc, (int)polar[i].point.x, (int)polar[i].point.y);
                    else
                        MoveToEx(hdc, (int)polar[i].point.x, (int)polar[i].point.y, NULL);
                }
                //cout << "Iteration finished" << endl;
            }
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            //cout << "Exiting method" << endl;
            return 0;
        }
        else {
            cerr << "RUNTIME EXCEPTION: attempted to draw a null graphical point array" << endl;
            return -1;
        }
    }

    void set_timeStep(float timeStep_) { timeStep = timeStep_; }
    float get_timeStep() const { return timeStep; }
    int get_polar_length() const { return polar_length; }
    GraphPoint* get_polar() const { return polar; }
    //void set_timeFirst(libsgp4::DateTime timeFirst_) { timeFirst = timeFirst_; }
    //libsgp4::DateTime get_timeFirst() { return timeFirst; }
    ~Orbit() {
        cout << "Orbit destructor is called" << endl;
        delete[] polar;
        delete[] polar_t;
    }

private:
    GraphPoint* polar;                  /// Graphical points describing the satellite's trajectory over celestial sphere. Point's mask is 'true' if the satellite is traveling in the upper hemisphere.
    libsgp4::DateTime* polar_t;         /// Datetimes of the satellite pass corresponding to graphical points in 'polar'.
    float timeStep;                     /// Sampling time interval of the satellite motion.
    libsgp4::SGP4 sat;                  /// Object of a class from 'sgp4' library which allows access to satellite's location at any given point in time through series of mathematical transformations.
    int polar_length;                   /// 'polar' array length.
};
