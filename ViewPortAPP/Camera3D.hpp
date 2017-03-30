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
#include "Viewport.hpp"
#include "Transform.hpp"

#ifndef CAMERA2_HPP
#define CAMERA2_HPP

class Camera3D {
private:
    float width;
    float height;
    float depth;
    Ponto* ogmin;
    Ponto* ogmax;
    Ponto* windowmin;
    Ponto* windowmax;
    
    Ponto* pos;
    Viewport* viewport;
    Transform* transform;
    int rot = 0;
    float zoom = 1.0f;

public:

    Camera3D() {
        windowmin = new Ponto();
        ogmin = new Ponto();
        windowmax = new Ponto(600, 600);
        ogmax =  new Ponto(600, 600);
        
        viewport = Viewport::getInstance();
        transform = Transform::getInstance();
        pos = new Ponto();
        calculate_matrix();
    }

    static Camera3D* getInstance() {
        static Camera3D* camera;
        if (!camera) {
            camera = new Camera3D();
        }
        return camera;
    }
    
    Ponto* winCenter(){
        return new Ponto((windowmax->getX() - windowmin->getX())/2,(windowmax->getY() - windowmin->getY())/2);
    }
    
    void rotateCamera(int degrees){
        rot = degrees; 
    }

    int getRot(){
       return rot; 
    }
    
    void moveCamera(float xAmount, float yAmount) {
        pos->move_to(pos->getX() + xAmount, pos->getY() + yAmount);
        windowmin = pos;
        windowmax = new Ponto(pos->getX() + width, pos->getY() + height);
        
    }

    Ponto* getPos() {
        return pos;
    }
    
    Ponto* T(Ponto* p){
        return transform->transform(p);
    }

    Ponto* rotateTransform(int angle, Ponto* p, Ponto* c){
        return transform->rotationT2D(angle, p, c);
    }

    Ponto* drawTransform(Ponto* p) {
        calculate_matrix();
        return transform->dT(p);
    }
    
    Ponto* clickTransform(Ponto* p) {
        calculate_matrix();
        return transform->cT(p);
    }
    

    float getZoom() {
        return zoom;
    }
    
    void calculate_matrix(){
        transform->calculate_draw_and_click_matrix(viewport->min(),viewport->max(),windowmin,windowmax, zoom);
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
    
    void SCN(){
        transform->setT(transform->set_2D_move_matrix(-winCenter()->getX(), -winCenter()->getY()));
        transform->concatenate_matrix(transform->set_2D_rotation_matrix(rot));
    }
    
};

//#endif /* CAMERA_HPP */

#endif /* CAMERA_HPP */

