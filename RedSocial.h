#ifndef __REDSOCIAL_H__
#define __REDSOCIAL_H__

#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
using namespace std;

class RedSocial{
  public:
    RedSocial(); // O(1)

    const set<int> & usuarios() const; // O(1)
    string obtener_alias(int id) const; // O(log n)
    const set<string> & obtener_amigos(int id) const; // O(log n)
    int cantidad_amistades() const; // O(1)

    void registrar_usuario(string alias, int id); // O(log n) + O(1) promedio
    void eliminar_usuario(int id); // sin requerimiento
    void amigar_usuarios(int id_A, int id_B); // sin requerimiento
    void desamigar_usuarios(int id_A, int id_B); // sin requerimiento
    
    int obtener_id(string alias) const; // O(1) promedio
    const set<string> & obtener_conocidos(int id) const; // O(log n)
    const set<string> & conocidos_del_usuario_mas_popular() const; // O(1)

  private:
    void reconstruir_conocidos_de(int id);
    void recalcular_mas_popular();
    
    map<int, string> users; // id y alias
    set<int> ids; // ids unicos
    map<int, set<string>> amigos; // id y alias de amigos

    unordered_map<string, int> alias_to_id;
    map<int, set<string>> conocidos; // id y alias de conocidos
    int amistades_count;

    int id_mas_popular;
    map<int, set<string>>::iterator it_conocidos_mas_popular;
    
    
    /*
    INVARIANTE DE REPRESENTACION
    
    EN ESPAÑOL:
    - Todos los ids en 'ids' tienen una entrada correspondiente en 'users', 'amigos' y 'conocidos'
    - Para cada id en users, existe una entrada inversa en alias_to_id
    - Todos los alias son únicos, no vacíos y tienen como máximo 200 caracteres
    - Las relaciones de amistad son simétricas: si B está en amigos[A], entonces A está en amigos[B]
    - Los conocidos de un usuario U son aquellos usuarios V tales que existe un usuario W donde:
      W está en amigos[U], V está en amigos[W], y V NO está en amigos[U]
    - amistades_count es igual a la suma de |amigos[id]| / 2 para todo id en ids
    - id_mas_popular es -1 si no hay usuarios, o es un id en ids que tiene la máxima cantidad de amigos
    - it_conocidos_mas_popular apunta a los conocidos del usuario más popular si existe, sino a conocidos.end()
    - Ningún usuario es amigo de sí mismo
    - Ningún usuario es conocido de sí mismo
    
    EN LOGICA:
    (∀id : int) id ∈ ids ⟺ (id ∈ claves(users) ∧ id ∈ claves(amigos) ∧ id ∈ claves(conocidos))
    
    (∀id : int) id ∈ claves(users) ⟹ users[id] ∈ claves(alias_to_id) ∧ alias_to_id[users[id]] = id
    
    (∀alias : string) alias ∈ claves(alias_to_id) ⟹ (alias ≠ "" ∧ |alias| ≤ 200)
    
    (∀id_A, id_B : int) id_A ∈ ids ∧ id_B ∈ ids ⟹ 
        (users[id_B] ∈ amigos[id_A] ⟺ users[id_A] ∈ amigos[id_B])
    
    (∀id_U, id_V : int) id_U ∈ ids ∧ id_V ∈ ids ⟹
        (users[id_V] ∈ conocidos[id_U] ⟺ 
            (∃id_W : int) id_W ∈ ids ∧ 
                users[id_W] ∈ amigos[id_U] ∧ 
                users[id_V] ∈ amigos[id_W] ∧ 
                users[id_V] ∉ amigos[id_U] ∧
                id_V ≠ id_U)
    
    amistades_count = (Σ id ∈ ids: |amigos[id]|) / 2
    
    (ids = ∅ ⟹ id_mas_popular = -1) ∧
    (ids ≠ ∅ ⟹ id_mas_popular ∈ ids ∧ 
        (∀id : int) id ∈ ids ⟹ |amigos[id_mas_popular]| ≥ |amigos[id]|)
    
    (id_mas_popular = -1 ⟹ it_conocidos_mas_popular = conocidos.end()) ∧
    (id_mas_popular ≠ -1 ⟹ esValido(it_conocidos_mas_popular) ∧ clave(it_conocidos_mas_popular) = id_mas_popular)
    
    (∀id : int) id ∈ ids ⟹ users[id] ∉ amigos[id]
    
    (∀id : int) id ∈ ids ⟹ users[id] ∉ conocidos[id]
    */
};

#endif