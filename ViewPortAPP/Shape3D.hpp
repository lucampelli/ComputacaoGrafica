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
#include "Ponto.hpp"
#include "Transform.hpp"
#include <iostream>
#ifndef SHAPE3D_HPP
#define SHAPE3D_HPP

using namespace std;

struct Aresta {
    Ponto* p1;
    Ponto* p2;

    Aresta(Ponto* p1, Ponto* p2) : p1(p1), p2(p2) {
    }

    Ponto* get(int i) {
        if (i == 0) {
            return p1;
        } else if (i == 1) {
            return p2;
        } else {
            return NULL;
        }
    }

    void set(int i, Ponto* p) {
        if (i == 0) {
            p1 = p;
        } else if (i == 1) {
            p2 = p;
        }
    }

    int size() {
        int s = 0;
        if (p1) {
            s++;
        }
        if (p2) {
            s++;
        }
        return s;
    }

    bool equals(Aresta* a) {
        for (int i = 0; i < a->size() < size() ? a->size() : size(); i++) {
            if (a->get(i) != get(i)) {
                return false;
            }
        }
        return true;
    }
};

struct Surface {
    Aresta* a1;
    Aresta* a2;
    Aresta* a3;

    Surface(Aresta* a1 = NULL, Aresta* a2 = NULL, Aresta* a3 = NULL) : a1(a1), a2(a2), a3(a3) {
    }

    Surface(Ponto* p1, Ponto* p2, Ponto* p3) {
        a1 = new Aresta(p1, p2);
        a2 = new Aresta(p2, p3);
        a3 = new Aresta(p3, p1);
    }

    Ponto* getCenter() {
        Ponto* c = new Ponto();
        for (int i = 0; i < size(); i++) {
            c = c->sum(get(i)->p1);
        }
        return c->div(size());
    }

    Aresta* get(int i) {
        if (i == 0) {
            return a1;
        } else if (i == 1) {
            return a2;
        } else if (i == 2) {
            return a3;
        } else {
            return NULL;
        }
    }

    void set(Aresta* dado) {
        if (!a1) {
            a1 = dado;
        } else if (!a2) {
            a2 = dado;
        } else if (!a3) {
            a3 = dado;
        }
    }

    int size() {
        int s = 0;
        if (a1) {
            s++;
        }
        if (a2) {
            s++;
        }
        if (a3) {
            s++;
        }
        return s;
    }
};

class Shape3D {
private:
    Ponto* pos;
    ListaEnc<Surface*>* tris = new ListaEnc<Surface*>();
    ListaEnc<Surface*>* clipTris = new ListaEnc<Surface*>();
    int vertices;
    int rotationX = 0;
    int rotationY = 0;
    int rotationZ = 0;
    double scaleX = 1;
    double scaleY = 1;
    double scaleZ = 1;
    string name;

    ListaEnc<Shape3D*>* children = new ListaEnc<Shape3D*>();

protected:
    Transform* transform = Transform::getInstance();
    bool fillShape = false;
    bool line = false;
    int type = 0;
    float rgb[3] = {0, 0, 0};

public:

    Shape3D() : vertices(0) {
        pos = new Ponto();
    }

    Shape3D(double x, double y, double z) : vertices(0) {
        pos = new Ponto(x, y, z);
    }

    Shape3D(ListaEnc<Surface*>* tris) {
        pos = tris->getHead()->a1->p1;
        tris = tris;
        vertices = tris->getSize() + 2;

    }

    void setName(string n) {
        name = n;
    }

    string getName() {
        return name;
    }

    void addTris(Ponto* p1, Ponto* p2, Ponto* p3) {
        Surface* surf = new Surface(p1, p2, p3);
        tris->adiciona(surf);
    }

    void addTris(Surface* s) {
        tris->adiciona(s);
    }

    void setTrisList(ListaEnc<Surface*>* s) {
        this->tris = s;
        this->vertices = s->getSize() + 2;
    }

    ListaEnc<Surface*>* getTris() {
        return tris;
    }

    Ponto* findCenter() {
        Ponto* c = new Ponto();

        for (int i = 0; i < tris->getSize(); i++) {
            c = c->sum(tris->get(i)->getCenter());
        }
        for (int i = 0; i < children->getSize(); i++) {
            c = c->sum(children->get(i)->findCenter());
        }

        c = c->div(tris->getSize() + children->getSize());
        return c;
    }

    void setRotX(int degrees) { //usar este método para rotacionar a figura pelo centro.
        rotationX = degrees;
    }

    void setRotY(int degrees) { //usar este método para rotacionar a figura pelo centro.
        rotationY = degrees;
    }

    void setRotZ(int degrees) { //usar este método para rotacionar a figura pelo centro.
        rotationZ = degrees;
    }

    void setScale(double X, double Y, double Z) {
        scaleX = X;
        scaleY = Y;
        scaleZ = Z;
        applyT(true);
    }

    void move(double Dx, double Dy, double Dz) {
        pos->move_by(Dx, Dy, Dz);
    }

    void addChild(Shape3D* child) {
        children->adiciona(child);
    }

    ListaEnc<Shape3D*>* getChildren() {
        return children;
    }

    void setChildren(ListaEnc<Shape3D*>* child) {
        children = child;
    }

