/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include "PointsCloud.h"
#include "Orbit.h"

/**
  * This class stores all data related to the satellite tracking and visualisation
  */
class Simulation {
public:
    Simulation() {
        k_zoom = 0.02;
        earth = new GraphPoint[100];
        surface = new PointsCloud[7];
        float c[3] = {0, 0, 0};
        float v[3] = {0, 0, R_earth};
        for(int i = 0; i < 4; i++) {
            float u[3] = {R_earth * std::sin(pi / 4 * i), R_earth * std::cos(pi / 4 * i), 0};
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

        updateEarth();
    }
    Simulation(const float& k_zoom_) {
        k_zoom = k_zoom_;
        earth = new GraphPoint[100];
        surface = new PointsCloud[7];
        float c[3] = {0, 0, 0};
        float v[3] = {0, 0, R_earth};
        for(int i = 0; i < 4; i++) {
            float u[3] = {R_earth * std::sin(pi / 4 * i), R_earth * std::cos(pi / 4 * i), 0};
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
        if(time_step == -1 || time_step == NULL) {
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
        Orbit new_orbit = Orbit();
        new_orbit.set_r3(r, r_len);
        //new_orbit.set_timeStep(time_step * 60);
        //new_orbit.set_timeFirst(tle.Epoch());
        Triplet<int> new_orbit_color = { clRed, clGreen, clBlue };
        satellites.push_back(new_orbit);
        sat_color.push_back(new_orbit_color);
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
        int res = 0;
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, earth[0].point.x, earth[0].point.y, NULL);
        for (size_t i = 1; i < 100; i++) {
            LineTo(hdc, earth[i].point.x, earth[i].point.y);
        }
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        LineTo(hdc, earth[0].point.x, earth[0].point.y);
        for (int  i = 0; i < 7; i++) {
            res += surface[i].draw(hdc, 0, 0, 0);
        }
        for (int i = 0; i < satellites.size(); i++) {
            res += satellites[i].draw(hdc, sat_color[i].num1, sat_color[i].num2, sat_color[i].num3);
        }
        if (res != 0) {
            cerr << "RUNTIME EXCEPTION: Method 'draw' of an object of 'Graph3D' class has malfunctioned " << -res << " times" << endl;
        }
        return res;
    }
    void updateGraph3D(const float* n) {
        for (int i = 0; i < satellites.size(); i++) {
            satellites[i].projectCloud(R_earth * k_zoom, n, k_zoom);
        }
        for (int  i = 0; i < 7; i++) {
            surface[i].projectCloud(R_earth * k_zoom, n, k_zoom);
        }
    }
    void scroll(const float& dk_zoom, const float* n) {
        if(k_zoom > -dk_zoom) {
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
            earth[i].point.x = R_earth * k_zoom * std::sin((float)i / 50 * pi) + x_c;
            earth[i].point.y = R_earth * k_zoom * std::cos((float)i / 50 * pi) + y_c;
        }
    }
    GraphPoint* earth;
    float k_zoom;
    vector<Triplet<int>> sat_color;
    vector<Orbit> satellites;
    PointsCloud* surface;
};
