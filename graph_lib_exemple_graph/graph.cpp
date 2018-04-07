#include "graph.h"
#include <fstream>

using namespace std;

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    ///Bouton ajouter sommet
    m_top_box.add_child(boutonAjouterSommet);
    boutonAjouterSommet.set_frame(0,0,100,100);
    boutonAjouterSommet.set_bg_color(NOIR);

    m_top_box.add_child(TboutonAjouterSommet);
    TboutonAjouterSommet.set_frame_pos(0,10);
    TboutonAjouterSommet.set_message("AJOUT SOMMET");
    TboutonAjouterSommet.set_bg_color(BLANC);

    ///Bouton ajouter arete
    m_top_box.add_child(boutonAjouterArete);
    boutonAjouterArete.set_frame(0,100,100,100);
    boutonAjouterArete.set_bg_color(VIOLET);

    m_top_box.add_child(TboutonAjouterArete);
    TboutonAjouterArete.set_frame_pos(0,110);
    TboutonAjouterArete.set_message("AJOUT ARETE");
    TboutonAjouterArete.set_bg_color(BLANC);

    ///Bouton supprimer sommet
    m_top_box.add_child(boutonSupprimerSommet);
    boutonSupprimerSommet.set_frame(0,200,100,100);
    boutonSupprimerSommet.set_bg_color(NOIR);

    m_top_box.add_child(TboutonSupprimerSommet);
    TboutonSupprimerSommet.set_frame_pos(0,210);
    TboutonSupprimerSommet.set_message("SUP SOMMET");
    TboutonSupprimerSommet.set_bg_color(BLANC);

    ///Bouton supprimer arete
    m_top_box.add_child(boutonSupprimerArete);
    boutonSupprimerArete.set_frame(0,300,100,100);
    boutonSupprimerArete.set_bg_color(VIOLET);

    m_top_box.add_child(TboutonSupprimerArete);
    TboutonSupprimerArete.set_frame_pos(0,310);
    TboutonSupprimerArete.set_message("SUP ARETE");
    TboutonSupprimerArete.set_bg_color(BLANC);

    ///Bouton sauvegarde
    m_top_box.add_child(boutonSauvegarde);
    boutonSauvegarde.set_frame(0,400,100,100);
    boutonSauvegarde.set_bg_color(NOIR);

    m_top_box.add_child(TboutonSauvegarde);
    TboutonSauvegarde.set_frame_pos(0,410);
    TboutonSauvegarde.set_message("SAUVEGARDE");
    TboutonSauvegarde.set_bg_color(BLANC);

    ///Bouton retour menu
    m_top_box.add_child(boutonRetourMenu);
    boutonRetourMenu.set_frame(0,500,100,100);
    boutonRetourMenu.set_bg_color(VIOLET);

    m_top_box.add_child(TboutonRetourMenu);
    TboutonRetourMenu.set_frame_pos(0,510);
    TboutonRetourMenu.set_message("RETOUR MENU");
    TboutonSupprimerSommet.set_bg_color(BLANC);


}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}

// Lecture des fichiers textes:
void Graph::Lire_fichier(std::string nomfichier)
{
    ///Declaration des variables pour la lecture des sommets
    std::string nomImage;
    int indiceSommet;
    int ordre;
    double valeur;
    int x;
    int y;

    ///Declaration des variables pour la lecture des aretes
    int nbAretes;
    int indiceArete;
    int indiceS1;
    int indiceS2;
    double poids;

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    std::ifstream fichier(nomfichier, ios::in);

    if(fichier)
    {
        ///Lecture des sommets
        fichier >> ordre;

        for (int i=0; i<ordre;i++)
        {
            fichier >> indiceSommet;
            fichier >> nomImage;
            fichier >> valeur;
            fichier >> x;
            fichier >> y;

            add_interfaced_vertex(indiceSommet, valeur, x, y, nomImage);
        }

        ///Lecture des aretes
        fichier >> nbAretes;

        for (int i=0; i<nbAretes;i++)
        {
            fichier >> indiceArete;
            fichier >> indiceS1;
            fichier >> indiceS2;
            fichier >> poids;

            add_interfaced_edge(indiceArete, indiceS1, indiceS2, poids);
        }
    }
    fichier.close();
}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    ///Ajouter sommet
    if(m_interface->boutonAjouterSommet.clicked())
    {
        ajouterSommet();
    }

    ///Ajouter Arete
    if(m_interface->boutonAjouterArete.clicked())
    {
        ajouterArete();
    }

    ///Supprimer sommet
    if(m_interface->boutonSupprimerSommet.clicked())
    {
        supprimerSommet();
    }

    ///Supprimer arete
    if(m_interface->boutonSupprimerArete.clicked())
    {
        supprimerArete();
    }



}

