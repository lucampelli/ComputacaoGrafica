/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shape.hpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
 *
 * Created on 9 de Março de 2017, 16:57
 */

#include "ListaEnc.hpp"
#include "gtk/gtk.h"
#include "Transform.hpp"
#include <iostream>
#ifndef SHAPE_HPP
#define SHAPE_HPP

using namespace std;

class Shape {
private:
    Transform* transform = Transform::getInstance();
    Ponto* pos;
    ListaEnc<Ponto*>* pontos;
    ListaEnc<Ponto*>* clipPs = new ListaEnc<Ponto*>();
    int vertices;
    int rotation2D = 0;
    double scaleX = 1;
    double scaleY = 1;
    bool fillShape = false;
    string name;

public:

    Shape() : vertices(0) {
        pos = new Ponto();
        pontos = new ListaEnc<Ponto*>();

    }

    Shape(double x, double y) : vertices(0) {
        pos = new Ponto(x, y);
        pontos = new ListaEnc<Ponto*>();

    }

    Shape(Ponto* p) : vertices(1) {

        pos = p;
        pontos = new ListaEnc<Ponto*>();
        pontos->adiciona(p);

    }

    Shape(ListaEnc<Ponto*>* points) {
        pos = new Ponto();
        pontos = points;
        vertices = points->getSize();

    }

    Shape(double x, double y, ListaEnc<Ponto*>* points) {
        pos = new Ponto(x, y);
        pontos = points;
        vertices = points->getSize();

    }

    void setFill() {
        fillShape = true;
    }

    void setName(string n) {
        name = n;
    }

    string getName() {
        return name;
    }

    void addPoints(Ponto* p) {
        pontos->adiciona(p);
        vertices++;
    }

    void addPoints(int num, Ponto* p[]) {
        for (int i = 0; i < num; i++) {
            pontos->adiciona(p[i]);
            vertices++;
        }
    }

    void setPointsList(ListaEnc<Ponto*>* p) {
        this->pontos = p;
        this->vertices = p->getSize();
    }

    ListaEnc<Ponto*>* getPontos() {
        return pontos;
    }

    Ponto* findCenter() {
        double x = 0;
        double y = 0;
        for (int i = 0; i < vertices; i++) {
            x += pontos->get(i)->getX();
            y += pontos->get(i)->getY();
        }

        x = x / vertices;
        y = y / vertices;

        return new Ponto(x, y);
    }

    void setRot(int degrees) { //usar este método para rotacionar a figura pelo centro.
        rotation2D = degrees;
        applyT();
    }

    void setScale(double X, double Y) {
        scaleX = X;
        scaleY = Y;
        applyT();
    }

    void move(double Dx, double Dy) {
        for (int i = 0; i < pontos->getSize(); i++) {
            pontos->get(i)->move_by(Dx, Dy);
        }
    }

    void applyT() { //e este para rotacionar ao redor de um ponto qualquer
        for (int i = 0; i < vertices; i++) {

            Ponto* p = transform->transform(pontos->get(i));
            pontos->get(i)->move_to(p->getX(), p->getY());

        }
    }

    bool pointClip(Ponto* p, Ponto* clipMin, Ponto* clipMax) {
        if (p->getX() < clipMin->getX()) {
            return true;
        }
        if (p->getX() > clipMax->getX()) {
            return true;
        }
        if (p->getY() < clipMin->getY()) {
            return true;
        }
        if (p->getY() > clipMax->getY()) {
            return true;
        }
        return false;
    }

    void findRC(Ponto* p, Ponto* clipMin, Ponto* clipMax) {
        RegionCode rc = RegionCode();

        if (p->getX() < clipMin->getX()) {
            //0001
            rc.set(true, 3);
        }
        if (p->getX() > clipMax->getX()) {
            //0010
            rc.set(true, 2);
        }
        if (p->getY() < clipMin->getY()) {
            //0100
            rc.set(true, 1);
        }
        if (p->getY() > clipMax->getY()) {
            //1000
            rc.set(true, 0);
        }
        p->setRC(rc);
    }

