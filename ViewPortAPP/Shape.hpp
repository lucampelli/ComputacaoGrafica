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

#include "Ponto.hpp"
#include "ListaEnc.hpp"
#include "gtk/gtk.h"
#include "Camera3D.hpp"
#include <iostream>
#ifndef SHAPE_HPP
#define SHAPE_HPP

using namespace std;

class Shape {
private:
    Camera* cam = Camera::getInstance();;
    Ponto* pos;
    ListaEnc<Ponto*>* pontos;
    int vertices;
    string name;
    
public:

    Shape() : vertices(0) {
        pos = new Ponto();
        pontos = new ListaEnc<Ponto*>();
        
    }

    Shape(float x, float y) : vertices(0) {
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

    Shape(float x, float y, ListaEnc<Ponto*>* points) {
        pos = new Ponto(x, y);
        pontos = points;
        vertices = points->getSize();
    
    }

    void setName(string n){
        name = n;
    }
    
    string getName(){
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
    
    void setPointsList(ListaEnc<Ponto*>* p){
        this->pontos = p;
        this->vertices = p->getSize();
    }

    ListaEnc<Ponto*>* getPontos() {
        return pontos;
    }

    void draw(cairo_t* cr) {
        Ponto* camPos = cam->getPos();
        
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 0.5);
        //do_transform;
        if (vertices > 1) {
            Ponto* atual = pontos->get(0);
            atual = cam->drawTransform(atual);
            cairo_move_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());
            
            for (int i = 1; i < vertices; i++) {
                Ponto* atual = pontos->get(i);
                atual = cam->drawTransform(atual);
                cairo_line_to(cr,atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY()); //good
                cairo_move_to(cr, atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());
            }
            atual = pontos->get(0);
            atual = cam->drawTransform(atual);
            cairo_line_to(cr,atual->getX() + pos->getX() + camPos->getX(), atual->getY() + pos->getY() + camPos->getY());
            
            cairo_stroke(cr);
        }

    }

    void printPontos() {
        cout << pontos->getSize() << endl;
        for (int i = 0; i < pontos->getSize(); i++) {
            cout << pontos->get(i)->getX() << "," << pontos->get(i)->getY() << endl;
        }
    }

};

class Retangulo : public Shape{
public:
    Retangulo(float x, float y, float width, float height){
        Ponto* p[] = {new Ponto(x,y), new Ponto(x+width,y),new Ponto(x+width,y+height),new Ponto(x,y+height)};
        this->addPoints(4,p);
    }
};

class Quadrado : public Retangulo{
public:
    Quadrado(float x, float y, float size) : Retangulo(x,y,size,size){}
};

class Poligono : public Shape{
public:
    Poligono(float x, float y,ListaEnc<Ponto*>* p){
        for(int i = 0; i < p->getSize();i++){
            p->get(i)->move_to(p->get(i)->getX() + x, p->get(i)->getY() + y);
        }
        this->setPointsList(p);
    }
    
    
};

#endif /* SHAPE_HPP */

