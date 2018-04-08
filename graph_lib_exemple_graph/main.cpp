#include "grman/grman.h"
#include <iostream>

#include "graph.h"

using namespace std;
int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    g.lirefichier("sauv_desert.txt");
    //g.make_example();
    //g.Lire_fichier("essai.txt");



    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();
        g.calcul_value();
        //g.menu();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
   // g.sauvegarde("sauv_desert.txt");

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


