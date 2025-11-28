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
// Complejidad: O(1), solo inicialización de variables y asignación de iterador

const set<int> & RedSocial::usuarios() const{
    return this->ids;
}
// Complejidad: O(1), retorna referencia directa al set, sin copia ni iteración

string RedSocial::obtener_alias(int id) const{
    return this->users.at(id);
}
// Complejidad: O(log n), búsqueda en map

const set<string> & RedSocial::obtener_amigos(int id) const{
    return amigos.at(id);
}
// Complejidad: O(log n), búsqueda en map 

int RedSocial::cantidad_amistades() const{
    return this->amistades_count;
}
// Complejidad: O(1), acceso directo a variable mantenida como invariante

void RedSocial::registrar_usuario(string alias, int id){
    users[id] = alias;                    // O(log n), inserción en map
    ids.insert(id);                       // O(log n), inserción en set
    amigos[id] = {};                      // O(log n), inserción en map
    alias_to_id[alias] = id;              // O(1), inserción en unordered_map
    conocidos[id] = {};                   // O(log n), inserción en map

    // Si es el primer usuario o tiene más amigos que el actual más popular
    if (id_mas_popular == -1) {
        id_mas_popular = id;              // O(1)
        it_conocidos_mas_popular = conocidos.find(id); // O(log n), búsqueda en map
    }
}
// Complejidad: O(log n) + O(1)

void RedSocial::eliminar_usuario(int id){
    // Guardar amigos antes de eliminar
    vector<string> amigos_a_eliminar;
    for(auto amigo : amigos[id]){               // O(k) donde k = grado del usuario
        amigos_a_eliminar.push_back(amigo);     // O(1) por inserción
    }
    
    // Desamigar de todos sus amigos
    for(auto amigo_alias : amigos_a_eliminar){  // O(k) iteraciones
        desamigar_usuarios(id, obtener_id(amigo_alias)); // O(k*n) en peor caso
    }
    
    // Eliminar todas las estructuras del usuario
    string alias = users[id];                   // O(log n), búsqueda en map
    alias_to_id.erase(alias);                   // O(1), borrado de unordered_map
    users.erase(id);                            // O(log n), borrado de map
    ids.erase(id);                              // O(log n), borrado de set
    amigos.erase(id);                           // O(log n), borrado de map
    conocidos.erase(id);                        // O(log n), borrado de map

    // Si eliminamos al más popular, recalcular
    if (id == id_mas_popular) {
        recalcular_mas_popular();               // O(n), recorre todos los usuarios
    }
}
// Complejidad: Sin requerimiento, pero es O(k*n) donde k es el grado del usuario eliminado

void RedSocial::amigar_usuarios(int id_A, int id_B){
    string alias_A = users[id_A];              // O(log n), búsqueda en map
    string alias_B = users[id_B];              // O(log n), búsqueda en map
    
    // Agregar amistad bidireccional
    amigos[id_A].insert(alias_B);              // O(log |amigos[id_A]|), inserción en set
    amigos[id_B].insert(alias_A);              // O(log |amigos[id_B]|), inserción en set
    this->amistades_count += 1;                // O(1)

    // A y B ya no pueden ser conocidos entre sí
    conocidos[id_A].erase(alias_B);            // O(log |conocidos[id_A]|), borrado en set
    conocidos[id_B].erase(alias_A);            // O(log |conocidos[id_B]|), borrado en set

    // Actualizar conocidos: los amigos de B (excepto A) son conocidos de A si no son amigos directos
    for(auto amigo_de_B : amigos[id_B]){       // O(|amigos[id_B]|)
        if(amigo_de_B != alias_A){
            int id_amigo_de_B = obtener_id(amigo_de_B);     // O(1), búsqueda en unordered_map
            // Si no es amigo directo de A, entonces son conocidos
            if(amigos[id_A].count(amigo_de_B) == 0){        // O(log |amigos[id_A]|), búsqueda en set
                conocidos[id_A].insert(amigo_de_B);         // O(log |conocidos[id_A]|), inserción en set
                conocidos[id_amigo_de_B].insert(alias_A);   // O(log |conocidos[id_amigo_de_B]|), inserción en set
            }
        }
    }

    // Actualizar conocidos: los amigos de A (excepto B) son conocidos de B si no son amigos directos
    for(auto amigo_de_A : amigos[id_A]){       // O(|amigos[id_A]|)
        if(amigo_de_A != alias_B){
            int id_amigo_de_A = obtener_id(amigo_de_A);     // O(1) promedio
            // Si no es amigo directo de B, entonces son conocidos
            if(amigos[id_B].count(amigo_de_A) == 0){        // O(log |amigos[id_B]|)
                conocidos[id_B].insert(amigo_de_A);         // O(log |conocidos[id_B]|)
                conocidos[id_amigo_de_A].insert(alias_B);   // O(log |conocidos[id_amigo_de_A]|)
            }
        }
    }

    // Recalcular el más popular (pueden haber cambiado las cantidades de amigos)
    recalcular_mas_popular();                  // O(n) - recorre todos los usuarios
}
// Complejidad: Sin requerimiento, pero esO(k*log n + n) donde k es el máximo entre id_A e id_B

