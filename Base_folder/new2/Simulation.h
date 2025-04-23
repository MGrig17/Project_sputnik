/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include "PointsCloud.h"
#include "Orbit.h"
#include "sgp4/Observer.h"
#define x_c1 400
#define y_c1 350
#define x_c2 1000
#define y_c2 350
  /**
    * This class stores all data related to the satellite tracking and visualisation
    */
class Simulation {
public:
    Simulation() : observatory(55.92933, 37.52252, 0.197) { // Those are MIPT dormitory #3 geodetic coordinates: latitude, longitude and altitude respectively
        k_zoom = 0.02;
        radar[0] = new GraphPoint[100];
        radar[1] = new GraphPoint[100];
        radar[2] = new GraphPoint[100];
        radar[3] = new GraphPoint[100];
        radar[4] = new GraphPoint[100];
        radar[5] = new GraphPoint[100];
        radar[6] = new GraphPoint[100];
        radar[7] = new GraphPoint[100];
        earth = new GraphPoint[100];
        surface = new PointsCloud[7];
        float c[3] = { 0, 0, 0 };
        float v[3] = { 0, 0, R_earth };
        for (int i = 0; i < 4; i++) {
            float u[3] = { R_earth * std::sin(pi / 4 * i), R_earth * std::cos(pi / 4 * i), 0 };
            Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
            surface[i].set_r3(new_ellipse, 30);
        }
        float u[3];
        v[0] = 0;
        v[1] = R_earth;
        v[2] = 0;
        u[0] = R_earth;
        u[1] = 0;
        u[2] = 0;
        Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
        surface[4].set_r3(new_ellipse, 30);


        c[2] = R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[5].set_r3(new_ellipse, 30);

        c[2] = -R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[6].set_r3(new_ellipse, 30);

        The_Radar();
        updateEarth();

    }
    Simulation(const float& k_zoom_) : observatory(55.92933, 37.52252, 0.197) { // Those are MIPT dormitory #3 geodetic coordinates: latitude, longitude and altitude respectively
        k_zoom = k_zoom_;
        radar[0] = new GraphPoint[100];
        radar[1] = new GraphPoint[100];
        radar[2] = new GraphPoint[100];
        radar[3] = new GraphPoint[100];
        radar[4] = new GraphPoint[100];
        radar[5] = new GraphPoint[100];
        radar[6] = new GraphPoint[100];
        radar[7] = new GraphPoint[100];
        earth = new GraphPoint[100];
        surface = new PointsCloud[7];
        float c[3] = { 0, 0, 0 };
        float v[3] = { 0, 0, R_earth };
        for (int i = 0; i < 4; i++) {
            float u[3] = { R_earth * std::sin(pi / 4 * i), R_earth * std::cos(pi / 4 * i), 0 };
            Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
            surface[i].set_r3(new_ellipse, 30);
        }
        float u[3];
        v[0] = 0;
        v[1] = R_earth;
        v[2] = 0;
        u[0] = R_earth;
        u[1] = 0;
        u[2] = 0;
        Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
        surface[4].set_r3(new_ellipse, 30);


        c[2] = R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[5].set_r3(new_ellipse, 30);

        c[2] = -R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[6].set_r3(new_ellipse, 30);

        The_Radar();
        updateEarth();
    }

