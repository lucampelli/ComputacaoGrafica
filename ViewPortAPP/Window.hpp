/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Window.hpp
 * Author: lucampelli
 *
 * Created on 17 de Março de 2017, 15:23
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Ponto.hpp"


class Window{
private:
    float width;
    float height;
    float depth;
    Ponto* ogmin;
    Ponto* ogmax;
    Ponto* windowmin;
    Ponto* windowmax;
    
    Window() : width(600), height(600) {
        windowmin = new Ponto();
        ogmin = windowmin;
        windowmax = new Ponto(600, 600);
        ogmax = windowmax;
    }

    Window(float width, float height, float depth = 1) : width(width), height(height), depth(depth) {
        windowmin = new Ponto();
        ogmin = windowmin;
        windowmax = new Ponto(width, height,depth);
        ogmax = windowmax;
    }

public:
    
    static Window* getInstance(){
        static Window* p;
        if(!p){
            p = new Window();
        }
        return p;
    }
    
    static Window* getInstance(float w, float h, float d = 1){
        static Window* p;
        if(!p){
            p = new Window(w,h,d);
        }
        return p;
    }

    Ponto* min() {
        return windowmin;
    }

    Ponto* max() {
        return windowmax;
    }
    
    void moveWindowToPoint(Ponto* p){
        windowmin = p;
        windowmax = new Ponto(p->getX() + width, p->getY() + height, p->getZ());
        std::cout<<"Window: "<<windowmax->getX()<<" : "<<windowmax->getY()<<std::endl;
    }
};
#endif /* WINDOW_HPP */

