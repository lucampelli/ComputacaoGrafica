/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ponto.hpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
 *
 * Created on 9 de Março de 2017, 16:58
 */

#ifndef PONTO_HPP
#define PONTO_HPP

class Ponto{
    
private:
    float x,y;

public:
    Ponto() : x(0),y(0){}
    
    Ponto(float nx, float ny) : x(nx), y(ny){}
    
    float getX(){
        return x;
    }
    
    float getY(){
        return y;
    }
    
    void move_to(float nx, float ny){
        x = nx;
        y = ny;
    }
};

#endif /* PONTO_HPP */

