/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Matriz.hpp
 * Author: lucampelli
 *
 * Created on 17 de Março de 2017, 16:17
 */

#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include "ListaEnc.hpp"

class Matriz {
private:
    int l, c;
    ListaEnc<ListaEnc<float>*>* matriz;

public:

    Matriz(int l, int c) : l(l), c(c) {
        matriz = new ListaEnc<ListaEnc<float>*>();
        for (int i = 0; i < l; i++) {
            matriz->adiciona(new ListaEnc<float>());
            for (int j = 0; j < c; j++) {
                matriz->get(i)->adiciona(0);
            }
        }
    }

    int col() {
        return c;
    }

    int lin() {
        return l;
    }

    float get(int l, int c) {
        return matriz->get(l)->get(c);
    }

    void set(int l, int c, float dado) {
        matriz->get(l)->modifica(c, dado);
    }

    Matriz* operator*(Matriz* m) {
        Matriz* r = new Matriz(l, m->col());
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < m->col(); j++) {
                float a = 0;
                for (int k = 0; k < m->l; k++) {
                    for (int n = 0; n < m->col(); n++) {
                        a += get(k,n) * m->get(n,k);
                    }
                }
                r->set(i,j,a);
            }
        }
        return r;
    }
};

#endif /* MATRIZ_HPP */

