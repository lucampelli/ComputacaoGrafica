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
    float x,y,z,w;
    float RC[4];

public:
    Ponto() : x(0),y(0),z(0),w(0){}
    
    Ponto(float nx, float ny) : x(nx), y(ny),z(1),w(1){}
    Ponto(float nx, float ny,float nz) : x(nx), y(ny),z(nz),w(1){}
    
    float getX(){
        return x;
    }
    
    float getY(){
        return y;
    }
    
    float getZ(){
        return z;
    }
    
    float getW(){
        return w;
    }
    
    float* getRC(){
        return RC;
    }
    
    float setRC(float rc[4]){
        RC = rc;
    }
    
    void move_to(float nx, float ny){
        x = nx;
        y = ny;
    }
    
    void move_to(float nx, float ny, float nz){
        x = nx;
        y = ny;
        z = nz;
    }
    
    void move_by(float nx, float ny){
        x += nx;
        y += ny;
    }
    
    void move_by(float nx, float ny, float nz){
        x += nx;
        y += ny;
        z += nz;
    }
    
    Ponto* sum(Ponto* p){
        return new Ponto(this->getX() + p->getX(),this->getY() + p->getY(),this->getZ() + p->getZ());
    }
    
    Ponto* mult(Ponto* p){
        return new Ponto(this->getX() * p->getX(),this->getY() * p->getY(),this->getZ() * p->getZ());
    }

};

#endif /* PONTO_HPP */

