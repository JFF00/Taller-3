#include <iostream>
#include "nodoServer.h"

nodoServer::nodoServer(string idu,string ids,string type){//idu:id user -- ids:id server
    id=idu;
    nombreserver=ids;
    tipo=type;
}
void nodoServer::addServer(nodoServer*ns, int dist){ //añade el servidor al cual se conecta y la distancia entre ambos, dentro del vector se almacena como un par.
    servers.emplace_back(ns,dist);
}
void nodoServer::addVelocidad(int vel){
    velocidad.push_back(vel);
}
vector<pair<nodoServer*,int>> nodoServer::getServers(){
    return servers;
}
int nodoServer::getVelocidad(string id){
    for(int i=0;i<servers.size();i++){
        if(servers[i].first->getId()==id){
                return velocidad[i];
        }
    }
    return 0;
}




