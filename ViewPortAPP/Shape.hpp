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
    string name;

protected:
    bool fillShape = false;
    bool line = false;
    int type = 0;


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
        clipPs->clean();
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

    void clipCS(Ponto* clipMin, Ponto* clipMax, Ponto* p1, Ponto* p2) {

        for (int i = 0; i < pontos->getSize(); i++) {
            Ponto* p = pontos->get(i);
            findRC(p, clipMin, clipMax);
        }


        if (((p1->getRC() && p2->getRC()) == RegionCode()) && p1->getRC() != p2->getRC()) { //Parcialmente fora
            double m = (p1->getY() - p2->getY()) / ((p1->getX() - p2->getX()) + 0.000001f);
            double Ye, Yd, Xf, Xt;
            Ponto* es, *di, *ci, *ba;

            if (p1->getRC() == RegionCode()) {
                if (!clipPs->exists(p1)) {
                    clipPs->adiciona(p1);
                }
            }

            if (p1->getRC()[3] || p2->getRC()[3]) {
                Ye = m * (clipMin->getX() - p1->getX()) + p1->getY();
                if (Ye >= clipMin->getY() && Ye <= clipMax->getY()) {
                    es = new Ponto(clipMin->getX(), Ye);
                    clipPs->adiciona(es);
                }
            }

            if (p1->getRC()[2] || p2->getRC()[2]) {
                Yd = m * (clipMax->getX() - p1->getX()) + p1->getY();
                if (Yd >= clipMin->getY() && Yd <= clipMax->getY()) {
                    di = new Ponto(clipMax->getX(), Yd);
                    clipPs->adiciona(di);
                }
            }

            if (p1->getRC()[1] || p2->getRC()[1]) {
                Xf = p1->getX() + (1 / m) * (clipMin->getY() - p1->getY());
                if (Xf >= clipMin->getX() && Xf <= clipMax->getX()) {
                    ba = new Ponto(Xf, clipMin->getY());
                    clipPs->adiciona(ba);
                }
            }

            if (p1->getRC()[0] || p2->getRC()[0]) {
                Xt = p1->getX() + (1 / m) * (clipMax->getY() - p1->getY());
                if (Xt >= clipMin->getX() && Xt <= clipMax->getX()) {
                    ci = new Ponto(Xt, clipMax->getY());
                    clipPs->adiciona(ci);
                }
            }

            if (p2->getRC() == RegionCode()) {
                if (!clipPs->exists(p2)) {
                    clipPs->adiciona(p2);
                }
            }

        }

        if (p1->getRC() == p2->getRC() && p1->getRC() == RegionCode()) { //totalmente dentro
            if (!clipPs->exists(p1)) {
                clipPs->adiciona(p1);
            }
            if (!clipPs->exists(p2)) {
                clipPs->adiciona(p2);
            }
        }
        


    }

    ListaEnc<Ponto*>* clipLB(Ponto* pi, Ponto* pf, Ponto* clipMin, Ponto* clipMax) {

        Ponto* pmin;
        Ponto* pmax;

        float p1 = -(pf->getX() - pi->getX()); //Só mudei os nomes, acho que vc meio que não viu que algumas vez em vez de get(), vc colocou getSize()... de boa
        float p2 = -p1;
        float p3 = -(pf->getY() - pi->getY());
        float p4 = -p3;
        float q1 = pi->getX() - clipMin->getX();
        float q2 = clipMax->getX() - pi->getX();
        float q3 = pi->getY() - clipMin->getY();
        float q4 = clipMax->getY() - pi->getY();

        float r1 = q1 / p1;
        float r3 = q3 / p3;
        float u1 = max(0.0f, max(r1, r3));
        float r2 = q2 / p2;
        float r4 = q4 / p4;
        float u2 = min(1.0f, min(r2, r4));

        if (u1 != 0) {
            pmin = new Ponto(clipMin->getX(), pontos->getHead()->getY() + u1 * p4); //precisa dar uma olhada aqui... não sei se é assim mesmo...
        } else { // tipo o u1 são os valores negativos
            pmin = pi;
        }

        if (u2 != 1) {
            pmax = new Ponto(pontos->getHead()->getX() + u2*p2, clipMax->getY());
        } else { //e o u2 são os positivos... acho
            pmax = pontos->get(pontos->getSize());
        }
    }

    void polClip(Ponto* clipMin, Ponto* clipMax) {

        for (int i = 0; i < pontos->getSize(); i++) {
            Ponto* p = pontos->get(i);
            findRC(p, clipMin, clipMax);
        }
        Ponto* atual = pontos->get(0);
        Ponto* anterior = pontos->get(pontos->getSize() - 1);

        for (int i = 1; i <= pontos->getSize(); i++) {

            if ((anterior->getRC() == RegionCode()) && (atual->getRC() != RegionCode())) {
                clipCS(clipMin, clipMax, anterior, atual);
                cout << "dentro -> fora" << endl;

            }


            if ((anterior->getRC() != RegionCode()) && (atual->getRC() == RegionCode())) {
                clipCS(clipMin, clipMax, anterior, atual);
                cout << "fora -> dentro" << endl;

            } else if (atual->getRC() == RegionCode()) {
                clipPs->adiciona(atual);
            }

            if (i < pontos->getSize()) {
                anterior = pontos->get(i - 1);
                atual = pontos->get(i);
            }

        }


    }

    void clip(Ponto* clipMin, Ponto* clipMax) {

        clipPs->clean();

        if (type == 0) {
            if (!pointClip(pontos->getHead(), clipMin, clipMax)) {
                clipPs->adiciona(pontos->getHead());
                clipPs->adiciona(pontos->get(1));
            }
        } else
            if (type == 1) {
            clipCS(clipMin, clipMax, pontos->getHead(), pontos->get(1));
        } else {
            polClip(clipMin, clipMax);
        }
        cout << endl;

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
            if (!line) {
                atual = clipPs->get(0);
                atual = transform->dT(atual);
                cairo_line_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());
            }
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

    void setFill(bool f = true) {
        this->fillShape = f;
    }

    void setType(int t) {
        this->type = t;
    }

    void setLine(bool l = true) {
        this->line = l;
    }

    bool getFill() {
        return fillShape;
    }

    int getType() {
        return type;
    }

    bool getLine() {
        return line;
    }


};

class Reta : public Shape {
public:

    Reta(Ponto* p1, Ponto* p2) {
        this->addPoints(p1);
        this->addPoints(p2);
        this->setType(1);
        this->setLine();
    }
};

class Retangulo : public Shape {
public:

    Retangulo(double x, double y, double width, double height) {
        Ponto * p[] = {new Ponto(x, y), new Ponto(x + width, y), new Ponto(x + width, y + height), new Ponto(x, y + height)};
        this->addPoints(4, p);
        this->setType(2);
    }
};

class Point : public Retangulo {
public:

    Point(double x, double y) : Retangulo(x, y, 0.005f, 0.005f) {
        this->setType(0);
    }
};

class Quadrado : public Retangulo {
public:

    Quadrado(double x, double y, double size) : Retangulo(x, y, size, size) {
        this->setType(3);
    }
};

class Poligono : public Shape {
public:

    Poligono(double x, double y, ListaEnc<Ponto*>* p) {
        for (int i = 0; i < p->getSize(); i++) {
            p->get(i)->move_to(p->get(i)->getX() + x, p->get(i)->getY() + y);
        }
        this->setPointsList(p);
        this->setType(4);
    }


};

#endif /* SHAPE_HPP */

