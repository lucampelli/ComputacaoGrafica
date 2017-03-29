/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SCN.hpp
 * Author: luca
 *
 * Created on 29 de Março de 2017, 09:31
 */

#ifndef SCN_HPP
#define SCN_HPP

#include "ListaEnc.hpp"
#include "Window.hpp"


class SCN{
    
private:
    Ponto* Smin = new Ponto(-1,-1);
    Ponto* Smax = new Ponto(1,1);
    ListaEnc<Shape*>* normShapes = new ListaEnc<Shape*>();
    Window* window = Window::getInstance();
    
    
public:
    
    static SCN* getInstance(){
        static SCN* scn;
        if(!scn){
            scn = new SCN();
        }
        return scn;
    }
    
    void addShape(Shape* s){
        normShapes->adiciona(s);
    }
    
    void remShape(int index){
        normShapes->retiraDaPosicao(index);
    }
    
    
    
};

#endif /* SCN_HPP */