    void applyT(bool trans) { //aplica a transformada na shape

        ListaEnc<Ponto*>* passados = new ListaEnc<Ponto*>();

        for (int i = 0; i < tris->getSize(); i++) {
            passados->clean();
            for (int j = 0; j < tris->get(i)->size(); j++) {
                for (int k = 0; k < tris->get(i)->get(j)->size(); k++) {
                    if (!passados->exists(tris->get(i)->get(j)->get(k))) {
                        passados->adiciona(tris->get(i)->get(j)->get(k));
                        tris->get(i)->get(j)->set(k, transform->transform3D(tris->get(i)->get(j)->get(k)));
                    }
                }
            }
        }
        if (trans) {
            for (int i = 0; i < children->getSize(); i++) {
                children->get(i)->applyT(trans);
            }
        }
    }

    void retirarW(double focus) {
        for (int i = 0; i < tris->getSize(); i++) {
            Surface* s = tris->get(i);
            for (int j = 0; j < s->size(); j++) {
                Aresta* a = s->get(j);

                for (int k = 0; k < a->size(); k++) {
                    double W = a->get(k)->getZ() / focus;

                    //cout<< W << " : " << a->get(k)->getZ() << " : " << focus << endl;

                    double Xp = a->get(k)->getX() / W;
                    double Yp = a->get(k)->getY() / W;
                    double Zp = a->get(k)->getZ() / W;

                    a->get(k)->move_to(Xp, Yp, Zp);
                }
            }
        }
    }

