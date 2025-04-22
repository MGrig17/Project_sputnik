/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include "supplementary.h"
#include <vector>
#include <string>
#include <iostream>
#include "sgp4/SGP4.h"
#include "sgp4/Tle.h"
#include "sgp4/Util.h"
#include "sgp4/Vector.h"
#include "sgp4/Globals.h"
#include "sgp4/DateTime.h"
#include "sgp4/TimeSpan.h"

#define R_earth 6400        ///Earth radius, km
#define x_border 750        ///Borderline pixel after which near-Earth space visualisation is not to be displayed

using namespace std;

/**
  * This class stores a single closed polyline.
  */
class PointsCloud {
public:
    PointsCloud() {
        N = 0;
        points = nullptr;
        r3 = nullptr;
    }
    PointsCloud(const int& N_) {
        N = N_;
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
    }
    PointsCloud(PointsCloud& pc) {
        cout << "Clone constructor is called" << endl;
        N = pc.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = pc.get_points()[i].point;
            points[i].mask = pc.get_points()[i].mask;
            r3[i].num1 = pc.get_r3()[i].num1;
            r3[i].num2 = pc.get_r3()[i].num2;
            r3[i].num3 = pc.get_r3()[i].num3;
        }
        cout << "Cloning has been completed" << endl;
    }

    PointsCloud& operator= (const PointsCloud& pc) {
        cout << "Custom assignment operator is called" << endl;
        delete[] points;
        delete[] r3;
        N = pc.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = pc.get_points()[i].point;
            points[i].mask = pc.get_points()[i].mask;
            r3[i].num1 = pc.get_r3()[i].num1;
            r3[i].num2 = pc.get_r3()[i].num2;
            r3[i].num3 = pc.get_r3()[i].num3;
        }
        cout << "Custom assignment has been completed" << endl;
    }

    /**
      * @brief Fills 'points' with up-to-date polyline projection onto the monitor plane
      * @param Radius of the image of the Earth on the monitor (in px)
      * @param Vector normal to the monitor plane oriented towards the camera position (must be of length 1)
      * @param Scale coefficient between coordinates of graphical point and local coordinates of projected physical point (px/km)
      */
    void projectCloud(const LONG& R_e, const float* n, const float& k_zoom, const int& x_c, const int& y_c) {
        //cout << boolalpha;
        const float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]); //нормировочный коэффициент для вектора базиса локальной системы координат, лежащего в плоскости xy
        float r2[2]; //элемент r3 в локальных координатах плоскости проектирования
        for (int i = 0; i < N; i++) {
            r2[0] = 0;
            r2[1] = 0;
            float t = 2 * pi * i / (N - 1); //параметр уравнения эллипса
            if (n[0] != 0 || n[1] != 0) {
                r2[0] = (n[1] * r3[i].num1 - n[0] * r3[i].num2) / norm;
                r2[1] = r3[i].num1 * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i].num2 * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i].num3 * std::sqrt(n[0] * n[0] + n[1] * n[1]);
            }
            else {
                r2[0] = r3[i].num1;
                r2[1] = r3[i].num2 * -1;
            }
            r2[0] *= k_zoom;
            r2[0] += x_c;
            r2[1] *= k_zoom;
            r2[1] += y_c;
            points[i].point.x = (LONG)r2[0];
            points[i].point.y = (LONG)r2[1];
            points[i].mask = (std::sqrt((points[i].point.x - x_c) * (points[i].point.x - x_c) + (points[i].point.y - y_c) * (points[i].point.y - y_c)) > R_e) || (r3[i].num1 * n[0] + r3[i].num2 * n[1] + r3[i].num3 * n[2] > 0);
        }
        //cout << points[0].point.x << points[0].point.y << points[0].mask;
        //cout << points[30].point.x << points[30].point.y << points[30].mask;
        //cout << noboolalpha << endl;
    }
    /**
      * @brief Draws the polyline based on the data stored in 'points'
      * @param HDC to draw in
      * @param red component of polyline color [0..255]
      * @param blue component of polyline color [0..255]
      * @param green component of polyline color [0..255]
      * @retval return code: 0 for normal workflow, -1 for empty 'points'
      */
    int draw(const HDC& hdc, const int& red, const int& green, const int& blue) {
        if (N != 0) {
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(red, green, blue));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            MoveToEx(hdc, (int)points[0].point.x, (int)points[0].point.y, NULL);
            //bool flag = true;
            for (size_t i = 1; i < N; i++) {
                if (points[i].mask) {
                    if(points[i - 1].mask && points[i].point.x < x_border && points[i - 1].point.x < x_border)
                        LineTo(hdc, (int)points[i].point.x, (int)points[i].point.y);
                    else
                        MoveToEx(hdc, (int)points[i].point.x, (int)points[i].point.y, NULL);
                }
                //flag = points[i].mask && points[i].point.x < x_border;
            }
            if (points[0].mask) {
                if(points[N - 1].mask && points[0].point.x < x_border && points[N - 1].point.x < x_border)
                    LineTo(hdc, (int)points[0].point.x, (int)points[0].point.y);
                else
                    MoveToEx(hdc, (int)points[0].point.x, (int)points[0].point.y, NULL);
            }
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            return 0;
        }
        else {
            cerr << "RUNTIME EXCEPTION: attempted to draw a null-PointsCloud" << endl;
            return -1;
        }
    }
    /**
      * @brief Assigns new array of physical points to the object
      * @param Array of physical points' coordinates
      * @param Length of 'r3_'
      */
    void set_r3(float**& r3_, const int& N_) {
        N = N_;
        if (points != nullptr)
            delete[] points;
        delete[] r3;
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        cout << N << endl;
        for (int i = 0; i < N; i++) {
            cout << i << endl;
            r3[i].num1 = r3_[i][0];
            r3[i].num2 = r3_[i][1];
            r3[i].num3 = r3_[i][2];
        }
    }
    /**
      * Look for the previous method description
      */
    void set_r3(Triplet<float>*& r3_, const int& N_) {
        N = N_;
        if (points != nullptr)
            delete[] points;
        delete[] r3;
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        cout << N << endl;
        for (int i = 0; i < N; i++) {
            cout << i << endl;
            r3[i].num1 = r3_[i].num1;
            r3[i].num2 = r3_[i].num2;
            r3[i].num3 = r3_[i].num3;
        }
    }

    int get_length() const { return N; }
    GraphPoint* get_points() const { return points; }
    Triplet<float>* get_r3() const { return r3; }

    ~PointsCloud() {
        cout << "PointsCloud destructor is called" << endl;
        delete[] points;
        delete[] r3;
    }
protected:
    GraphPoint* points;     /// Graphical points array describing polyline projection onto the monitor
    Triplet<float>* r3;     /// Physical points array describing the polyline itself
    int N;                  /// Number of points in polyline
};
