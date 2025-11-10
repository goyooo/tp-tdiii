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
    RedSocial();

    const std::set<int> & usuarios() const; // O(1)
    std::string obtener_alias(int id) const; // O(log n)
    const std::set<std::string> & obtener_amigos(int id) const; // O(log n)
    const std::set<std::string> & obtener_conocidos(int id) const; // O(log n)
    int cantidad_amistades() const; // O(1)

    void registrar_usuario(std::string alias, int id); // O(log n) + O(1) promedio
    void eliminar_usuario(int id); // sin requerimiento
    void amigar_usuarios(int id_A, int id_B); // sin requerimiento
    void desamigar_usuarios(int id_A, int id_B); // sin requerimiento
    
    int obtener_id(std::string alias) const; // O(1) promedio
    const std::set<std::string> & conocidos_del_usuario_mas_popular() const; // O(1)

  private:
    std::map<int, std::string> users;
    std::set<int> ids;
    std::map<int, std::set<std::string>> amigos;

    std::unordered_map<std::string, int> alias_to_id;
    std::map<int, set<std::string>> conocidos;
    int amistades_count;
    std::set<std::string> conocidos_populares;
};

#endif
