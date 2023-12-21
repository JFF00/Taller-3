#include <string>
#include <iostream>
#include <vector>
#include <climits>
#pragma once

using namespace std;

class nodoServer{
    public:
        vector<pair<nodoServer*,int>>servers; 
        nodoServer(string id,string nombreserver,string tipo);
        string getName(){return nombreserver;};
        string getId(){return id;};
        string getType(){return tipo;};
        void addServer(nodoServer*ns,int distancia);
        void addVelocidad(int vel);
        vector<pair<nodoServer*,int>> getServers();
        int getVelocidad(string id);
        //void bellmanFord2(nodoServer* origen);
    private:
        string id;
        string nombreserver;
        string tipo;
        vector<int> velocidad;
};