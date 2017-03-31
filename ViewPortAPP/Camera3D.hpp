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
#include "Shape.hpp"

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

    ListaEnc<Shape*> * shapes = new ListaEnc<Shape*>();
    ListaEnc<Shape*> * normShapes = new ListaEnc<Shape*>();

    Ponto* pos;
    Viewport* viewport;
    Transform* transform;
    int rot = 0;
    float zoom = 1.0f;

public:

    Camera3D() {
        height = 600;
        width = 600;

        windowmin = new Ponto();
        ogmin = new Ponto();
        windowmax = new Ponto(600, 600);
        ogmax = new Ponto(600, 600);

        viewport = Viewport::getInstance();
        transform = Transform::getInstance();
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

    Ponto* winCenter() {
        return new Ponto(-pos->getX() + (windowmax->getX() + windowmin->getX()) / 2, -pos->getY() + (windowmax->getY() + windowmin->getY()) / 2);
    }

    void rotateCamera(int degrees) {
        rot = degrees;
    }

    int getRot() {
        return rot;
    }

    void moveCamera(float xAmount, float yAmount) {
        pos->move_to(pos->getX() + xAmount, pos->getY() + yAmount);
        ogmin = new Ponto(pos->getX(), pos->getY());
        ogmax = new Ponto(pos->getX() + width, pos->getY() + height);
        cout << ogmin->getX() << "," << ogmin->getY() << endl;

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
        calculate_matrix();
        return transform->cT(p);
    }

    float getZoom() {
        return zoom;
    }

    void calculate_matrix() {
        transform->calculate_viewport_transform(viewport->min(), viewport->max(), windowmin, windowmax, zoom, winCenter());

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
        /*
        float medx = (ogmax->getX() + ogmin->getX()) / 2;
        float medy = (ogmax->getY() + ogmin->getY()) / 2;
        float zx = medx * zoom;
        float zy = medy * zoom;
        windowmin->move_to((ogmin->getX() - zx + medx) , (ogmin->getY() - zy + medy));
        windowmax->move_to((ogmax->getX() + zx - medx), (ogmax->getY() + zy - medy));
         */
        calculate_matrix();
    }

    void Zoom(float value) {
        zoom = value / 100;
    }

    void SCN() {
        
        transform->setT(transform->set_2D_move_matrix(-winCenter()->getX(), -winCenter()->getY()));
        transform->concatenate_matrix(transform->set_2D_rotation_matrix(-rot));
        transform->concatenate_matrix(transform->set_2D_scale_matrix(zoom, zoom));
        transform->concatenate_matrix(transform->set_2D_move_matrix(winCenter()->getX(), winCenter()->getY()));
        
        if (normShapes->listaVazia()) {
            for (int i = 0; i < shapes->getSize(); i++) {
                normShapes->adiciona(shapes->get(i));
            }
        }
        
        for (int i = 0; i < shapes->getSize(); i++) {
            Shape* s = shapes->get(i);
            ListaEnc<Ponto*>* nPontos = new ListaEnc<Ponto*>();
            for(int j = 0; j < s->getPontos()->getSize(); j++){
                nPontos->adiciona(transform->transform( new Ponto(s->getPontos()->get(j)->getX() / width, s->getPontos()->get(j)->getY() / height)));
            }
            Shape* n = new Shape(nPontos);
            normShapes->modifica(i,n);
            
        }

    }

};

//#endif /* CAMERA_HPP */

#endif /* CAMERA_HPP */

