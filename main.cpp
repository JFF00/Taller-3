#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "nodoServer.cpp"
using namespace std;

void leeServer(vector<nodoServer*>&servers){
   ifstream archivo("C:/Users/JF/Desktop/xd/xd/servidores.csv",ios::in);
   if(!archivo){
      cout<<"Error al abrir archivo"<<endl;
   }
   else{
      string linea;
      getline(archivo,linea);
      while(getline(archivo,linea)){
         vector<string>fila;
         stringstream sep(linea);
         string token;
         while(getline(sep,token,',')){
            fila.push_back(token);
         }
         nodoServer* server=new nodoServer(fila[0],fila[1],fila[2]);
         servers.push_back(server);
      }
   }

}
void leeConexiones(vector<nodoServer*>&servers){
   ifstream archivo("C:/Users/JF/Desktop/xd/xd/conexiones.csv");
   if(!archivo){
      cout<<"Error al abrir"<<endl;
      }
   else{
      string linea;
      getline(archivo,linea);
      while(getline(archivo,linea)){
         vector<string>fila;
         stringstream sep(linea);
         string token;
         while(getline(sep,token,',')){
            fila.push_back(token);
         }
         int speed;
         speed=stoi(fila[2]);
         int dist;
         dist=stoi(fila[3]);
         for(int i=0;i<servers.size();i++){ //busca el server dentro de la lista de servidores
            if(fila[0]==servers[i]->getId()){
               for(int j=0;j<servers.size();j++){
                  if(fila[1]==servers[j]->getId()){
                  servers[i]->addServer(servers[j],dist);
                  servers[i]->addVelocidad(speed);
                  servers[j]->addServer(servers[i],dist);
                  servers[j]->addVelocidad(speed);
               }}
                  
            }
         }
      }
   }
}
int getIdServer(string id,vector<nodoServer*>servers){ //funcion que retorna el indice de un server dentro de una lista de servers.
   int i;
   for(int i=0;i<servers.size();i++){
      if(stoi(id)==stoi(servers[i]->getId())){
         return i;
      }
   }
   return i;
}
void bellmanFord(nodoServer* origen,nodoServer*destino,int tamano){
    vector<int> dist(300, INT_MAX); //tabla del algoritmo, se almacenan todas las distancias desde un determinado nodo. 
    vector<nodoServer*> current; 
    vector<nodoServer*> camino(300,nullptr); //se crea un vector que contendrá una referencia al nodo origen y los posteriores, para obtener el camino mas corto.
    vector<vector<int>> distancias(300); // se crea un vector donde se almacenarán las distancias entre vectores de un camino.
    camino[stoi(origen->getId())] = nullptr; 
    current.push_back(origen);
    dist[stoi(origen->getId())] = 0;
    bool distChanged = true;
    while(distChanged) // Se recorren todos los nodos y sus adyacentes, se forma la tabla del algoritmo, con la distancia de el nodo origen a todos los demás nodos.
    {    
        distChanged=false;
        vector<nodoServer*>visited;
        for(auto &nodoOrigen:current)
        {
            for(auto &nodoDest:nodoOrigen->getServers())
            {
                if(dist[stoi(nodoDest.first->getId())]>dist[stoi(nodoOrigen->getId())]+nodoDest.second)
                {
                    dist[stoi(nodoDest.first->getId())]=dist[stoi(nodoOrigen->getId())]+nodoDest.second;
                    camino[stoi(nodoDest.first->getId())]=nodoOrigen;
                    distancias[stoi(nodoDest.first->getId())]=distancias[stoi(nodoOrigen->getId())];
                    distancias[stoi(nodoDest.first->getId())].push_back(nodoDest.second);
                    visited.push_back(nodoDest.first);
                    distChanged = true; //Si existe cambio en alguna distancia, el ciclo comienza denuevo.
                }
            }
        }
        current.clear();
        current=visited; 
    }
   nodoServer*currentNodo=destino;
   vector<int>velocidades;//Vector que almacena las velocidades para cada conexion dentro del camino corto.
   vector<nodoServer*>caminofinal; //Vector que almacena unicamente los nodos del camino mas corto.
   vector<int>segundosFinal; //vector que almacena los segundos entre cada nodo.
   //Se imprimen desde el vector camino los nodos en el camino determinado,desde el destino hasta el origen.Además se agregan los nodos especificos al vector caminofinal.
    cout <<"El mejor camino desde "<<origen->getId()<< " hasta " <<destino->getId()<< " es:  ";
    while(currentNodo!=nullptr) {
        cout<<currentNodo->getId() << "|";
        caminofinal.push_back(currentNodo);
        currentNodo=camino[stoi(currentNodo->getId())];
    }
   //se agregan las velocidades de cada nodo dentro de caminofinal, al leerse el camino desde el destino hacia el origen, entonces las velocidades deben estar en el mismo sentido-
   for(int i=caminofinal.size()-1;i>0;i--){
      velocidades.push_back(caminofinal[i]->getVelocidad(caminofinal[i-1]->getId()));
   }
    //Se agregan los segundos a segundosFinal desde vector distancia.
   for(auto&distancia:distancias[stoi(destino->getId())]) {
        segundosFinal.push_back(distancia);
    }
    for(int i=0;i<segundosFinal.size();i++){
      //cout<<segundosFinal[i]<<"/"<<velocidades[i]<<" ";
    }
   cout<<endl;
   //finalmente se calculan los tiempos totales segun la velocidad de cada servidor
   int total=0;
   for(int i=segundosFinal.size();i>=0;i--){
      cout<<(tamano/velocidades[i])*segundosFinal[i]<<" seg. | ";
      total+=(tamano/velocidades[i])*segundosFinal[i];
      
   }
   cout<<"Tiempo total de demora del envío es: "<<total<<" segundos";
    cout<<endl;
}
void menu(){
   vector<nodoServer*>servers;
   leeServer(servers);
   leeConexiones(servers);
   cout<<"---------------MENU-----------------"<<endl;
   bool in=true;
   while(in){
      cout<<"1)Enviar Mensaje"<<endl;
      cout<<"2)Salir"<<endl;
      int opt;
      cin>>opt;
      switch(opt){
       case 1:
       {
         cout<<"Ingresar tu numero de servidor: "<<endl;
         string origen;
         cin>>origen;
         while(servers[stoi(origen)]->getType()=="router"){
            cout<<"Server invalido. Escoger un server cliente"<<endl;
            cin>>origen;
         }
         nodoServer* server1=servers[getIdServer(origen,servers)];
         
         cout<<"Ingresar numero de servidor destinatario: "<<endl;
         string destino;
         cin>>destino;
         cout<<"Tamaño del archivo"<<endl;
         string tamano;
         cin>>tamano;
         nodoServer *server2=servers[getIdServer(destino,servers)];
         bellmanFord(server1,server2,stoi(tamano));
         break;
       }
      case 2:
      {
         cout<<"Saliendo..."<<endl;;
         in=false;
         break;
      }
      default: 
      {
         cout<<"Opcion invalida."<<endl;
         break;
      }
      }
   }
   
}
int main(){
   menu();
}


