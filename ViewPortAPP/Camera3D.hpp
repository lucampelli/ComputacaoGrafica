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
    Ponto* windowtl;
    Ponto* windowmax;
    Ponto* windowbr;
    Ponto* vUp = new Ponto(0, 1, 0);
    Ponto* viewFocus = new Ponto(0, 0, -1);
    Ponto* zVect = new Ponto(0, 0, 1);

    ListaEnc<Shape3D*> * shapes = new ListaEnc<Shape3D*>();
    ListaEnc<Shape3D*> * shapesPersp = new ListaEnc<Shape3D*>();
    ListaEnc<Shape3D*> * normShapes = new ListaEnc<Shape3D*>();

    bool bugBoolean = false;

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

        windowmin = new Ponto(-1, -1, 0);
        windowbr = new Ponto(1, -1, 0);
        windowtl = new Ponto(-1, 1, 0);
        scnmin = new Ponto(-1, -1, 0);
        windowmax = new Ponto(1, 1, 0);
        scnmax = new Ponto(1, 1, 0);

        viewport = Viewport::getInstance();
        transform = Transform::getInstance();
        clip = ClipPlane::getInstance();
        pos = new Ponto();
        calculate_matrix();
    }

    ListaEnc<Shape3D*>* getShapeList() {
        return shapes;
    }

    ListaEnc<Shape3D*>* getShapePerspList() {
        return shapesPersp;
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
        return new Ponto(-pos->getX() + (windowmax->getX() + windowmin->getX()) / 2, -pos->getY() + (windowmax->getY() + windowmin->getY()) / 2, -pos->getZ() + (windowmax->getZ() + windowmin->getZ()) / 2);
    }

    void rotateCameraZ(int degrees) {
        rotZ += degrees;
        transform->setT3D(transform->set_3D_move_matrix(-winCenter()->getX(), -winCenter()->getY(), -winCenter()->getZ()));
        transform->concatenate_matrix_3D(transform->set_3Dz_rotation_matrix(degrees));
        transform->concatenate_matrix_3D(transform->set_3D_move_matrix(winCenter()->getX(), winCenter()->getY(), winCenter()->getZ()));
        zVect = transform->transform3D(zVect);
        viewFocus = transform->transform3D(viewFocus);
        windowmin = transform->transform3D(windowmin);
        windowtl = transform->transform3D(windowtl);
        windowbr = transform->transform3D(windowbr);
        windowmax = transform->transform3D(windowmax);
        bugBoolean = true;
    }

    void rotateCameraX(int degrees) {
        rotX += degrees;
        transform->setT3D(transform->set_3D_move_matrix(-winCenter()->getX(), -winCenter()->getY(), -winCenter()->getZ()));
        transform->concatenate_matrix_3D(transform->set_3Dx_rotation_matrix(degrees));
        transform->concatenate_matrix_3D(transform->set_3D_move_matrix(winCenter()->getX(), winCenter()->getY(), winCenter()->getZ()));
        zVect = transform->transform3D(zVect);
        viewFocus = transform->transform3D(viewFocus);
        windowmin = transform->transform3D(windowmin);
        windowtl = transform->transform3D(windowtl);
        windowbr = transform->transform3D(windowbr);
        windowmax = transform->transform3D(windowmax);
        bugBoolean = true;

    }

    void rotateCameraY(int degrees) {
        rotY += degrees;
        transform->setT3D(transform->set_3D_move_matrix(-winCenter()->getX(), -winCenter()->getY(), -winCenter()->getZ()));
        transform->concatenate_matrix_3D(transform->set_3Dy_rotation_matrix(degrees));
        transform->concatenate_matrix_3D(transform->set_3D_move_matrix(winCenter()->getX(), winCenter()->getY(), winCenter()->getZ()));
        zVect = transform->transform3D(zVect);
        viewFocus = transform->transform3D(viewFocus);
        windowmin = transform->transform3D(windowmin);
        windowtl = transform->transform3D(windowtl);
        windowbr = transform->transform3D(windowbr);
        windowmax = transform->transform3D(windowmax);
        bugBoolean = true;
    }

    int getRot() {
        return rotZ;
    }

    void moveCamera(double xAmount, double yAmount) {
        transform->setT3D(transform->set_3Dx_rotation_matrix(-rotX));
        transform->concatenate_matrix_3D(transform->set_3Dy_rotation_matrix(-rotY));
        transform->concatenate_matrix_3D(transform->set_3Dz_rotation_matrix(-rotZ));
        Ponto* p = transform->transform3D(new Ponto(-xAmount, yAmount, 0));

        windowmin->move_by(p->getX(), p->getY(), p->getZ());
        windowmax->move_by(p->getX(), p->getY(), p->getZ());
        windowtl->move_by(p->getX(), p->getY(), p->getZ());
        windowbr->move_by(p->getX(), p->getY(), p->getZ());
        scnmin->move_by(p->getX(), p->getY(), p->getZ());
        scnmax->move_by(p->getX(), p->getY(), p->getZ());
        viewFocus->move_by(p->getX(), p->getY(), p->getZ());
        zVect->move_by(p->getX(), p->getY(), p->getZ());
        if (rotX == 0 && rotY == 0 && rotZ == 0 && !bugBoolean) {
            clip->move(p->getX(), p->getY());
        } else if (!rotX == 0 && rotY == 0 && rotZ == 0 && !bugBoolean) {
            bugBoolean = true;
        }

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
        return transform->cT(p, zVect, rotX, rotY, rotZ);
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

private:

    void pointPersp(double focus) {


        transform->setT3D(transform->set_3D_move_matrix(-viewFocus->getX(), -viewFocus->getY(), -viewFocus->getZ()));
        transform->concatenate_matrix_3D(transform->set_3Dx_rotation_matrix(-rotX));
        transform->concatenate_matrix_3D(transform->set_3Dy_rotation_matrix(-rotY));

        for (int i = 0; i < shapesPersp->getSize(); i++) {
            shapesPersp->get(i)->applyT(false);
            shapesPersp->get(i)->retirarW(focus);
        }




    }

    void ortoPersp() {
        transform->setT3D(transform->set_3D_move_matrix(-winCenter()->getX(), -winCenter()->getY(), -winCenter()->getZ()));
        transform->concatenate_matrix_3D(transform->set_3Dx_rotation_matrix(-rotX));
        transform->concatenate_matrix_3D(transform->set_3Dy_rotation_matrix(-rotY));


        for (int i = 0; i < shapesPersp->getSize(); i++) {
            shapesPersp->get(i)->applyT(false);
        }


    }

public:

    void perspective(float focus) {

        shapesPersp->clean();

        for (int i = 0; i < shapes->getSize(); i++) {
            Shape3D* s = shapes->get(i);
            Shape3D* n = new Shape3D();

            for (int j = 0; j < s->getTris()->getSize(); j++) {
                Surface* surf = s->getTris()->get(j);
                Surface* temp = new Surface();

                for (int k = 0; k < surf->size(); k++) {
                    Aresta* a = surf->get(k);
                    temp->set(new Aresta(new Ponto((a->p1->getX()), (a->p1->getY()), (a->p1->getZ())),
                            new Ponto((a->p2->getX()), (a->p2->getY()), (a->p2->getZ()))));
                }

                n->addTris(temp);
            }
            n->setFill(s->getFill());
            n->setLine(s->getLine());
            n->setType(s->getType());
            n->setChildren(s->getChildren());
            n->setRGB(s->getRGB()[0], s->getRGB()[1], s->getRGB()[2]);
            shapesPersp->adiciona(n);
        }

        if (focus == 0) {
            ortoPersp();
        } else {
            pointPersp(1.9f + focus);
        }
    }

    void SCN() {

        transform->setT(transform->set_2D_move_matrix(-winCenter()->getX(), -winCenter()->getY()));
        transform->concatenate_matrix(transform->set_2D_rotation_matrix(-rotZ));
        transform->concatenate_matrix(transform->set_2D_scale_matrix(zoom, zoom));
        transform->concatenate_matrix(transform->set_2D_move_matrix(winCenter()->getX(), winCenter()->getY()));

        normShapes->clean();

        for (int i = 0; i < shapesPersp->getSize(); i++) {
            Shape3D* s = shapesPersp->get(i);
            Shape3D* n = new Shape3D();

            for (int j = 0; j < s->getTris()->getSize(); j++) {
                Surface* surf = s->getTris()->get(j);
                Surface* temp = new Surface();

                for (int k = 0; k < surf->size(); k++) {
                    Aresta* a = surf->get(k);
                    temp->set(new Aresta(transform->transform(new Ponto((a->p1->getX()), (a->p1->getY()), (a->p1->getZ()))),
                            transform->transform(new Ponto((a->p2->getX()), (a->p2->getY()), (a->p2->getZ())))));
                }

                n->addTris(temp);
            }
            n->setFill(s->getFill());
            n->setLine(s->getLine());
            n->setType(s->getType());
            n->setChildren(s->getChildren());
            n->setRGB(s->getRGB()[0], s->getRGB()[1], s->getRGB()[2]);
            normShapes->adiciona(n);
        }

    }

};

#endif /* CAMERA3D_HPP */