void RedSocial::desamigar_usuarios(int id_A, int id_B){
    string alias_A = users[id_A];              // O(log n), búsqueda en map
    string alias_B = users[id_B];              // O(log n), búsqueda en map

    // Guardar amigos previos a cortar la amistad
    auto amigos_A_antes = amigos[id_A];        // O(|amigos[id_A]|), copia del set
    auto amigos_B_antes = amigos[id_B];        // O(|amigos[id_B]|),  copia del set

    // Cortar amistad bidireccional
    amigos[id_A].erase(alias_B);               // O(log |amigos[id_A]|), borrado en set
    amigos[id_B].erase(alias_A);               // O(log |amigos[id_B]|), borrado en set
    amistades_count -= 1;                      // O(1)

    // Conjunto de usuarios afectados que necesitan reconstruir sus conocidos
    set<int> afectados;                        // O(1)
    afectados.insert(id_A);                    // O(log |afectados|) = O(1) inicialmente
    afectados.insert(id_B);                    // O(log |afectados|) = O(1) 
    for (const auto& aliasX : amigos_A_antes) afectados.insert(obtener_id(aliasX)); // O(k_A * log k) donde k_A = |amigos_A_antes|
    for (const auto& aliasY : amigos_B_antes) afectados.insert(obtener_id(aliasY)); // O(k_B * log k) donde k_B = |amigos_B_antes|

    // Reconstruir conocidos de todos los afectados
    for (int id : afectados) {                 // O(k) iteraciones donde k = |afectados|
        reconstruir_conocidos_de(id);          // O(grado^2) por usuario
    }

    // Recalcular el más popular
    recalcular_mas_popular();                  // O(n) - recorre todos los usuarios
}
// Complejidad: Sin requerimiento, O(k*grado^2 + n) donde k es el número de usuarios afectados

int RedSocial::obtener_id(string alias) const{
    return alias_to_id.at(alias);              // O(1), búsqueda en unordered_map
}
// Complejidad: O(1)

const set<string> & RedSocial::obtener_conocidos(int id) const{  
    return conocidos.at(id);                    // O(log n), búsqueda en map
}
// Complejidad: O(log n)

const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    return it_conocidos_mas_popular->second;   // O(1)
}
// Complejidad: O(1) 



// Funciones auxiliares

void RedSocial::reconstruir_conocidos_de(int id) {
    const string alias_u = users[id];          // O(log n), búsqueda en map
    auto& out = conocidos[id];                 // O(log n), búsqueda en map
    out.clear();                               // O(|conocidos[id]|), limpiar set
    
    // Por cada amigo f de u...
    for (const auto& alias_f : amigos[id]) {   // O(|amigos[id]|) iteraciones
        int id_f = obtener_id(alias_f);        // O(1), búsqueda en unordered_map
        // agrego los amigos de f como conocidos de u (si no son amigos directos de u)
        for (const auto& alias_w : amigos[id_f]) { // O(|amigos[id_f]|)
            if (alias_w != alias_u && amigos[id].count(alias_w) == 0) { // O(log |amigos[id]|)
                out.insert(alias_w);           // O(log |conocidos[id]|)
            }
        }
    }
}
// Complejidad: O(grado^2 * log n), donde grado es el grado máximo del usuario

void RedSocial::recalcular_mas_popular() {
    id_mas_popular = -1;                       // O(1)
    int max_amigos = -1;                       // O(1)
    
    // Buscar el usuario con más amigos
    for (int usuario_id : ids) {               // O(n) iteraciones donde n = |ids|
        int cant_amigos = amigos[usuario_id].size(); // O(log n), búsqueda + O(1), size()
        if (cant_amigos > max_amigos) {        // O(1)
            max_amigos = cant_amigos;          // O(1)
            id_mas_popular = usuario_id;      // O(1)
        }
    }
    
    // Actualizar el iterador a los conocidos del más popular
    if (id_mas_popular != -1) {               // O(1)
        it_conocidos_mas_popular = conocidos.find(id_mas_popular); // O(log n), búsqueda en map
    } else {
        it_conocidos_mas_popular = conocidos.end(); // O(1)
    }
}
// Complejidad: O(n * log n) donde n es el número de usuarios