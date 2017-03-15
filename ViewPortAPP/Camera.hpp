/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.hpp
 * Author: lucampelli
 *
 * Created on 9 de Março de 2017, 19:00
 */
/*
#include "Ponto.hpp"

#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera {
private:
    Ponto* pos;
    float zoom = 1.0f;

public:

    Camera() {
        pos = new Ponto();
    }

    static Camera* getInstance() {
        static Camera* viewport;
        if (!viewport) {
            viewport = new Camera();
        }
        return viewport;
    }

    void moveCamera(float xAmount, float yAmount) {
        pos->move_to(pos->getX() + xAmount, pos->getY() + yAmount);
    }

    Ponto* getPos() {
        return pos;
    }

    float DistX(Ponto* p1, Ponto* p2) {
        return p1->getX() - p2->getX(); //Gambi
    }

    float DistY(Ponto* p1, Ponto* p2) {
        return p1->getY() - p2->getY();//Gambi
    }
    
    Ponto* drawTransform(Ponto* p){
        Ponto *ps = new Ponto(pos->getX() + 300, pos->getY() + 300);
        float xD = DistX(p,ps);
        float yD = DistY(p,ps);                                             //do it again with slides
        return new Ponto(ps->getX() + zoom * xD, ps->getY() + zoom * yD);   // try clipping? considered
    }
    
    Ponto* clickTransform(Ponto* p){
        Ponto *ps = new Ponto(pos->getX() + 300, pos->getY() + 300);
        float xD = DistX(p,ps);
        float yD = DistY(p,ps);                                             //do it again with slides
        return new Ponto(ps->getX() + xD / zoom, ps->getY() + yD / zoom);   //have both world size and viewport size
    }
    
    
    float getZoom(){
        return zoom;
    }

    void Zoom(bool in) {
        if (in) {
            zoom += 0.1f;
            if (zoom > 3) {
                zoom = 3;
            }
        } else {
            zoom -= 0.1f;
            if (zoom < 0.1f) {
                zoom = 0.1f;
            }
        }

    }
    
    void Zoom(float value){
        zoom = value/100;
    }
};
*/
 

#include "Ponto.hpp"

#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera {
private:
    Ponto* pos;
    float zoom = 1.0f;
    float viewportmin[2] = {0, 0};
    float viewportmax[2] = {600, 600};
    float windowmin[2] = {0, 0};
    float windowmax[2] = {600, 600};
    float m11 = (viewportmax[0] - viewportmin[0]) / (windowmax[0] - windowmin[0]);
    float m22 = (viewportmax[1] - viewportmin[1]) / (windowmax[1] - windowmin[1]);
    float m13 = -windowmin[0] * (m11) + viewportmin[0];
    float m23 = -windowmin[1] * (m22) + viewportmin[1];

public:

    Camera() {
        pos = new Ponto();
    }

    static Camera* getInstance() {
        static Camera* viewport;
        if (!viewport) {
            viewport = new Camera();
        }
        return viewport;
    }

    void moveCamera(float xAmount, float yAmount) {
        pos->move_to(pos->getX() + xAmount, pos->getY() + yAmount);
    }

    Ponto* getPos() {
        return pos;
    }

    void calculate_matrix() {
        m11 = (viewportmax[0] - viewportmin[0]) / (windowmax[0] - windowmin[0]);
        m22 = (viewportmax[1] - viewportmin[1]) / (windowmax[1] - windowmin[1]);
        m13 = -windowmin[0] * (m11) + viewportmin[0];
        m23 = -windowmin[1] * (m22) + viewportmin[1];
    }

    void calculate_matrix(float viewportmin[2], float viewportmax[2]) {
        m11 = (viewportmax[0] - viewportmin[0]) / (windowmax[0] - windowmin[0]);
        m22 = (viewportmax[1] - viewportmin[1]) / (windowmax[1] - windowmin[1]);
        m13 = -windowmin[0] * (m11) + viewportmin[0];
        m23 = -windowmin[1] * (m22) + viewportmin[1];
    }

    Ponto* drawTransform(Ponto* p) {
        // try clipping? considered
        return new Ponto(p->getX() * m11 + m13 + pos->getX(), p->getY() * m22 + m23 + pos->getY());
    }
    
    Ponto* clickTransform(Ponto* p) {
        // try clipping? considered
        return new Ponto((p->getX()- m13 - pos->getX()) / m11, (p->getY() - m23 - pos->getY()) / m22 );
    }
    

    float getZoom() {
        return zoom;
    }

    void Zoom(bool in) {
        if (in) {
            zoom += 0.1f;
            if (zoom > 3) {
                zoom = 3;
                return;
            }
        } else {
            zoom -= 0.1f;
            if (zoom < 0.1f) {
                zoom = 0.1f;
                return;
            }
        }
        float medx = (viewportmax[0] + viewportmin[0]) / 2;
        float medy = (viewportmax[1] + viewportmin[1]) / 2;
        float zx = medx * zoom;
        float zy = medy * zoom;
        float nmin[2] = {medx - zx, medy - zy};
        float nmax[2] = {medx + zx, medy + zy};

        calculate_matrix(nmin, nmax);
    }

    void Zoom(float value) {
        zoom = value / 100;
    }
};

//#endif /* CAMERA_HPP */

#endif /* CAMERA_HPP */

