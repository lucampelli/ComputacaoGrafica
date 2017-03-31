/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ListaEnc.hpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
 *
 * Created on 8 de Março de 2017, 14:04
 */

#ifndef LISTAENC_HPP
#define LISTAENC_HPP

#include "Elemento.hpp"
#include <iostream>

using namespace std;

template<typename T>
class ListaEnc {
public:

    ListaEnc() : head(NULL), size(0) {
    }

    ~ListaEnc();

    // inicio

    void adicionaNoInicio(const T& dado) {

        if (listaVazia()) {
            head = new Elemento<T>(dado, NULL);
        } else {

            Elemento<T> * novo = new Elemento<T>(dado, head);
            head = novo;
            
        }
        
        size++;
        return;
        
    }

    T retiraDoInicio(){
        if(listaVazia()){
            cout<<"Lista Vazia"<<endl;
            return  NULL;
        }
        
        Elemento<T> * retirar = head;
        head = head->getProximo();
        T info = retirar->getInfo();
        size--;
        delete retirar;
        cout<<size<<endl;
        return info;

    }

    void eliminaDoInicio(){
        Elemento<T> * eliminar = head;
        head = head->getProximo();
        delete eliminar;
    }

    // posicao
    void adicionaNaPosicao(const T& dado, int pos){
        if(pos > size){
            cout<<"invalid index"<<endl;
            return;
        }
        
        if(pos == 0){
            adicionaNoInicio(dado);
            return;
        }
        
        Elemento<T> * temp = head;
        
        for (int i = 1; i < pos; i++){
            temp = temp->getProximo();
        }
        
        Elemento<T> * novo = new Elemento<T>(dado, temp->getProximo());
        temp->setProximo(novo);
        size++;
        
    }
    
    void clean(){
        
        int s = size;
        
        for(int i = 0; i < s; i++){
            retiraDoInicio();
        }
    }

    int posicao(const T& dado) const;

    T* posicaoMem(const T& dado) const;

    bool contem(const T& dado);

    T retiraDaPosicao(int pos) {
        if(pos >= size){
            cout<<"invalid index: "<<pos<<endl;
            return NULL;
        }
        
        if(pos == 0){
            return retiraDoInicio();
        }
        
        Elemento<T> * temp = head;
        Elemento<T> * remover = head;
        
        for (int i = 1; i < pos; i++){
            temp = temp->getProximo();
        }
        
        remover = temp->getProximo();
        temp->setProximo(remover->getProximo());
        
        T info = remover->getInfo();
        delete remover;
        size--;
        return info;
    }
    
    void modifica(int pos, const T& dado){
        retiraDaPosicao(pos);
        adicionaNaPosicao(dado, pos);
    }

    //fim
    void adiciona(const T& dado){
        adicionaNaPosicao(dado,size);
    }

    T retira();

    // especifico
    T retiraEspecifico(const T& dado);

    void adicionaEmOrdem(const T& data);

    bool listaVazia() const{
        if(size == 0){
            return true;
        }
        return false;
    }

    bool igual(T dado1, T dado2){
        return dado1 == dado2;
    }

    bool maior(T dado1, T dado2){
        return dado1 > dado2;
    }

    bool menor(T dado1, T dado2){
        return dado1 < dado2;
    }

    void destroiLista();

    int getSize(){
        return size;
    }
    
    T getHead(){
        return head->getInfo();
    }
    
    T get(int pos){
        if(pos >= size){
            cout<<"invalid index"<<endl;
            return 0;
        }
        
        if(pos == 0){
            return getHead();
        }
        
        Elemento<T> * temp = head;
        
        for (int i = 0; i < pos; i++){
            temp = temp->getProximo();
        }
        return temp->getInfo();
    }
    
private:
    Elemento<T>* head;
    int size;
};
#endif /* LISTAENC_HPP */

