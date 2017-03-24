/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Viewport.hpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
 *
 * Created on 17 de Março de 2017, 15:23
 */

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "Ponto.hpp"

class Viewport {
private:
    float width;
    float height;
    Ponto* ogmin;
    Ponto* ogmax;
    Ponto* viewportmin;
    Ponto* viewportmax;

    Viewport() : width(600), height(600) {
        viewportmin = new Ponto();
        ogmin = new Ponto();
        viewportmax = new Ponto(600, 600);
        ogmax = new Ponto(600, 600);;
    }

    Viewport(float width, float height) : width(width), height(height) {
        viewportmin = new Ponto();
        ogmin = new Ponto();
        viewportmax = new Ponto(width, height);
        ogmax = new Ponto(width, height);
    }

public:
    
    static Viewport* getInstance(){
        static Viewport* p;
        if(!p){
            p = new Viewport();
        }
        return p;
    }
    
    static Viewport* getInstance(float w, float h){
        static Viewport* p;
        if(!p){
            p = new Viewport(w,h);
        }
        return p;
    }

    Ponto* min() {
        return viewportmin;
    }

    Ponto* max() {
        return viewportmax;
    }

    void scaleViewPort(float scaleValue) {
        float medx = (ogmax->getX() + ogmin->getX()) / 2;
        float medy = (ogmax->getY() + ogmin->getY()) / 2;
        float zx = medx * scaleValue;
        float zy = medy * scaleValue;
        viewportmin->move_to(ogmin->getX() - zx + medx , ogmin->getY() - zy + medy);
        viewportmax->move_to(ogmax->getX() + zx - medx, ogmax->getY() + zy - medy);
        std::cout<<"ScaleViewPortMin:" << ogmin->getX()<<" : "<< ogmin->getY()<<std::endl;
        std::cout<<"ScaleViewPortMax:" << viewportmin->getX()<<" : "<< viewportmin->getY()<<std::endl;
    }

};
#endif /* VIEWPORT_HPP */

