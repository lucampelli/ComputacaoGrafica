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
#include "Shape3D.hpp"

#ifndef CAMERA3D_HPP
#define CAMERA3D_HPP

class Camera3D {
private:
    double width;
    double height;
    double depth;
    Ponto* scnmin;
    Ponto* scnmax;
    Ponto* windowmin;
    Ponto* windowmax;
    Ponto* vUp = new Ponto(0, 1, 0);
    Ponto* zVect = new Ponto(0, 0, 1);

    ListaEnc<Shape3D*> * shapes = new ListaEnc<Shape3D*>();
    ListaEnc<Shape3D*> * shapes2D = new ListaEnc<Shape3D*>();
    ListaEnc<Shape3D*> * normShapes = new ListaEnc<Shape3D*>();

    Ponto* pos;
    Viewport* viewport;
    Transform* transform;
    ClipPlane* clip;
    int rotX = 0;
    int rotY = 0;
    int rotZ = 0;
    double zoom = 1.0f;

public:

    Camera3D() {
        height = 2;
        width = 2;

        windowmin = new Ponto(-1, -1);
        scnmin = new Ponto(-1, -1);
        windowmax = new Ponto(1, 1);
        scnmax = new Ponto(1, 1);

        viewport = Viewport::getInstance();
        transform = Transform::getInstance();
        clip = ClipPlane::getInstance();
        pos = new Ponto();
        calculate_matrix();
    }

    ListaEnc<Shape3D*>* getShapeList() {
        return shapes;
    }

    ListaEnc<Shape3D*>* getNormList() {
        return normShapes;
    }

    static Camera3D* getInstance() {
        static Camera3D* camera;
        if (!camera) {
            camera = new Camera3D();
        }
        return camera;
    }

    Ponto* getClipMin() {
        return clip->getMin();
    }

    Ponto* getClipMax() {
        return clip->getMax();
    }

    Ponto* winCenter() {
        return new Ponto(-pos->getX() + (windowmax->getX() + windowmin->getX()) / 2, -pos->getY() + (windowmax->getY() + windowmin->getY()) / 2);
    }

    void rotateCameraZ(int degrees) {
        rotZ += degrees;
    }

    void rotateCameraX(int degrees) {
        rotX += degrees;
    }

    void rotateCameraY(int degrees) {
        rotY += degrees;
    }

    int getRot() {
        return rotZ;
    }

    void moveCamera(double xAmount, double yAmount) {

        windowmin->move_by(-xAmount, yAmount);
        windowmax->move_by(-xAmount, yAmount);
        scnmin->move_by(-xAmount, yAmount);
        scnmax->move_by(-xAmount, yAmount);
        clip->move(-xAmount, yAmount);

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
        return transform->cT(p, zoom, rotZ);
    }

    double getZoom() {
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

            windowmin->move_by(-width / 10, -height / 10);
            windowmax->move_by(width / 10, height / 10);
        }
    }

    void Zoom(double value) {
        zoom = value / 100;
    }

    void SwitchDimension() { //this will have the rotations to print everything on screen right
        Ponto* originalZVect = new Ponto(zVect->getX(), zVect->getY(), zVect->getZ());



    }

    void SCN() {

        transform->setT(transform->set_2D_move_matrix(-winCenter()->getX(), -winCenter()->getY()));
        transform->concatenate_matrix(transform->set_2D_rotation_matrix(-rotZ));
        transform->concatenate_matrix(transform->set_2D_scale_matrix(zoom, zoom));
        transform->concatenate_matrix(transform->set_2D_move_matrix(winCenter()->getX(), winCenter()->getY()));

        normShapes->clean();

        for (int i = 0; i < shapes->getSize(); i++) {
            Shape3D* s = shapes->get(i);
            Shape3D* n = new Shape3D();

            for (int j = 0; j < s->getTris()->getSize(); j++) {
                Surface* surf = s->getTris()->get(j);
                Surface* temp = new Surface();

                for (int k = 0; k < surf->size(); k++) {
                    Aresta* a = surf->get(k);
                    temp->set(new Aresta(transform->transform(new Ponto((a->p1->getX()), (a->p1->getY()))),
                            transform->transform(new Ponto((a->p2->getX()), (a->p2->getY())))));
                }

                n->addTris(temp);
            }
            n->setFill(s->getFill());
            n->setLine(s->getLine());
            n->setType(s->getType());
            normShapes->adiciona(n);
        }

    }

};

#endif /* CAMERA3D_HPP */

