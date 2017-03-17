/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Elemento.hpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
 *
 * Created on 8 de Março de 2017, 14:05
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
    float nmin[2] = {0,0};
    float nmax[2] = {600,600};
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
        windowmin[0] = -pos->getX();
        windowmin[1] = -pos->getY();
        windowmax[0] = -pos->getX() + 600;
        windowmax[1] = -pos->getY() + 600;
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
        calculate_matrix(nmin,nmax);
        return new Ponto(p->getX() * m11 + m13 , p->getY() * m22 + m23 );
    }
    
    Ponto* clickTransform(Ponto* p) {
        // try clipping? considered
        calculate_matrix(nmin,nmax);
        return new Ponto((p->getX()- m13 ) / m11, (p->getY() - m23 ) / m22 );
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
        nmin[0] = medx - zx;
        nmin[1] = medy - zy;
        nmax[0] = medx + zx;
        nmax[1] = medy + zy;

        calculate_matrix(nmin, nmax);
    }

    void Zoom(float value) {
        zoom = value / 100;
    }
};

//#endif /* CAMERA_HPP */

#endif /* CAMERA_HPP */