/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
/// r�f�rence vers le Edge � enlever
Edge &remed=m_edges.at(eidx);
std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;
/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
std::cout << m_edges.size() << std::endl;
/// test : on a bien des �l�ments interfac�s
if (m_interface && remed.m_interface)
{
/// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
/* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
/* m_interface->m_main_box.add_child(ei->m_top_edge); */
/* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
}
/// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
std::vector<int> &veto = m_vertices[remed.m_to].m_in;
vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );
/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
m_edges.erase( eidx );
/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
std::cout << m_edges.size() << std::endl;
}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::ajouterSommet()
{
    std::string nomImage;
    double valeur;
    cout << "Entrer la valeur du sommet" << endl;
    cin >> valeur;
    cout << "Entrer le nom de l'image" << endl;
    cin >> nomImage;

    add_interfaced_vertex(m_vertices.size(), valeur, 120, 10, nomImage);
}

void Graph::ajouterArete()
{
    int indiceS1, indiceS2;
    double poids;
    cout << "Entrer l'indice du sommet de depart" << endl;
    cin >> indiceS1;
    cout << "Entrer l'indice du sommet d'arrivee" << endl;
    cin >> indiceS2;
    cout << "Entrer le poids de l'arete" << endl;
    cin >> poids;

    add_interfaced_edge(m_edges.size(), indiceS1, indiceS2, poids);
}

void Graph::supprimerArete()
{
    int indiceS1, indiceS2, indiceArete;
    cout << "Entrer l'indice du sommet de depart" << endl;
    cin >> indiceS1;
    cout << "Entrer l'indice du sommet d'arrivee" << endl;
    cin >> indiceS2;

    if(m_vertices.count(indiceS1) + m_vertices.count(indiceS2) == 2)
    {
        for(auto elem: m_edges)
        {
            if(elem.second.m_from==indiceS1 && elem.second.m_to==indiceS2)
            {
                indiceArete=elem.first;
            }
        }
        test_remove_edge(indiceArete);
    }
}

void Graph::supprimerSommet()
{
    int indiceSommet;
    cout <<"Entrer l'indice du sommet" << endl;
    cin >> indiceSommet;

    if(m_vertices.count(indiceSommet) == 1)
    {
        for(int i=0; i<m_vertices[indiceSommet].m_in.size(); i++)
        {
            this->test_remove_edge(m_vertices[indiceSommet].m_in[i]);
        }

        for(int i=0; i<m_vertices[indiceSommet].m_out.size(); i++)
        {
            this->test_remove_edge(m_vertices[indiceSommet].m_out[i]);
        }

        m_interface->m_main_box.remove_child( m_vertices[indiceSommet].m_interface->m_top_box );
        m_vertices.erase(indiceSommet);
    }
}

void Graph::menu()
{
    BITMAP *buffer;
    BITMAP *fond;
    BITMAP *fond2;
    bool quitter = false;
    int mouse_prec;
    int mouse_nouv;
    //std::string nomfichier;

    buffer=create_bitmap(800, 600);
    clear_bitmap(buffer);

    ///Chargement des images
    fond = load_bitmap("fond.bmp", NULL);
    if(!fond)
    {
        allegro_message("Image non trouvee");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    fond2 = load_bitmap("fond2.bmp", NULL);
    if(!fond2)
    {
        allegro_message("Image non trouvee");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    while ( quitter==false )
    {
        blit(fond, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);


        clear_bitmap(buffer);

        mouse_prec=mouse_nouv;
        mouse_nouv=mouse_b&1;


        if( mouse_x>0 && mouse_x<300 && mouse_y>0 && mouse_y<300)
                {
                    if(!mouse_prec && mouse_nouv)

                    {
                     /// lancement du jeu niveau 1
                     //make_example();
                     Lire_fichier("essai.txt");
                        quitter=true;
                    }


                  /*  if (color2 == makecol(112,146,190))
                    {
                        ///lancement du jeu niveau 2
                        return 2;
                    }

                    if(color2 == makecol(163,73,164))
                    {
                        ///lancement du jeu niveau 3
                        return 3;
                    }*/
                }

                ///Si la souris est sur la position de la case "quitter"
        if(!mouse_prec && mouse_nouv && mouse_x>0 && mouse_x<300 && mouse_y>400 && mouse_y<600)
        {
            ///Si clique souris, on quitte
            quitter=true;

        }
    }
}





