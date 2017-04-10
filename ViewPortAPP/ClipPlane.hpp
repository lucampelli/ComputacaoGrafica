/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClipPlane.hpp
 * Author: luca
 *
 * Created on 6 de Abril de 2017, 08:49
 */

#ifndef CLIPPLANE_HPP
#define CLIPPLANE_HPP

#include "Ponto.hpp"

class ClipPlane{
    Ponto* clipMin = new Ponto(-0.9f,-0.9f);
    Ponto* clipMax = new Ponto( 0.9f, 0.9f);
    
public:
    
    static ClipPlane* getInstance(){
        static ClipPlane* clip;
        if(!clip){
            clip = new ClipPlane();
        }
        return clip;
    }
    
    Ponto* getMin(){
        return clipMin;
    }
    
    Ponto* getMax(){
        return clipMax;
    }
    
    void move(double x, double y){
        clipMin->move_by(x,y);
        clipMax->move_by(x,y);
    }
    
};

#endif /* CLIPPLANE_HPP */

