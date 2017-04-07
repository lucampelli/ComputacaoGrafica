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
#include "ClipPlane.hpp"
#include "Shape.hpp"

#ifndef CAMERA2_HPP
#define CAMERA2_HPP

class Camera3D {
private:
    float width;
    float height;
    float depth;
    Ponto* scnmin;
    Ponto* scnmax;
    Ponto* windowmin;
    Ponto* windowmax;

    ListaEnc<Shape*> * shapes = new ListaEnc<Shape*>();
    ListaEnc<Shape*> * normShapes = new ListaEnc<Shape*>();

    Ponto* pos;
    Viewport* viewport;
    Transform* transform;
    ClipPlane* clip;
    int rot = 0;
    float zoom = 1.0f;

public:

    Camera3D() {
        height = 2;
        width = 2;

        windowmin = new Ponto(-1,-1);
        scnmin = new Ponto(-1,-1);
        windowmax = new Ponto(1,1);
        scnmax = new Ponto(1,1);

        viewport = Viewport::getInstance();
        transform = Transform::getInstance();
        clip = ClipPlane::getInstance();
        pos = new Ponto();
        calculate_matrix();
    }

    ListaEnc<Shape*>* getShapeList() {
        return shapes;
    }

    ListaEnc<Shape*>* getNormList() {
        return normShapes;
    }

    static Camera3D* getInstance() {
        static Camera3D* camera;
        if (!camera) {
            camera = new Camera3D();
        }
        return camera;
    }
    
    Ponto* getClipMin(){
        return clip->getMin();
    }
    
    Ponto* getClipMax(){
        return clip->getMax();
    }

    Ponto* winCenter() {
        return new Ponto(-pos->getX() + (windowmax->getX() + windowmin->getX()) / 2, -pos->getY() + (windowmax->getY() + windowmin->getY()) / 2);
    }

    void rotateCamera(int degrees) {
        rot += degrees;
    }

    int getRot() {
        return rot;
    }

    void moveCamera(float xAmount, float yAmount) {        
        
        windowmin->move_by(-xAmount,yAmount);
        windowmax->move_by(-xAmount,yAmount);
        scnmin->move_by(-xAmount,yAmount);
        scnmax->move_by(-xAmount,yAmount);
        clip->move(-xAmount,yAmount);

    }

    Ponto* getPos() {
        return pos;
    }

    Ponto* T(Ponto* p) {
        return transform->transform(p);
    }

    Ponto* rotateTransform(int angle, Ponto* p, Ponto* c) {
        return transform->rotationT2D(angle, p, c);
    }

    Ponto* clickTransform(Ponto* p) {
        //calculate_matrix();
        return transform->cT(p, zoom, rot);
    }

    float getZoom() {
        return zoom;
    }

    void calculate_matrix() {
        transform->calculate_viewport_transform(viewport->min(), viewport->max(), windowmin, windowmax, winCenter());

    }

    void Zoom(bool in) {
        if (in) {
            zoom += 0.1f;
            if (zoom > 1.5f) {
                zoom = 1.5f;
                return;
            }
            windowmin->move_by(width / 10, height / 10);
            windowmax->move_by(-width / 10, -height / 10);
            
        } else {
            zoom -= 0.1f;
            if (zoom < 0.1f) {
                zoom = 0.1f;
                return;
            }
            
            windowmin->move_by(-width/10,-height/10);
            windowmax->move_by(width/10,height/10);
        }
        //calculate_matrix();
    }

    void Zoom(float value) {
        zoom = value / 100;
    }

    void SCN() {

        transform->setT(transform->set_2D_move_matrix(-winCenter()->getX(), -winCenter()->getY()));
        transform->concatenate_matrix(transform->set_2D_rotation_matrix(-rot));
        transform->concatenate_matrix(transform->set_2D_scale_matrix(zoom, zoom));
        transform->concatenate_matrix(transform->set_2D_move_matrix(winCenter()->getX(), winCenter()->getY()));

        normShapes->clean();
        
        for (int i = 0; i < shapes->getSize(); i++) {
            Shape* s = shapes->get(i);
            ListaEnc<Ponto*>* nPontos = new ListaEnc<Ponto*>();
            for (int j = 0; j < s->getPontos()->getSize(); j++) {
                nPontos->adiciona(transform->transform(new Ponto((s->getPontos()->get(j)->getX()) , (s->getPontos()->get(j)->getY()))));
            }
            Shape* n = new Shape(nPontos);
            normShapes->adiciona(n);
        }

    }

};

#endif /* CAMERA2_HPP */

