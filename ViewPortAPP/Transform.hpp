/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Transform.hpp
 * Author: lucampelli
 *
 * Created on 17 de Março de 2017, 15:53
 */

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Ponto.hpp"


class Transform{
    
private:
    float m11,m13,m22,m23;
    
    Transform(){
        
    }
    
public:    
    static Transform* getInstance(){
        static Transform* t;
        if(!t){
            t = new Transform();
        }
        return t;
    }
    
    void calculate_matrix(Ponto* vpmin, Ponto* vpmax, Ponto* wmin, Ponto* wmax){
        m11 = (vpmax->getX() - vpmin->getX()) / (wmax->getX() - wmin->getX());
        m22 = (vpmax->getY() - vpmin->getY()) / (wmax->getY() - wmin->getY());
        m13 = -wmin->getX() * (m11) + vpmin->getX();
        m23 = -wmin->getY() * (m22) + vpmin->getY();
        std::cout<<"Transform: "<<m11<<" : "<<m13<<" : "<< m22 << " : " << m23 << std::endl;
    }
    
    Ponto* T(Ponto* p){
        return new Ponto(p->getX() * m11 + m13 , p->getY() * m22 + m23 );
    }
    
    Ponto* iT(Ponto* p){
        return new Ponto((p->getX()- m13 ) / m11, (p->getY() - m23 ) / m22 );
    }
    
    
};

#endif /* TRANSFORM_HPP */

