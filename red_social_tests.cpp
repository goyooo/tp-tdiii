#include <gtest/gtest.h>
#include "RedSocial.h"

using namespace std;

TEST(RedSocial, vacia) {
    RedSocial rs;
    
    EXPECT_EQ(0, rs.cantidad_amistades());
    EXPECT_EQ(set<int>(), rs.usuarios());
}

TEST(RedSocial, agregar_usuarios_ver_ids) {
    RedSocial rs;

    rs.registrar_usuario("agus", 1);
    rs.registrar_usuario("gerva", 2);
    rs.registrar_usuario("tom", 3);

    set<int> u = {1,2,3};
    EXPECT_EQ(u, rs.usuarios());

    rs.registrar_usuario("vir", 4);
    rs.registrar_usuario("vivi", 5);
    
    u = {1,2,3,4,5};
    EXPECT_EQ(u, rs.usuarios());
}

TEST(RedSocial, agregar_usuarios_ver_alias) {
    RedSocial rs;

    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("gerva", 2);
    rs.registrar_usuario("agus", 1);

    EXPECT_EQ("tom", rs.obtener_alias(3));
    EXPECT_EQ("gerva", rs.obtener_alias(2));
    EXPECT_EQ("agus", rs.obtener_alias(1));
}

TEST(RedSocial, agregar_usuarios_ver_amigos) {
    RedSocial rs;

    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("gerva", 2);
    rs.registrar_usuario("agus", 1);

    EXPECT_EQ(0, rs.cantidad_amistades());
    EXPECT_EQ(set<string>(), rs.obtener_amigos(3));
    EXPECT_EQ(set<string>(), rs.obtener_amigos(2));
    EXPECT_EQ(set<string>(), rs.obtener_amigos(1));
}

TEST(RedSocial, eliminar_usuarios_ver_ids) {
    RedSocial rs;

    rs.registrar_usuario("agus", 1);
    rs.registrar_usuario("gerva", 2);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 4);
    rs.registrar_usuario("vivi", 5);

    rs.eliminar_usuario(2);
    rs.eliminar_usuario(3);

    set<int> u = {1,4,5};
    EXPECT_EQ(u, rs.usuarios());
    
    rs.eliminar_usuario(1);
    rs.eliminar_usuario(4);
    rs.eliminar_usuario(5);

    u = {};
    EXPECT_EQ(u, rs.usuarios());
}

TEST(RedSocial, amigar_usuarios_ver_amigos) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,2);

    EXPECT_EQ(1, rs.cantidad_amistades());
    set<string> u = {"vir"};
    EXPECT_EQ(u, rs.obtener_amigos(1));
    u = {"vivi"};
    EXPECT_EQ(u, rs.obtener_amigos(2));

    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(4,5);

    EXPECT_EQ(3, rs.cantidad_amistades());
    u = {"vir", "tom"};
    EXPECT_EQ(u, rs.obtener_amigos(1));
    u = {"vivi"};
    EXPECT_EQ(u, rs.obtener_amigos(2));
    u = {"vivi"};
    EXPECT_EQ(u, rs.obtener_amigos(3));
    u = {"agus"};
    EXPECT_EQ(u, rs.obtener_amigos(4));
    u = {"gerva"};
    EXPECT_EQ(u, rs.obtener_amigos(5));
}

TEST(RedSocial, amigar_usuarios_ver_conocidos) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,2);

    set<string> u = {};
    EXPECT_EQ(u, rs.obtener_conocidos(1));
    EXPECT_EQ(u, rs.obtener_conocidos(2));

    // conocidos: 
    // - 4-1 y 3-2 
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(2,4);

    u = {"gerva"};
    EXPECT_EQ(u, rs.obtener_conocidos(1));
    u = {"tom"};
    EXPECT_EQ(u, rs.obtener_conocidos(2));
    u = {"vir"};
    EXPECT_EQ(u, rs.obtener_conocidos(3));
    u = {"vivi"};
    EXPECT_EQ(u, rs.obtener_conocidos(4));
    u = {};
    EXPECT_EQ(u, rs.obtener_conocidos(5));

    // los conocidos 1-4, 2-3 se amigan 
    // pero ahora 3-4 y  
    rs.amigar_usuarios(1,4);
    rs.amigar_usuarios(2,3);
    // 3-4 quedan como conocidos a través de 1 o de 2

    u = {};
    EXPECT_EQ(u, rs.obtener_conocidos(1));
    EXPECT_EQ(u, rs.obtener_conocidos(2));
    u = {"gerva"};
    EXPECT_EQ(u, rs.obtener_conocidos(3));
    u = {"tom"};
    EXPECT_EQ(u, rs.obtener_conocidos(4));
    u = {};
    EXPECT_EQ(u, rs.obtener_conocidos(5));

}