    void add_orbit(const string& name, const string& tle_line1, const string& tle_line2, const float& timeStep, const int& clRed, const int& clGreen, const int& clBlue) {
        const libsgp4::Tle tle(name, tle_line1, tle_line2);
        const libsgp4::SGP4 sgp4(tle);
        //cout << "TLE epoch: " << tle.Epoch() << endl;
        const float minutes_in_day = 60 * 24;
        const float end_time = minutes_in_day / tle.MeanMotion();
        float time_step = timeStep;
        //cout << time_step << "!" << endl;
        if (time_step == -1 || time_step == NULL) {
            //cout << "Yeet!" << endl;
            time_step = end_time / 100;
        }
        const int r_len = end_time / time_step;
        float** r = new float* [r_len];
        for (int i = 0; i < r_len; i++) {
            libsgp4::Eci eci = sgp4.FindPosition(i * time_step);
            //std::cout << eci.Position() << std::endl;
            //cout << i << endl;
            r[i] = new float[3];
            r[i][0] = eci.Position().x;
            r[i][1] = eci.Position().y;
            r[i][2] = eci.Position().z;
        }
        Orbit new_orbit = Orbit(sgp4);
        new_orbit.set_r3(r, r_len);
        new_orbit.set_timeStep(time_step);
        new_orbit.skyProject(observatory, tle.Epoch(), tle.Epoch() + libsgp4::TimeSpan(10, 0, 0), x_c2, y_c2);
        //new_orbit.set_timeFirst(tle.Epoch());
        Triplet<int> new_orbit_color = { clRed, clGreen, clBlue };
        satellites.push_back(new_orbit);
        sat_color.push_back(new_orbit_color);
        //cout << "polar_length = " << satellites[i].get_polar_length() << endl;
        for (int i = 0; i < r_len; i++) {
            delete[] r[i];
        }
        delete[] r;
    }
    void remove_orbit() {
        satellites.pop_back();
        sat_color.pop_back();
    }
    int draw(const HDC& hdc) {
        cout << "Entered sim.draw" << endl;
        int res = 0;
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(139, 139, 139));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        for (size_t j = 1; j<8; j++) {
            MoveToEx(hdc, radar[j][0].point.x, radar[j][0].point.y, NULL);
            for (size_t i = 1; i < 100; i++) {
                LineTo(hdc, radar[j][i].point.x, radar[j][i].point.y);
            }
            LineTo(hdc, radar[j][0].point.x, radar[j][0].point.y);
        }
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        hPen = CreatePen(PS_SOLID, 7, RGB(0, 0, 0));
        hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, radar[0][0].point.x, radar[0][0].point.y, NULL);
        for (size_t i = 1; i < 100; i++) {
            LineTo(hdc, radar[0][i].point.x, radar[0][i].point.y);
        }
        LineTo(hdc, radar[0][0].point.x, radar[0][0].point.y);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        hPen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
        hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, earth[0].point.x, earth[0].point.y, NULL);
        for (size_t i = 1; i < 100; i++) {
            if (earth[i].point.x < x_border && earth[i - 1].point.x < x_border)
                LineTo(hdc, earth[i].point.x, earth[i].point.y);
            else
                MoveToEx(hdc, earth[i].point.x, earth[i].point.y, NULL);
        }
        
        if (earth[0].point.x < x_border && earth[99].point.x < x_border)
            LineTo(hdc, earth[0].point.x, earth[0].point.y);
        cout << "Earth redrawn" << endl;
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        for (int i = 0; i < 7; i++) {
            res += surface[i].draw(hdc, 0, 0, 0);
        }
        cout << "Earth surface redrawn" << endl;
        for (int i = 0; i < satellites.size(); i++) {
            res += satellites[i].draw(hdc, sat_color[i].num1, sat_color[i].num2, sat_color[i].num3);
        }
        /*...*/
        if (res != 0) {
            cerr << "RUNTIME EXCEPTION: Method 'draw' of an object of 'Graph3D' class has malfunctioned " << -res << " times" << endl;
        }
        cout << "Exited sim.draw" << endl;
        return res;
    }
    void updateGraph3D(const float* n) {
        for (int i = 0; i < satellites.size(); i++) {
            satellites[i].projectCloud(R_earth * k_zoom, n, k_zoom, x_c1, y_c1);
        }
        for (int i = 0; i < 7; i++) {
            surface[i].projectCloud(R_earth * k_zoom, n, k_zoom, x_c1, y_c1);
        }
    }
    void scroll(const float& dk_zoom, const float* n) {
        if (k_zoom > -dk_zoom) {
            k_zoom += dk_zoom;
            updateEarth();
            updateGraph3D(n);
        }
    }
private:
    /**
      * @brief This method is supposed to be called once k_zoom is changed to bring Earth to scale
      */
    void updateEarth() {
        for (int i = 0; i < 100; i++) {
            earth[i].point.x = R_earth * k_zoom * std::sin((float)i / 50 * pi) + x_c1;
            earth[i].point.y = R_earth * k_zoom * std::cos((float)i / 50 * pi) + y_c1;
        }
    }
    void The_Radar() {
        for (int i = 0; i < 100; i++) {
            radar[0][i].point.x = R_earth * 0.04 * std::sin((float)i / 50 * pi) +  x_c2;
            radar[0][i].point.y = R_earth * 0.04 * std::cos((float)i / 50 * pi) +  y_c2;

            radar[1][i].point.x = R_earth * 0.032 * std::sin((float)i / 50 * pi) + x_c2;
            radar[1][i].point.y = R_earth * 0.032 * std::cos((float)i / 50 * pi) + y_c2;

            radar[2][i].point.x = R_earth * 0.024 * std::sin((float)i / 50 * pi) + x_c2;
            radar[2][i].point.y = R_earth * 0.024 * std::cos((float)i / 50 * pi) + y_c2;

            radar[3][i].point.x = R_earth * 0.016 * std::sin((float)i / 50 * pi) + x_c2;
            radar[3][i].point.y = R_earth * 0.016 * std::cos((float)i / 50 * pi) + y_c2;

            radar[4][i].point.x = x_c2;
            radar[4][i].point.y = R_earth * 0.04 * std::cos((float)i / 50 * pi) + y_c2;

            radar[5][i].point.x = R_earth * 0.04 * std::sin((float)i / 50 * pi) + x_c2;
            radar[5][i].point.y = y_c2;

            radar[6][i].point.x = R_earth * 0.04 * sqrt(2) / 2 * std::sin((float)i / 50 * pi) + x_c2;
            radar[6][i].point.y = R_earth * 0.04 * sqrt(2) / 2 * std::sin((float)i / 50 * pi) + y_c2;

            radar[7][i].point.x = -R_earth * 0.04 * sqrt(2) / 2 * std::sin((float)i / 50 * pi) + x_c2;
            radar[7][i].point.y = R_earth * 0.04 * sqrt(2) / 2 * std::sin((float)i / 50 * pi) + y_c2;
        }
    }
    GraphPoint* radar[8]; //!!!!!!!RAAAAAADAAAAAAAAAAAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    GraphPoint* earth;
    float k_zoom;
    vector<Triplet<int>> sat_color;
    vector<Orbit> satellites;
    PointsCloud* surface;
    libsgp4::Observer observatory;
};
