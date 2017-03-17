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
#include "Window.hpp"
#include "Viewport.hpp"
#include "Transform.hpp"

#ifndef CAMERA2_HPP
#define CAMERA2_HPP

class Camera3D {
private:
    Ponto* pos;
    Ponto* center;
    Window* window;
    Viewport* viewport;
    Transform* transform;
    float zoom = 1.0f;

public:

    Camera3D() {
        pos = new Ponto();
        window = Window::getInstance();
        viewport = Viewport::getInstance();
        transform = Transform::getInstance();
        calculate_matrix();
    }

    static Camera3D* getInstance() {
        static Camera3D* camera;
        if (!camera) {
            camera = new Camera3D();
        }
        return camera;
    }

    void moveCamera(float xAmount, float yAmount) {
        pos->move_to(pos->getX() + xAmount, pos->getY() + yAmount);
        window->moveWindowToPoint(new Ponto(pos->getX(),pos->getY()));
        calculate_matrix();
    }

    Ponto* getPos() {
        return pos;
    }


    Ponto* drawTransform(Ponto* p) {
        return transform->T(p);
    }
    
    Ponto* clickTransform(Ponto* p) {
        return transform->iT(p);
    }
    

    float getZoom() {
        return zoom;
    }
    
    void calculate_matrix(){
        transform->calculate_matrix(viewport->min(),viewport->max(),window->min(),window->max());
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
        viewport->scaleViewPort(zoom);
        calculate_matrix();
    }

    void Zoom(float value) {
        zoom = value / 100;
    }
};

//#endif /* CAMERA_HPP */

#endif /* CAMERA_HPP */