    bool pointClip(Ponto* p, Ponto* clipMin, Ponto* clipMax) {
        clipTris->clean();
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

    Aresta* clipCS(Ponto* clipMin, Ponto* clipMax, Ponto* p1, Ponto* p2) {


        findRC(p1, clipMin, clipMax);
        findRC(p2, clipMin, clipMax);

        ListaEnc<Ponto*>* temp = new ListaEnc<Ponto*>();


        if (((p1->getRC() && p2->getRC()) == RegionCode()) && p1->getRC() != p2->getRC()) { //Parcialmente fora
            double m = (p1->getY() - p2->getY()) / ((p1->getX() - p2->getX()) + 0.000001f);
            double Ye, Yd, Xf, Xt;
            Ponto* es, *di, *ci, *ba;

            if (p1->getRC() == RegionCode()) {
                if (!temp->exists(p1)) {
                    temp->adiciona(p1);
                }
            }

            if (!p1->getRC()[1]) {
                if (p1->getRC()[3] || p2->getRC()[3]) {
                    Ye = m * (clipMin->getX() - p1->getX()) + p1->getY();
                    if (Ye >= clipMin->getY() && Ye <= clipMax->getY()) {
                        es = new Ponto(clipMin->getX(), Ye);
                        temp->adiciona(es);
                    }
                }
            }

            if (p1->getRC()[0] || p2->getRC()[0]) {
                Xt = p1->getX() + (1 / m) * (clipMax->getY() - p1->getY());
                if (Xt >= clipMin->getX() && Xt <= clipMax->getX()) {
                    ci = new Ponto(Xt, clipMax->getY());
                    temp->adiciona(ci);
                }
            }

            if (p1->getRC()[2] || p2->getRC()[2]) {
                Yd = m * (clipMax->getX() - p1->getX()) + p1->getY();
                if (Yd >= clipMin->getY() && Yd <= clipMax->getY()) {
                    di = new Ponto(clipMax->getX(), Yd);
                    temp->adiciona(di);
                }
            }


            if (p1->getRC()[1] || p2->getRC()[1]) {
                Xf = p1->getX() + (1 / m) * (clipMin->getY() - p1->getY());
                if (Xf >= clipMin->getX() && Xf <= clipMax->getX()) {
                    ba = new Ponto(Xf, clipMin->getY());
                    temp->adiciona(ba);
                }
            }

            if (p1->getRC()[1]) {
                if (p1->getRC()[3] || p2->getRC()[3]) {
                    Ye = m * (clipMin->getX() - p1->getX()) + p1->getY();
                    if (Ye >= clipMin->getY() && Ye <= clipMax->getY()) {
                        es = new Ponto(clipMin->getX(), Ye);
                        temp->adiciona(es);
                    }
                }
            }

            if (p2->getRC() == RegionCode()) {
                if (!temp->exists(p2)) {
                    temp->adiciona(p2);
                }
            }

        }

        if (p1->getRC() == p2->getRC() && p1->getRC() == RegionCode()) { //totalmente dentro
            if (!temp->exists(p1)) {
                temp->adiciona(p1);
            }
            if (!temp->exists(p2)) {
                temp->adiciona(p2);
            }
        }


        if (temp->getSize() == 2) {
            return new Aresta(temp->getHead(), temp->get(1));
        } else if (temp->getSize() == 1) {
            return new Aresta(temp->getHead(), NULL);
        } else {
            return NULL;
        }


    }

    void polClip(Ponto* clipMin, Ponto* clipMax) {

        for (int i = 0; i < tris->getSize(); i++) {
            for (int j = 0; j < tris->get(i)->size(); j++) {
                for (int k = 0; k < tris->get(i)->get(j)->size(); k++) {
                    findRC(tris->get(i)->get(j)->get(k), clipMin, clipMax);
                }
            }
        }


        for (int i = 0; i < tris->getSize(); i++) {
            Surface* sAtual = tris->get(i);
            Surface* clips = new Surface();

            for (int j = 0; j < sAtual->size(); j++) {
                Aresta* a = sAtual->get(j);
                Ponto* anterior = a->p1;
                Ponto* atual = a->p2;

                if ((anterior->getRC() == RegionCode()) && (atual->getRC() != RegionCode())) {
                    clips->set(clipCS(clipMin, clipMax, anterior, atual));
                }

                if (anterior->getRC() != RegionCode()) {
                    if (atual->getRC() != RegionCode()) {
                        if (atual->getRC() != anterior->getRC()) {
                            if ((atual->getRC() && anterior->getRC()) == RegionCode()) {
                                clips->set(clipCS(clipMin, clipMax, anterior, atual));
                            }
                        }
                    }
                }


                if ((anterior->getRC() != RegionCode()) && (atual->getRC() == RegionCode())) {
                    clips->set(clipCS(clipMin, clipMax, anterior, atual));

                }

                if ((anterior->getRC() == RegionCode()) && (atual->getRC() == RegionCode())) {
                    clips->set(new Aresta(anterior, atual));
                }

            }
            clipTris->adiciona(clips);
        }

    }

    void curveClip(Ponto* clipMin, Ponto* clipMax) {

        for (int i = 0; i < tris->getSize(); i++) {
            for (int j = 0; j < tris->get(i)->size(); j++) {
                for (int k = 0; k < tris->get(i)->get(j)->size(); k++) {
                    findRC(tris->get(i)->get(j)->get(k), clipMin, clipMax);
                }
            }
        }



        for (int i = 0; i < tris->getSize(); i++) {

            Surface* sAtual = tris->get(i);
            Surface* clips = new Surface();

            for (int j = 0; j < sAtual->size(); j++) {

                Aresta* aAtual = sAtual->get(j);
                Ponto* atual = aAtual->p2;
                Ponto* anterior = aAtual->p1;


                if ((anterior->getRC() == RegionCode()) && (atual->getRC() != RegionCode())) {
                    clips->set(clipCS(clipMin, clipMax, anterior, atual));
                }

                if (anterior->getRC() != RegionCode()) {
                    if (atual->getRC() != RegionCode()) {
                        if (atual->getRC() != anterior->getRC()) {
                            if ((atual->getRC() && anterior->getRC()) == RegionCode()) {
                                clips->set(clipCS(clipMin, clipMax, anterior, atual));
                            }
                        }
                    }
                }


                if ((anterior->getRC() != RegionCode()) && (atual->getRC() == RegionCode())) {
                    clips->set(clipCS(clipMin, clipMax, anterior, atual));
                }
                if ((anterior->getRC() == RegionCode()) && (atual->getRC() == RegionCode())) {
                    clips->set(new Aresta(anterior, atual));
                }
            }
            clipTris->adiciona(clips);
        }
    }

    void clip(Ponto* clipMin, Ponto* clipMax) {

        clipTris->clean();

        if (type == 0) {
            if (!pointClip(tris->getHead()->a1->p1, clipMin, clipMax)) {
                clipTris->adiciona(tris->getHead());
            }
        } else
            if (type == 1) {
            clipTris->adiciona(new Surface(clipCS(clipMin, clipMax, tris->getHead()->a1->p1, tris->getHead()->a1->p2), NULL, NULL));
        } else if (type != 5) {
            polClip(clipMin, clipMax);
        } else {
            curveClip(clipMin, clipMax);
        }

    }

    void draw(cairo_t* cr, Ponto* camPos) {

        Ponto* center = findCenter();
        cairo_set_source_rgb(cr, rgb[0], rgb[1], rgb[2]);
        cairo_set_line_width(cr, 1);
        //do_transform;

        Ponto* atual;
        if (clipTris->getSize() > 0) {
            for (int i = 0; i < clipTris->getSize(); i++) {
                Surface* sAtual = clipTris->get(i);

                for (int j = 0; j < sAtual->size(); j++) {
                    Aresta* aAtual = sAtual->get(j);
                    atual = aAtual->p1;
                    atual = transform->dT(atual);
                    cairo_move_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());


                    for (int k = 1; k < aAtual->size(); k++) {
                        atual = aAtual->get(k);
                        atual = transform->dT(atual);
                        cairo_line_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY()); //good
                        cairo_move_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());
                        cout << "X: " << -300 + atual->getX() + pos->getX() + camPos->getX() << " Y: " << -300 + atual->getY() + pos->getY() + camPos->getY() << endl;
                    }
                }

            }
            cout << endl;
            if (fillShape && !line) {
                cairo_stroke_preserve(cr);
                cairo_fill(cr);
            } else {
                cairo_stroke(cr);
            }


        }

        for (int i = 0; i < children->getSize(); i++) {
            children->get(i)->draw(cr, camPos);
        }


    }

    void printPontos() {
        //cout << clipTris->getSize() << endl;
        for (int i = 0; i < clipTris->getSize(); i++) {
            for (int j = 0; j < clipTris->get(i)->size(); j++) {
                for (int k = 0; k < clipTris->get(i)->get(j)->size(); k++) {
                    cout << clipTris->get(i)->get(j)->get(k)->getX() << " : ";
                    cout << clipTris->get(i)->get(j)->get(k)->getY() << " : ";
                    cout << clipTris->get(i)->get(j)->get(k)->getZ() << endl;
                }
            }
        }
    }

    void setRGB(float r, float g, float b) {
        this->rgb[0] = r;
        this->rgb[1] = g;
        this->rgb[2] = b;
    }

    float* getRGB() {
        return rgb;
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

class Reta3D : public Shape3D {
public:

    Reta3D(Ponto* p1, Ponto* p2) {
        Surface* s = new Surface(new Aresta(p1, p2), NULL, NULL);
        this->addTris(s);
        this->setType(1);
        this->setLine();
    }

    Reta3D(Ponto* p1, Ponto* p2, float r, float g, float b) {
        Surface* s = new Surface(new Aresta(p1, p2), NULL, NULL);
        this->addTris(s);
        this->setType(1);
        this->setLine();
        this->setRGB(r, g, b);
    }
};

class Retangulo3D : public Shape3D {
public:

    Retangulo3D(double x, double y, double z, double width, double height) {
        Ponto * p[] = {new Ponto(x, y, z), new Ponto(x, y + height, z), new Ponto(x + width, y + height, z), new Ponto(x + width, y, z)};
        this->addTris(new Surface(new Aresta(p[0], p[1]), NULL, NULL));
        this->addTris(new Surface(new Aresta(p[1], p[2]), NULL, NULL));
        this->addTris(new Surface(new Aresta(p[2], p[3]), NULL, NULL));
        this->addTris(new Surface(new Aresta(p[3], p[0]), NULL, NULL));
        this->setType(2);
    }
};

class Point3D : public Retangulo3D {
public:

    Point3D(double x, double y, double z = 1) : Retangulo3D(x, y, z, 0.005f, 0.005f) {
        this->setType(0);
    }
};

class Cubo3D : public Shape3D {
public:

    Cubo3D() {
        Ponto* p1 = new Ponto(0, 0, 0);
        Ponto* p2 = new Ponto(0, -0.3, 0);
        Ponto* p3 = new Ponto(0.3, -0.3, 0);
        this->addTris(new Surface(new Aresta(p1, p2), NULL, NULL));
        Ponto* p4 = new Ponto(0.3, 0, 0);
        this->addTris(new Surface(new Aresta(p2, p3), NULL, NULL));
        this->addTris(new Surface(new Aresta(p3, p4), NULL, NULL));
        this->addTris(new Surface(new Aresta(p4, p1), NULL, NULL));

        Ponto* p5 = new Ponto(0, 0, 0.3);
        Ponto* p6 = new Ponto(0, -0.3, 0.3);
        Ponto* p7 = new Ponto(0.3, -0.3, 0.3);
        this->addTris(new Surface(new Aresta(p5, p6), NULL, NULL));
        Ponto* p8 = new Ponto(0.3, 0, 0.3);
        this->addTris(new Surface(new Aresta(p6, p7), NULL, NULL));
        this->addTris(new Surface(new Aresta(p7, p8), NULL, NULL));
        this->addTris(new Surface(new Aresta(p8, p5), NULL, NULL));
        this->addTris(new Surface(new Aresta(p1, p5), NULL, NULL));
        this->addTris(new Surface(new Aresta(p6, p2), NULL, NULL));
        this->addTris(new Surface(new Aresta(p7, p3), NULL, NULL));
        this->addTris(new Surface(new Aresta(p4, p8), NULL, NULL));
        this->setType(3);
    }
};

class Quadrado3D : public Retangulo3D {
public:

    Quadrado3D(double x, double y, double z, double size) : Retangulo3D(0, 0, 0, 0.3, -0.3) {
        this->setType(3);
    }
};

class Poligono3D : public Shape3D {
public:

    Poligono3D(double x, double y, double z, ListaEnc<Ponto*>* p) {
        for (int i = 0; i < p->getSize(); i++) {
            p->get(i)->move_to(p->get(i)->getX() + x, p->get(i)->getY() + y, p->get(i)->getZ() + z);
        }

        for (int i = 0; i < p->getSize() - 1; i++) {
            Surface* s = new Surface(new Aresta(p->get(i), p->get(i + 1)));
            this->addTris(s);
        }
        Surface* s = new Surface(new Aresta(p->get(p->getSize() - 1), p->get(0)));
        this->addTris(s);
        this->setType(4);
    }
};

class CurvaBezier3D : public Shape3D {
public:

    void Bezier(Ponto* p1, Ponto* p2, Ponto* p3, Ponto* p4, float passo, ListaEnc<Ponto*>* outPontos) {
        Matriz* T = new Matriz(1, 4);
        Matriz* GBx = new Matriz(4, 1);
        Matriz* GBy = new Matriz(4, 1);
        GBx->set(0, 0, p1->getX());
        GBx->set(1, 0, p2->getX());
        GBx->set(2, 0, p3->getX());
        GBx->set(3, 0, p4->getX());
        GBy->set(0, 0, p1->getY());
        GBy->set(1, 0, p2->getY());
        GBy->set(2, 0, p3->getY());
        GBy->set(3, 0, p4->getY());

        for (float i = 0; i <= 1; i += passo) {
            T->set(0, 0, i * i * i);
            T->set(0, 1, i * i);
            T->set(0, 2, i);
            T->set(0, 3, 1);

            Matriz* TMB = T->multiply(MB);

            outPontos->adiciona(new Ponto(TMB->multiply(GBx)->get(0, 0), TMB->multiply(GBy)->get(0, 0)));
        }

    }

    CurvaBezier3D(float x, float y, float z, ListaEnc<Ponto*>* p) {
        ListaEnc<Ponto*>* temp = new ListaEnc<Ponto*>();
        MB->set(0, 0, -1);
        MB->set(0, 1, 3);
        MB->set(0, 2, -3);
        MB->set(0, 3, 1);
        MB->set(1, 0, 3);
        MB->set(1, 1, -6);
        MB->set(1, 2, 3);
        MB->set(2, 0, -3);
        MB->set(2, 1, 3);
        MB->set(3, 0, 1);

        //temp->adiciona(p->getHead());

        for (int i = 0; i < p->getSize(); i++) {
            p->get(i)->move_to(p->get(i)->getX() + x, p->get(i)->getY() + y, p->get(i)->getZ() + z);
        }

        if (p->getSize() < 4) {
            for (int i = 0; i < 4 - p->getSize(); i++) {
                p->adiciona(p->get(p->getSize() - 1));
            }
        }

        if (p->getSize() != 4 + (3 * (p->getSize() / 4))) {
            for (int i = p->getSize(); i < 4 + (3 * ((int) p->getSize() / 4)); i++) {
                p->adiciona(p->get(p->getSize() - 1));
            }
        }

        for (int i = 0; i < p->getSize() - 3; i += 3) {
            Bezier(p->get(i), p->get(i + 1), p->get(i + 2), p->get(i + 3), 0.05f, temp);
        }

        for (int i = 0; i < temp->getSize() - 1; i++) {
            Surface* s = new Surface(new Aresta(temp->get(i), temp->get(i + 1)));
            this->addTris(s);
        }

        this->setType(5);
        this->setLine(true);


    }

private:
    Matriz *MB = new Matriz(4, 4);
};

class B_Spline3D : public Shape3D {
public:

    void BSpline(int n, double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double z1, double z2, double z3, double z4, ListaEnc<Ponto*>* outPontos) {

        outPontos->adiciona(new Ponto(x1, y1, z1));

        for (int i = 0; i < n; i++) {
            x1 += x2;
            x2 += x3;
            x3 += x4;
            y1 += y2;
            y2 += y3;
            y3 += y4;
            z1 += z2;
            z2 += z3;
            z3 += z4;

            outPontos->adiciona(new Ponto(x1, y1, z1));

        }

    }

    B_Spline3D(float x, float y, float z, ListaEnc<Ponto*>* p) {
        ListaEnc<Ponto*>* temp = new ListaEnc<Ponto*>();
        float passo = 0.2f;

        MBS->set(0, 0, -1);
        MBS->set(0, 1, 3);
        MBS->set(0, 2, -3);
        MBS->set(0, 3, 1);
        MBS->set(1, 0, 3);
        MBS->set(1, 1, -6);
        MBS->set(1, 2, 3);
        MBS->set(2, 0, -3);
        MBS->set(2, 2, 3);
        MBS->set(3, 0, 1);
        MBS->set(3, 1, 4);
        MBS->set(3, 2, 1);

        MBS = MBS->multiply(1.0 / 6.0);

        Matriz* E = new Matriz(4, 4);
        E->set(0, 3, 1);
        E->set(1, 0, passo * passo * passo);
        E->set(1, 1, passo * passo);
        E->set(1, 2, passo);
        E->set(2, 0, 6 * (passo * passo * passo));
        E->set(2, 1, 2 * (passo * passo));
        E->set(3, 0, 6 * (passo * passo * passo));

        //temp->adiciona(p->getHead());

        for (int i = 0; i < p->getSize(); i++) {
            p->get(i)->move_to(p->get(i)->getX() + x, p->get(i)->getY() + y, p->get(i)->getZ() + z);
        }

        if (p->getSize() < 4) {
            for (int i = 0; i < 4 - p->getSize(); i++) {
                p->adiciona(p->get(p->getSize() - 1));
            }
        }

        for (int i = 3; i < p->getSize(); i++) {
            Matriz* GBSx = new Matriz(4, 1);
            Matriz* GBSy = new Matriz(4, 1);
            Matriz* GBSz = new Matriz(4, 1);
            GBSx->set(0, 0, p->get(i - 3)->getX());
            GBSx->set(1, 0, p->get(i - 2)->getX());
            GBSx->set(2, 0, p->get(i - 1)->getX());
            GBSx->set(3, 0, p->get(i)->getX());

            GBSy->set(0, 0, p->get(i - 3)->getY());
            GBSy->set(1, 0, p->get(i - 2)->getY());
            GBSy->set(2, 0, p->get(i - 1)->getY());
            GBSy->set(3, 0, p->get(i)->getY());

            GBSz->set(0, 0, p->get(i - 3)->getZ());
            GBSz->set(1, 0, p->get(i - 2)->getZ());
            GBSz->set(2, 0, p->get(i - 1)->getZ());
            GBSz->set(3, 0, p->get(i)->getZ());



            Matriz* Cx = MBS->multiply(GBSx);
            Matriz* Cy = MBS->multiply(GBSy);
            Matriz* Cz = MBS->multiply(GBSz);

            Cx = E->multiply(Cx);
            Cy = E->multiply(Cy);
            Cz = E->multiply(Cz);

            BSpline(1 / passo, Cx->get(0, 0), Cx->get(1, 0), Cx->get(2, 0), Cx->get(3, 0), Cy->get(0, 0), Cy->get(1, 0), Cy->get(2, 0), Cy->get(3, 0), Cz->get(0, 0), Cz->get(1, 0), Cz->get(2, 0), Cz->get(3, 0), temp);

        }

        for (int i = 0; i < temp->getSize() - 1; i++) {
            Surface* s = new Surface(new Aresta(temp->get(i), temp->get(i + 1)));
            this->addTris(s);
        }

        this->setType(5);
        this->setLine(true);
    }


private:
    Matriz* MBS = new Matriz(4, 4);



};

class SurfaceBezier : public Shape3D {
    Matriz* S = new Matriz(1, 4);
    Matriz* T = new Matriz(4, 1);
    Matriz* Mb = new Matriz(4, 4);
    float passo;

public:

    SurfaceBezier(float x, float y, float z, float passo, ListaEnc<Ponto*>* pontos, ListaEnc<Shape3D*>* gambiPoints) {
        this->passo = passo;

        Mb->set(0, 0, -1);
        Mb->set(0, 1, 3);
        Mb->set(0, 2, -3);
        Mb->set(0, 3, 1);
        Mb->set(1, 0, 3);
        Mb->set(1, 1, -6);
        Mb->set(1, 2, 3);
        Mb->set(2, 0, -3);
        Mb->set(2, 1, 3);
        Mb->set(3, 0, 1);


        if (pontos->getSize() == 16) {

            Matriz* Gbx = new Matriz(4, 4);
            Matriz* Gby = new Matriz(4, 4);
            Matriz* Gbz = new Matriz(4, 4);


            int k = 0;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    Gbx->set(i, j, pontos->get(k)->getX());
                    Gby->set(i, j, pontos->get(k)->getY());
                    Gbz->set(i, j, pontos->get(k)->getZ());
                    k++;
                }
            }

            ListaEnc<Ponto*>* temp = new ListaEnc<Ponto*>();
            Matriz* mx;
            Matriz* my;
            Matriz* mz;

            float s = 0;
            for (int i = 0; i < 1 / passo; i++) {

                float t = 0;

                S->set(0, 0, s * s * s);
                S->set(0, 1, s * s);
                S->set(0, 2, s);
                S->set(0, 3, 1);

                for (int j = 0; j < 1 / passo; j++) {

                    T->set(0, 0, t * t * t);
                    T->set(1, 0, t * t);
                    T->set(2, 0, t);
                    T->set(3, 0, 1);

                    mx = S->multiply(Mb);
                    mx = mx->multiply(Gbx);
                    mx = mx->multiply(Mb);
                    mx = mx->multiply(T);

                    my = S->multiply(Mb);
                    my = my->multiply(Gby);
                    my = my->multiply(Mb);
                    my = my->multiply(T);

                    mz = S->multiply(Mb);
                    mz = mz->multiply(Gbz);
                    mz = mz->multiply(Mb);
                    mz = mz->multiply(T);

                    temp->adiciona(new Ponto(mx->get(0, 0), my->get(0, 0), mz->get(0, 0)));

                    t += passo;
                }
                s += passo;
            }

            ListaEnc<Ponto*>* l;


            for (int i = 0; i < 1 / (passo * passo); i += 1 / passo) {
                for (int j = 0; j < (1 / passo) - 1; j++) {
                    Surface* s = new Surface(new Aresta(temp->get(i + j), temp->get(i + j + 1)));
                    this->addTris(s);
                }
            }

            for (int j = 0; j < (1 / passo); j++) {
                for (int i = 0; i < 1 / (passo * passo) - (1 / passo); i += 1 / passo) {
                    Surface* s = new Surface(new Aresta(temp->get(i + j), temp->get(i + j + (1 / passo))));
                    this->addTris(s);
                }
            }

            this->setType(7);
            this->setLine(true);

        }
    }
};

