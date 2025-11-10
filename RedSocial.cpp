#include "RedSocial.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
using namespace std;

RedSocial::RedSocial():amistades_count(0){
}

const set<int> & RedSocial::usuarios() const{
    return this->ids;
}
string RedSocial::obtener_alias(int id) const{
    return this->users.at(id);
}
const set<string> & RedSocial::obtener_amigos(int id) const{
    return amigos.at(id);
}

const set<string> & RedSocial::obtener_conocidos(int id) const{
    set<string> conocidos_de_id = {};
    for(auto amigo : amigos.at(id)){
        int id_amigo = obtener_id(amigo);
        for(auto conocido_de_amigo : amigos.at(id_amigo)){
            if(conocido_de_amigo != users.at(id) && amigos.at(id).find(conocido_de_amigo) == amigos.at(id).end()){
                conocidos_de_id.insert(conocido_de_amigo);
            }
        }
    }
    
    return conocidos_de_id;
}

int RedSocial::cantidad_amistades() const{
    return this->amistades_count;
}

void RedSocial::registrar_usuario(string alias, int id){
    users[id] = alias;
    ids.insert(id);
    amigos[id] = {};
    alias_to_id[alias] = id;
}

void RedSocial::eliminar_usuario(int id){
    vector<string> amigos_a_eliminar;
    for(auto amigo : amigos[id]){
        amigos_a_eliminar.push_back(amigo);
    }
    for(auto keys : amigos_a_eliminar){
        desamigar_usuarios(id, obtener_id(keys));
    }
    string alias = users[id];
    alias_to_id.erase(alias);
    users.erase(id);
    ids.erase(id);
    amigos.erase(id);
    conocidos.erase(id);
}

void RedSocial::amigar_usuarios(int id_A, int id_B){
    string alias_A = users[id_A];
    string alias_B = users[id_B];
    amigos[id_A].insert(alias_B);
    amigos[id_B].insert(alias_A);

    this->amistades_count += 1;  

    conocidos[id_A].insert(alias_B);
    conocidos[id_B].insert(alias_A);
}

void RedSocial::desamigar_usuarios(int id_A, int id_B){
    string alias_A = users[id_A];
    string alias_B = users[id_B];
    amigos[id_A].erase(alias_B);
    amigos[id_B].erase(alias_A);

    this->amistades_count -= 1;

    conocidos[id_A].erase(alias_B);
    conocidos[id_B].erase(alias_A);

}

int RedSocial::obtener_id(string alias) const{
    
    return alias_to_id.at(alias);
    
}

const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    set<string> conocidos_populares = {};
    return conocidos_populares;
}

