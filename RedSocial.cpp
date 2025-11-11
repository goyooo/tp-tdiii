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
    return conocidos.at(id);
}

int RedSocial::cantidad_amistades() const{
    return this->amistades_count;
}

void RedSocial::registrar_usuario(string alias, int id){
    users[id] = alias;
    ids.insert(id);
    amigos[id] = {};
    alias_to_id[alias] = id;
    conocidos[id] = {};
}

void RedSocial::amigar_usuarios(int id_A, int id_B){
    string alias_A = users[id_A];
    string alias_B = users[id_B];
    amigos[id_A].insert(alias_B);
    amigos[id_B].insert(alias_A);
    this->amistades_count += 1;  

    //A y B ya no pueden ser conocidos
    conocidos[id_A].erase(alias_B);
    conocidos[id_B].erase(alias_A);

        for(auto amigo_de_B : amigos[id_B]){
        if(amigo_de_B != alias_A){
            int id_amigo_de_B = obtener_id(amigo_de_B);

            //Si Y no es amigo directo de A, entonces son conocidos
            if(amigos[id_A].count(amigo_de_B) == 0){
                conocidos[id_A].insert(amigo_de_B);
                conocidos[id_amigo_de_B].insert(alias_A);
            }
        }
    }

    //Actualizar conocidos de B con los amigos de A
    for(auto amigo_de_A : amigos[id_A]){
        if(amigo_de_A != alias_B){
            int id_amigo_de_A = obtener_id(amigo_de_A);

            //Si X no es amigo directo de B, entonces son conocidos
            if(amigos[id_B].count(amigo_de_A) == 0){
                conocidos[id_B].insert(amigo_de_A);
                conocidos[id_amigo_de_A].insert(alias_B);
            }
        }
    }
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




int RedSocial::obtener_id(string alias) const{
    return alias_to_id.at(alias);
}

const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    
    int id_usuario = conocidos.rbegin()->first;
    int compare = conocidos.rbegin()->second.size();
    
    for(auto ids : conocidos){
        if(ids.first != id_usuario && ids.second.size() > compare){
            id_usuario = ids.first;
            compare = ids.second.size();
        }
    }
    return conocidos.at(id_usuario);
}