class BSplineSurface : public Shape3D {
    Matriz* Mb = new Matriz(4, 4);
    Matriz* Mt = new Matriz(4, 4);
    Matriz* Es = new Matriz(4, 4);
    Matriz* Et = new Matriz(4, 4);

    Matriz* DDX1;
    Matriz* DDY1;
    Matriz* DDZ1;
    Matriz* DDX2;
    Matriz* DDY2;
    Matriz* DDZ2;

private:

    void BSpline(int n, double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double z1, double z2, double z3, double z4, ListaEnc<Ponto*>* outPontos) {

        outPontos->adiciona(new Ponto(x1, y1, z1));

        for (int i = 0; i < n; i++) {
            x1 += x2;
            x2 += x3;
            x3 += x4;
            y1 += y2;
            y2 += y3;
            y3 += y4;
            z1 += z2;
            z2 += z3;
            z3 += z4;

            outPontos->adiciona(new Ponto(x1, y1, z1));

        }
    }

public:

    BSplineSurface(ListaEnc<Ponto*>* pontos, int lin, int col, float passo) {
        Es->set(0, 3, 1);
        Es->set(1, 0, passo * passo * passo);
        Es->set(1, 1, passo * passo);
        Es->set(1, 2, passo);
        Es->set(2, 0, 6 * passo * passo * passo);
        Es->set(2, 1, 2 * passo * passo);
        Es->set(3, 0, 6 * passo * passo * passo);

        Et->set(3, 0, 1);
        Et->set(0, 1, passo * passo * passo);
        Et->set(1, 1, passo * passo);
        Et->set(2, 1, passo);
        Et->set(0, 2, 6 * passo * passo * passo);
        Et->set(1, 2, 2 * passo * passo);
        Et->set(0, 3, 6 * passo * passo * passo);

        Mb->set(0, 0, -1);
        Mb->set(0, 1, 3);
        Mb->set(0, 2, -3);
        Mb->set(0, 3, 1);
        Mb->set(1, 0, 3);
        Mb->set(1, 1, -6);
        Mb->set(1, 2, 3);
        Mb->set(2, 0, -3);
        Mb->set(2, 2, 3);
        Mb->set(3, 2, 1);
        Mb->set(3, 1, 4);
        Mb->set(3, 0, 1);

        Mt->set(0, 0, -1);
        Mt->set(1, 0, 3);
        Mt->set(2, 0, -3);
        Mt->set(3, 0, 1);
        Mt->set(0, 1, 3);
        Mt->set(1, 1, -6);
        Mt->set(2, 1, 3);
        Mt->set(0, 2, -3);
        Mt->set(2, 2, 3);
        Mt->set(2, 3, 1);
        Mt->set(1, 3, 4);
        Mt->set(0, 3, 1);

        Mb = Mb->multiply(1.0 / 6.0);
        Mt = Mt->multiply(1.0 / 6.0);

        Matriz* Gx = new Matriz(4, 4);
        Matriz* Gy = new Matriz(4, 4);
        Matriz* Gz = new Matriz(4, 4);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                Ponto* p = pontos->get(i * lin + j);
                Gx->set(i, j, p->getX());
                Gy->set(i, j, p->getY());
                Gz->set(i, j, p->getZ());
            }
        }
        BSplineSurface3D(Gx, Gy, Gz, passo);



    }

