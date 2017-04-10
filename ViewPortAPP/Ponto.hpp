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

class RegionCode{
private:
    bool RC[4] = {false, false, false, false};
    
public:
    RegionCode(){}
    
    RegionCode(bool b0, bool b1, bool b2, bool b3){
        RC[0] = b0;
        RC[1] = b1;
        RC[2] = b2;
        RC[3] = b3;
    }
    
    bool get(int index){
        return RC[index];
    }
    
    void set(bool bit, int index){
        RC[index] = bit;
    }
    
    void set(RegionCode nc){
        RC[0] = nc[0];
        RC[1] = nc[1];
        RC[2] = nc[2];
        RC[3] = nc[3];
    }
    
    bool operator [](int x){
        return get(x);
    }
    
    bool operator ==(RegionCode rc){
        return (RC[0] == rc[0]) && (RC[1] == rc[1]) && (RC[2] == rc[2]) && (RC[3] == rc[3]);
    }
    
    void operator = (RegionCode nc){
        RC[0] = nc[0];
        RC[1] = nc[1];
        RC[2] = nc[2];
        RC[3] = nc[3];
    }
    
    
    bool operator != (RegionCode rc){
        return (RC[0] != rc[0]) || (RC[1] != rc[1]) || (RC[2] != rc[2]) || (RC[3] != rc[3]);
    }
    
    RegionCode operator && (RegionCode rc){
        RegionCode temp;
        temp.set(RC[0] && rc[0], 0);
        temp.set(RC[1] && rc[1], 1);
        temp.set(RC[2] && rc[2], 2);
        temp.set(RC[3] && rc[3], 3);
        return temp;
    }
    
};

class Ponto{
    
private:
    double x,y,z,w;
    RegionCode RC = RegionCode();

public:
    Ponto() : x(0),y(0),z(0),w(0){}
    
    Ponto(double nx, double ny) : x(nx), y(ny),z(1),w(1){}
    Ponto(double nx, double ny,double nz) : x(nx), y(ny),z(nz),w(1){}
    
    double getX(){
        return x;
    }
    
    double getY(){
        return y;
    }
    
    double getZ(){
        return z;
    }
    
    double getW(){
        return w;
    }
    
    RegionCode getRC(){
        return RC;
    }
    
    double setRC(RegionCode rc){
        RC = rc;
    }
    
    void move_to(double nx, double ny){
        x = nx;
        y = ny;
    }
    
    void move_to(double nx, double ny, double nz){
        x = nx;
        y = ny;
        z = nz;
    }
    
    void move_by(double nx, double ny){
        x += nx;
        y += ny;
    }
    
    void move_by(double nx, double ny, double nz){
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