TEST(RedSocial, desamigar_usuarios_ver_amigos) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(4,5);
    rs.desamigar_usuarios(1,2);

    EXPECT_EQ(2, rs.cantidad_amistades());
    set<string> u = {"tom"};
    EXPECT_EQ(u, rs.obtener_amigos(1));
    u = {};
    EXPECT_EQ(u, rs.obtener_amigos(2));
    u = {"vivi"};
    EXPECT_EQ(u, rs.obtener_amigos(3));
    u = {"agus"};
    EXPECT_EQ(u, rs.obtener_amigos(4));
    u = {"gerva"};
    EXPECT_EQ(u, rs.obtener_amigos(5));
}

TEST(RedSocial, desamigar_usuarios_ver_conocidos) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);

    // 2-3 son conocidos a través de 1
    set<string> u = {"tom"};
    EXPECT_EQ(u,rs.obtener_conocidos(2));
    u = {"vir"};
    EXPECT_EQ(u,rs.obtener_conocidos(3));

    rs.amigar_usuarios(4,2);

    // 1-4 son conocidos a través de 2
    u = {"gerva"};
    EXPECT_EQ(u,rs.obtener_conocidos(1));
    u = {"vivi"};
    EXPECT_EQ(u,rs.obtener_conocidos(4));


    rs.desamigar_usuarios(1,2);

    // Ahora 1-4 ya no son conocidos, y 2-3 tampoco

    u = {};
    EXPECT_EQ(u, rs.obtener_conocidos(1));
    EXPECT_EQ(u, rs.obtener_conocidos(2));
    EXPECT_EQ(u, rs.obtener_conocidos(3));
    EXPECT_EQ(u, rs.obtener_conocidos(4));
    EXPECT_EQ(u, rs.obtener_conocidos(5));
}

TEST(RedSocial, eliminar_usuarios_ver_amigos) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(4,5);

    rs.eliminar_usuario(1);

    EXPECT_EQ(1, rs.cantidad_amistades());
    set<string> u = {};
    EXPECT_EQ(u, rs.obtener_amigos(2));
    u = {};
    EXPECT_EQ(u, rs.obtener_amigos(3));
    u = {"agus"};
    EXPECT_EQ(u, rs.obtener_amigos(4));
    u = {"gerva"};
    EXPECT_EQ(u, rs.obtener_amigos(5));
}

TEST(RedSocial, eliminar_usuarios_ver_conocidos) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(4,5);

    // hasta acá 2-3 son conocidos
 
    rs.eliminar_usuario(1);

    // 2-3 no deberían conocerse

    set<string> u = {};
    EXPECT_EQ(u, rs.obtener_conocidos(2));
    EXPECT_EQ(u, rs.obtener_conocidos(3));
}


TEST(RedSocial, obtener_id) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 0);
    rs.registrar_usuario("tom", 27);

    EXPECT_EQ(5, rs.obtener_id("agus"));
    EXPECT_EQ(0, rs.obtener_id("gerva"));
    EXPECT_EQ(27, rs.obtener_id("tom"));
}

TEST(RedSocial, amigar_usuarios_ver_amigos_popular) {
    RedSocial rs;

    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(4,2);
    rs.amigar_usuarios(4,5);

    // "gerva" es el más popular, pero no tiene conocidos
    set<string> u = {};
    EXPECT_EQ(u, rs.conocidos_del_usuario_mas_popular());

    // Ahora tiene a "vivi" como conocida a través de "tom"
    rs.amigar_usuarios(4,3);

    u = {"vivi"};
    EXPECT_EQ(u, rs.conocidos_del_usuario_mas_popular());

    rs.amigar_usuarios(4,1);

    u = {};
    EXPECT_EQ(u, rs.conocidos_del_usuario_mas_popular());
}

TEST(RedSocial, desamigar_usuarios_ver_conocidos_popular_cambia) {
    RedSocial rs;

    rs.registrar_usuario("pepe", 6);
    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(4,5);
    rs.amigar_usuarios(4,3);
    rs.amigar_usuarios(1,5);
    rs.amigar_usuarios(6,5);
    // red de amigos:
    //   +-----+
    //   |     |
    // 6-5-4-3-1-2

    // el más popular es 5 o 1:
    set<string> u5 = {"tom", "vir"}, 
                u1 = {"gerva","pepe"};
    EXPECT_TRUE(u5 == rs.conocidos_del_usuario_mas_popular()
        || u1 == rs.conocidos_del_usuario_mas_popular());

    rs.desamigar_usuarios(5,1);
    rs.desamigar_usuarios(3,1);

    // red de amigos final: -- más popular 5 o 4:
    // 6-5-4-3 1-2

    set<string> u4 = {"pepe"}; 
                u5 = {"tom"};
    EXPECT_TRUE(u5 == rs.conocidos_del_usuario_mas_popular()
        || u4 == rs.conocidos_del_usuario_mas_popular());
}