private:

    void BSplineSurface3D(Matriz* Gx, Matriz* Gy, Matriz* Gz, float passo) {

        Matriz* Cx = Mb->multiply(Gx);
        Cx = Cx->multiply(Mt);
        Matriz* Cy = Mb->multiply(Gy);
        Cy = Cy->multiply(Mt);
        Matriz* Cz = Mb->multiply(Gz);
        Cz = Cz->multiply(Mt);

        DDX1 = Es->multiply(Cx);
        DDX1 = DDX1->multiply(Et);
        DDY1 = Es->multiply(Cy);
        DDY1 = DDY1->multiply(Et);
        DDZ1 = Es->multiply(Cz);
        DDZ1 = DDZ1->multiply(Et);

        DDX2 = DDX1->copy();
        DDY2 = DDY1->copy();
        DDZ2 = DDZ1->copy();

        ListaEnc<Ponto*>* temp = new ListaEnc<Ponto*>();
        ListaEnc<Ponto*>* temp1 = new ListaEnc<Ponto*>();

        for (int i = 0; i < 1 / passo; i++) {
            BSpline(1 / passo, DDX1->get(0, 0), DDX1->get(0, 1), DDX1->get(0, 2), DDX1->get(0, 3), DDY1->get(0, 0), DDY1->get(0, 1), DDY1->get(0, 2), DDY1->get(0, 3),
                    DDZ1->get(0, 0), DDZ1->get(0, 1), DDZ1->get(0, 2), DDZ1->get(0, 3), temp);

            DDX1->set(0, 0, DDX1->get(1, 0) + DDX1->get(0, 0));
            DDX1->set(0, 1, DDX1->get(1, 1) + DDX1->get(0, 1));
            DDX1->set(0, 2, DDX1->get(1, 2) + DDX1->get(0, 2));
            DDX1->set(0, 3, DDX1->get(1, 3) + DDX1->get(0, 3));
            DDX1->set(1, 0, DDX1->get(2, 0) + DDX1->get(1, 0));
            DDX1->set(1, 1, DDX1->get(2, 1) + DDX1->get(1, 1));
            DDX1->set(1, 2, DDX1->get(2, 2) + DDX1->get(1, 2));
            DDX1->set(1, 3, DDX1->get(2, 3) + DDX1->get(1, 3));
            DDX1->set(2, 0, DDX1->get(3, 0) + DDX1->get(2, 0));
            DDX1->set(2, 1, DDX1->get(3, 1) + DDX1->get(2, 1));
            DDX1->set(2, 2, DDX1->get(3, 2) + DDX1->get(2, 2));
            DDX1->set(2, 3, DDX1->get(3, 3) + DDX1->get(2, 3));

            DDY1->set(0, 0, DDY1->get(1, 0) + DDY1->get(0, 0));
            DDY1->set(0, 1, DDY1->get(1, 1) + DDY1->get(0, 1));
            DDY1->set(0, 2, DDY1->get(1, 2) + DDY1->get(0, 2));
            DDY1->set(0, 3, DDY1->get(1, 3) + DDY1->get(0, 3));
            DDY1->set(1, 0, DDY1->get(2, 0) + DDY1->get(1, 0));
            DDY1->set(1, 1, DDY1->get(2, 1) + DDY1->get(1, 1));
            DDY1->set(1, 2, DDY1->get(2, 2) + DDY1->get(1, 2));
            DDY1->set(1, 3, DDY1->get(2, 3) + DDY1->get(1, 3));
            DDY1->set(2, 0, DDY1->get(3, 0) + DDY1->get(2, 0));
            DDY1->set(2, 1, DDY1->get(3, 1) + DDY1->get(2, 1));
            DDY1->set(2, 2, DDY1->get(3, 2) + DDY1->get(2, 2));
            DDY1->set(2, 3, DDY1->get(3, 3) + DDY1->get(2, 3));

            DDZ1->set(0, 0, DDZ1->get(1, 0) + DDZ1->get(0, 0));
            DDZ1->set(0, 1, DDZ1->get(1, 1) + DDZ1->get(0, 1));
            DDZ1->set(0, 2, DDZ1->get(1, 2) + DDZ1->get(0, 2));
            DDZ1->set(0, 3, DDZ1->get(1, 3) + DDZ1->get(0, 3));
            DDZ1->set(1, 0, DDZ1->get(2, 0) + DDZ1->get(1, 0));
            DDZ1->set(1, 1, DDZ1->get(2, 1) + DDZ1->get(1, 1));
            DDZ1->set(1, 2, DDZ1->get(2, 2) + DDZ1->get(1, 2));
            DDZ1->set(1, 3, DDZ1->get(2, 3) + DDZ1->get(1, 3));
            DDZ1->set(2, 0, DDZ1->get(3, 0) + DDZ1->get(2, 0));
            DDZ1->set(2, 1, DDZ1->get(3, 1) + DDZ1->get(2, 1));
            DDZ1->set(2, 2, DDZ1->get(3, 2) + DDZ1->get(2, 2));
            DDZ1->set(2, 3, DDZ1->get(3, 3) + DDZ1->get(2, 3));

        }

        DDX1 = DDX2->transpose();
        DDY1 = DDY2->transpose();
        DDZ1 = DDZ2->transpose();


        for (int i = 0; i < 1 / passo; i++) {
            BSpline(1 / passo, DDX1->get(0, 0), DDX1->get(0, 1), DDX1->get(0, 2), DDX1->get(0, 3), DDY1->get(0, 0), DDY1->get(0, 1), DDY1->get(0, 2), DDY1->get(0, 3),
                    DDZ1->get(0, 0), DDZ1->get(0, 1), DDZ1->get(0, 2), DDZ1->get(0, 3), temp1);


            DDX1->set(0, 0, DDX1->get(1, 0) + DDX1->get(0, 0));
            DDX1->set(0, 1, DDX1->get(1, 1) + DDX1->get(0, 1));
            DDX1->set(0, 2, DDX1->get(1, 2) + DDX1->get(0, 2));
            DDX1->set(0, 3, DDX1->get(1, 3) + DDX1->get(0, 3));
            DDX1->set(1, 0, DDX1->get(2, 0) + DDX1->get(1, 0));
            DDX1->set(1, 1, DDX1->get(2, 1) + DDX1->get(1, 1));
            DDX1->set(1, 2, DDX1->get(2, 2) + DDX1->get(1, 2));
            DDX1->set(1, 3, DDX1->get(2, 3) + DDX1->get(1, 3));
            DDX1->set(2, 0, DDX1->get(3, 0) + DDX1->get(2, 0));
            DDX1->set(2, 1, DDX1->get(3, 1) + DDX1->get(2, 1));
            DDX1->set(2, 2, DDX1->get(3, 2) + DDX1->get(2, 2));
            DDX1->set(2, 3, DDX1->get(3, 3) + DDX1->get(2, 3));

            DDY1->set(0, 0, DDY1->get(1, 0) + DDY1->get(0, 0));
            DDY1->set(0, 1, DDY1->get(1, 1) + DDY1->get(0, 1));
            DDY1->set(0, 2, DDY1->get(1, 2) + DDY1->get(0, 2));
            DDY1->set(0, 3, DDY1->get(1, 3) + DDY1->get(0, 3));
            DDY1->set(1, 0, DDY1->get(2, 0) + DDY1->get(1, 0));
            DDY1->set(1, 1, DDY1->get(2, 1) + DDY1->get(1, 1));
            DDY1->set(1, 2, DDY1->get(2, 2) + DDY1->get(1, 2));
            DDY1->set(1, 3, DDY1->get(2, 3) + DDY1->get(1, 3));
            DDY1->set(2, 0, DDY1->get(3, 0) + DDY1->get(2, 0));
            DDY1->set(2, 1, DDY1->get(3, 1) + DDY1->get(2, 1));
            DDY1->set(2, 2, DDY1->get(3, 2) + DDY1->get(2, 2));
            DDY1->set(2, 3, DDY1->get(3, 3) + DDY1->get(2, 3));

            DDZ1->set(0, 0, DDZ1->get(1, 0) + DDZ1->get(0, 0));
            DDZ1->set(0, 1, DDZ1->get(1, 1) + DDZ1->get(0, 1));
            DDZ1->set(0, 2, DDZ1->get(1, 2) + DDZ1->get(0, 2));
            DDZ1->set(0, 3, DDZ1->get(1, 3) + DDZ1->get(0, 3));
            DDZ1->set(1, 0, DDZ1->get(2, 0) + DDZ1->get(1, 0));
            DDZ1->set(1, 1, DDZ1->get(2, 1) + DDZ1->get(1, 1));
            DDZ1->set(1, 2, DDZ1->get(2, 2) + DDZ1->get(1, 2));
            DDZ1->set(1, 3, DDZ1->get(2, 3) + DDZ1->get(1, 3));
            DDZ1->set(2, 0, DDZ1->get(3, 0) + DDZ1->get(2, 0));
            DDZ1->set(2, 1, DDZ1->get(3, 1) + DDZ1->get(2, 1));
            DDZ1->set(2, 2, DDZ1->get(3, 2) + DDZ1->get(2, 2));
            DDZ1->set(2, 3, DDZ1->get(3, 3) + DDZ1->get(2, 3));
        }



        for (int i = 0; i < (1 / (passo * passo)); i += (1 / passo) + 1) {
            for (int j = 0; j < (1 / passo) - 1; j++) {
                Surface* s = new Surface(new Aresta(temp->get(i + j), temp->get(i + j + 1)));
                this->addTris(s);
            }
        }

        for (int i = 0; i < (1 / (passo * passo)); i += (1 / passo) + 1) {
            for (int j = 0; j < (1 / passo) - 1; j++) {
                Surface* s = new Surface(new Aresta(temp1->get(i + j), temp1->get(i + j + 1)));
                this->addTris(s);
            }
        }

        this->setType(8);
        this->setLine(true);

    }



};

#endif /* SHAPE_HPP */