    ListaEnc<Ponto*>* clipCS(Ponto* clipMin, Ponto* clipMax) {
        clipPs->clean();

        for (int i = 0; i < pontos->getSize(); i++) {
            Ponto* p = pontos->get(i);
            findRC(p, clipMin, clipMax);
        }
        Ponto* p1;
        Ponto* p2;

        for (int i = 0; i < pontos->getSize(); i++) {
            p1 = pontos->get(i);
            if (i + 1 == pontos->getSize()) {
                p2 = pontos->getHead();
            } else {
                p2 = pontos->get(i + 1);
            }


            if (((p1->getRC() && p2->getRC()) == RegionCode()) && p1->getRC() != p2->getRC()) { //Parcialmente fora
                double m = (p1->getY() - p2->getY()) / ((p1->getX() - p2->getX()) + 0.000001f);
                double Ye, Yd, Xf, Xt;
                Ponto* es, *di, *ci, *ba;

                if(p1->getRC() == RegionCode()){
                    clipPs->adiciona(p1);
                }
                
                if (p1->getRC()[3] || p2->getRC()[3]) {
                    Ye = m * (clipMin->getX() - p1->getX()) + p1->getY();
                    if (Ye >= clipMin->getY() && Ye <= clipMax->getY()) {
                        es = new Ponto(clipMin->getX(), Ye);
                    }
                    clipPs->adiciona(es);
                }

                if (p1->getRC()[2] || p2->getRC()[2]) {
                    Yd = m * (clipMax->getX() - p1->getX()) + p1->getY();
                    if (Yd >= clipMin->getY() && Yd <= clipMax->getY()) {
                        di = new Ponto(clipMax->getX(), Yd);
                    }
                    clipPs->adiciona(di);
                }

                if (p1->getRC()[1] || p2->getRC()[1]) {
                    Xf = p1->getX() + (1 / m) * (clipMin->getY() - p1->getY());
                    if (Xf >= clipMin->getX() && Xf <= clipMax->getX()) {
                        ba = new Ponto(Xf, clipMin->getY());
                    }
                    clipPs->adiciona(ba);
                }

                if (p1->getRC()[0] || p2->getRC()[0]) {
                    Xt = p1->getX() + (1 / m) * (clipMax->getY() - p1->getY());
                    if (Xt >= clipMin->getY() && Xt <= clipMax->getY()) {
                        ci = new Ponto(Xt, clipMax->getY());
                    }
                    clipPs->adiciona(ci);
                }
                
                if(p2->getRC() == RegionCode()){
                    if(!clipPs->exists(p2)){
                        clipPs->adiciona(p2);
                    }
                    
                }
            }

            if (p1->getRC() == p2->getRC() && p1->getRC() == RegionCode()) { //totalmente dentro
                clipPs->adiciona(p1);
            }
        }

    }

    ListaEnc<Ponto*>* clipLB() {

    }

    void draw(cairo_t* cr, Ponto* camPos) {

        Ponto* center = findCenter();
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 0.5);
        //do_transform;
        if (clipPs->getSize() > 1) {
            Ponto* atual = clipPs->get(0);
            atual = transform->dT(atual);
            cairo_move_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());

            for (int i = 1; i < clipPs->getSize(); i++) {
                Ponto* atual = clipPs->get(i);
                atual = transform->dT(atual);
                cairo_line_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY()); //good
                cairo_move_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());
            }
            atual = clipPs->get(0);
            atual = transform->dT(atual);
            cairo_line_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());

            cairo_stroke(cr);
            if (fillShape) {
                cairo_fill(cr);
            }
        }


    }

    void printPontos() {
        cout << pontos->getSize() << endl;
        for (int i = 0; i < pontos->getSize(); i++) {
            cout << pontos->get(i)->getX() << "," << pontos->get(i)->getY() << endl;
        }
    }

};

class Retangulo : public Shape {
public:

    Retangulo(double x, double y, double width, double height) {
        Ponto * p[] = {new Ponto(x, y), new Ponto(x + width, y), new Ponto(x + width, y + height), new Ponto(x, y + height)};
        this->addPoints(4, p);
    }
};

class Point : public Retangulo {
public:

    Point(double x, double y) : Retangulo(x, y, 0.005f, 0.005f) {
    }
};

class Quadrado : public Retangulo {
public:

    Quadrado(double x, double y, double size) : Retangulo(x, y, size, size) {
    }
};

class Poligono : public Shape {
public:

    Poligono(double x, double y, ListaEnc<Ponto*>* p) {
        for (int i = 0; i < p->getSize(); i++) {
            p->get(i)->move_to(p->get(i)->getX() + x, p->get(i)->getY() + y);
        }
        this->setPointsList(p);
    }


};

#endif /* SHAPE_HPP */

