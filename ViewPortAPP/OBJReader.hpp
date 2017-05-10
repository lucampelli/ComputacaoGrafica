/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OBJReader.hpp
 * Author: luca
 *
 * Created on May 8, 2017, 1:57 PM
 */

#ifndef OBJREADER_HPP
#define OBJREADER_HPP

class OBJReader {
private:

    OBJReader() {

    }

public:

    static OBJReader* getInstance(){
        static OBJReader* obj;
        if(!obj){
            obj = new OBJReader();
        }
        return obj;
    }
    
    
    

};
#endif /* OBJREADER_HPP */