TEST(RedSocial, desamigar_usuarios_ver_conocidos_popular_no_cambia) {
    RedSocial rs;

    rs.registrar_usuario("pablo", 7);
    rs.registrar_usuario("pepe", 6);
    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);
    rs.registrar_usuario("pedro", 0);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(1,5);
    rs.amigar_usuarios(1,0);
    rs.amigar_usuarios(4,5);
    rs.amigar_usuarios(4,3);
    rs.amigar_usuarios(6,5);
    rs.amigar_usuarios(6,7);
    rs.amigar_usuarios(2,0);
    // red de amigos:
    //     +-----+
    //     |     |   
    // 7-6-5-4-3-1-2-0
    //           |   |
    //           +---+

    // el más popular es 1:
    set<string> u1 = {"gerva","pepe"};

    EXPECT_EQ(u1, rs.conocidos_del_usuario_mas_popular());            
          
    rs.desamigar_usuarios(2,0);
    rs.desamigar_usuarios(6,7);

    // red de amigos final: -- más popular 1:
    //     +-----+
    //     |     |   
    // 7 6-5-4-3-1-2 0
    //           |   |
    //           +---+

    EXPECT_EQ(u1, rs.conocidos_del_usuario_mas_popular());            
}

TEST(RedSocial, eliminar_usuario_ver_conocidos_popular_cambia) {
    RedSocial rs;
    
    rs.registrar_usuario("pablo", 7);
    rs.registrar_usuario("pepe", 6);
    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);
    rs.registrar_usuario("pedro", 0);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(1,5);
    rs.amigar_usuarios(1,0);
    rs.amigar_usuarios(4,5);
    rs.amigar_usuarios(4,3);
    rs.amigar_usuarios(6,5);
    rs.amigar_usuarios(6,7);
    rs.amigar_usuarios(2,0);
    // red de amigos:
    //     +-----+
    //     |     |   
    // 7-6-5-4-3-1-2-0
    //           |   |
    //           +---+

    // el más popular es 1:
    set<string> u1 = {"gerva","pepe"};

    EXPECT_EQ(u1, rs.conocidos_del_usuario_mas_popular());            
          
    rs.eliminar_usuario(2);
    rs.eliminar_usuario(0);

    // red de amigos final: -- más popular 5:
    //     +-----+
    //     |     |   
    // 7-6-5-4-3-1   

    set<string> u5 = {"tom","pablo"};    
    EXPECT_EQ(u5, rs.conocidos_del_usuario_mas_popular());            
}

TEST(RedSocial, eliminar_usuario_ver_conocidos_popular_no_cambia) {
    RedSocial rs;
    
    rs.registrar_usuario("pablo", 7);
    rs.registrar_usuario("pepe", 6);
    rs.registrar_usuario("agus", 5);
    rs.registrar_usuario("gerva", 4);
    rs.registrar_usuario("tom", 3);
    rs.registrar_usuario("vir", 2);
    rs.registrar_usuario("vivi", 1);
    rs.registrar_usuario("pedro", 0);

    rs.amigar_usuarios(1,2);
    rs.amigar_usuarios(1,3);
    rs.amigar_usuarios(1,0);
    rs.amigar_usuarios(4,5);
    rs.amigar_usuarios(4,3);
    rs.amigar_usuarios(6,5);
    rs.amigar_usuarios(6,7);
    rs.amigar_usuarios(2,0);

    // red de amigos:
    // 7-6-5-4-3-1-2-0
    //           |   |
    //           +---+

    // el más popular es 1:
    set<string> u1 = {"gerva"};
    EXPECT_EQ(u1, rs.conocidos_del_usuario_mas_popular());            
          
    rs.eliminar_usuario(7);
    rs.eliminar_usuario(6);
    rs.eliminar_usuario(5);

    // red de amigos final:
    // 4-3-1-2-0
    //     |   |
    //     +---+

    // el más popular es 1:
    EXPECT_EQ(u1, rs.conocidos_del_usuario_mas_popular());            
}