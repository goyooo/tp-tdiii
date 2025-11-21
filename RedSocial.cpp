#include "RedSocial.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
using namespace std;


RedSocial::RedSocial() : amistades_count(0), id_mas_popular(-1) {
    it_conocidos_mas_popular = conocidos.end(); // Iterador inválido inicialmente
}
// Complejidad: O(1) OK

const set<int> & RedSocial::usuarios() const{
    return this->ids;
}
// Complejidad: O(1) OK

string RedSocial::obtener_alias(int id) const{
    return this->users.at(id);
}
// Complejidad: O(log n) OK

const set<string> & RedSocial::obtener_amigos(int id) const{
    return amigos.at(id);
}
// Complejidad: O(log n) OK

int RedSocial::cantidad_amistades() const{
    return this->amistades_count;
}
// Complejidad: O(1) OK

void RedSocial::registrar_usuario(string alias, int id){
    users[id] = alias;
    ids.insert(id); 
    amigos[id] = {};
    alias_to_id[alias] = id;
    conocidos[id] = {};

    // Si es el primer usuario o tiene más amigos que el actual más popular
    if (id_mas_popular == -1) {
        id_mas_popular = id;
        it_conocidos_mas_popular = conocidos.find(id);
    }
}
// Complejidad: O(log n) + O(1) en promedio OK

void RedSocial::eliminar_usuario(int id){
    // Guardar amigos antes de eliminar
    vector<string> amigos_a_eliminar;
    for(auto amigo : amigos[id]){
        amigos_a_eliminar.push_back(amigo);
    }
    
    // Desamigar de todos sus amigos
    for(auto amigo_alias : amigos_a_eliminar){
        desamigar_usuarios(id, obtener_id(amigo_alias));
    }
    
    // Eliminar todas las estructuras del usuario
    string alias = users[id];
    alias_to_id.erase(alias);
    users.erase(id);
    ids.erase(id);
    amigos.erase(id);
    conocidos.erase(id);

    // Si eliminamos al más popular, recalcular
    if (id == id_mas_popular) {
        recalcular_mas_popular();
    }
}
// Complejidad: Sin requerimiento

void RedSocial::amigar_usuarios(int id_A, int id_B){
    string alias_A = users[id_A];
    string alias_B = users[id_B];
    
    // Agregar amistad bidireccional
    amigos[id_A].insert(alias_B);
    amigos[id_B].insert(alias_A);
    this->amistades_count += 1;  

    // A y B ya no pueden ser conocidos entre sí
    conocidos[id_A].erase(alias_B);
    conocidos[id_B].erase(alias_A);

    // Actualizar conocidos: los amigos de B (excepto A) son conocidos de A si no son amigos directos
    for(auto amigo_de_B : amigos[id_B]){
        if(amigo_de_B != alias_A){
            int id_amigo_de_B = obtener_id(amigo_de_B);
            // Si no es amigo directo de A, entonces son conocidos
            if(amigos[id_A].count(amigo_de_B) == 0){
                conocidos[id_A].insert(amigo_de_B);
                conocidos[id_amigo_de_B].insert(alias_A);
            }
        }
    }

    // Actualizar conocidos: los amigos de A (excepto B) son conocidos de B si no son amigos directos
    for(auto amigo_de_A : amigos[id_A]){
        if(amigo_de_A != alias_B){
            int id_amigo_de_A = obtener_id(amigo_de_A);
            // Si no es amigo directo de B, entonces son conocidos
            if(amigos[id_B].count(amigo_de_A) == 0){
                conocidos[id_B].insert(amigo_de_A);
                conocidos[id_amigo_de_A].insert(alias_B);
            }
        }
    }

    // Recalcular el más popular (pueden haber cambiado las cantidades de amigos)
    recalcular_mas_popular();
}
// Complejidad: Sin requerimiento

void RedSocial::desamigar_usuarios(int id_A, int id_B){
    string alias_A = users[id_A];
    string alias_B = users[id_B];

    // Guardar amigos previos a cortar la amistad
    auto amigos_A_antes = amigos[id_A];
    auto amigos_B_antes = amigos[id_B];

    // Cortar amistad bidireccional
    amigos[id_A].erase(alias_B);
    amigos[id_B].erase(alias_A);
    amistades_count -= 1;

    // Conjunto de usuarios afectados que necesitan reconstruir sus conocidos
    set<int> afectados;
    afectados.insert(id_A);
    afectados.insert(id_B);
    for (const auto& aliasX : amigos_A_antes) afectados.insert(obtener_id(aliasX));
    for (const auto& aliasY : amigos_B_antes) afectados.insert(obtener_id(aliasY));

    // Reconstruir conocidos de todos los afectados
    for (int id : afectados) {
        reconstruir_conocidos_de(id);
    }

    // Recalcular el más popular
    recalcular_mas_popular();
}
// Complejidad: Sin requerimiento

int RedSocial::obtener_id(string alias) const{
    return alias_to_id.at(alias);
}
// Complejidad: O(1) en promedio OK

const set<string> & RedSocial::obtener_conocidos(int id) const{  
    return conocidos.at(id);
}
// Complejidad: O(log n) OK

const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    return it_conocidos_mas_popular->second;
}
// Complejidad: O(1) OK



// Funciones auxiliares

void RedSocial::reconstruir_conocidos_de(int id) {
    const string alias_u = users[id];
    auto& out = conocidos[id];
    out.clear();
    
    // Por cada amigo f de u...
    for (const auto& alias_f : amigos[id]) {
        int id_f = obtener_id(alias_f);
        // ...agrego los amigos de f como conocidos de u (si no son amigos directos de u)
        for (const auto& alias_w : amigos[id_f]) {
            if (alias_w != alias_u && amigos[id].count(alias_w) == 0) {
                out.insert(alias_w);
            }
        }
    }
}

void RedSocial::recalcular_mas_popular() {
    id_mas_popular = -1;
    int max_amigos = -1;
    
    // Buscar el usuario con más amigos
    for (int usuario_id : ids) {
        int cant_amigos = amigos[usuario_id].size();
        if (cant_amigos > max_amigos) {
            max_amigos = cant_amigos;
            id_mas_popular = usuario_id;
        }
    }
    
    // Actualizar el iterador a los conocidos del más popular
    if (id_mas_popular != -1) {
        it_conocidos_mas_popular = conocidos.find(id_mas_popular);
    } else {
        it_conocidos_mas_popular = conocidos.end();
    }
}