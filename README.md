# Image-Compression-with-Quadtrees
This project aims to provide a means of compressing 512x512 images using Quadtrees.



#### Notes :
(The image path is what you must write when you charge it)

## Utilisation d'un buffer dans `binary_converter` :
Petite explication des modifs au niveau du binary_converter :
- BitBuffer : Une structure pour gérer un buffer de bits.
- init_bit_buffer(), flush_bit_buffer(), write_bit(), read_bit(), write_byte(), read_byte() : Fonctions pour manipuler les bits et les bytes.
- save_node() et load_node() : Modifiées pour utiliser le buffer de bits.
- save_quadtree_binary() et load_quadtree_binary() : Initialisent et utilisent le buffer de bits.
Cela devrait maintenant nous permettre de sauvegarder et charger les données bit à bit en utilisant un buffer, ce qui optimise aussi l'utilisation de l'espace disque :D.
Je noterai également toutes ces modifications dans le rapport ;)

## correction (simple 1) :

Il y a une petite correction que j'ai faite pour comparer correctement les couleurs des feuilles dans minimize_identical_leaves_in_node. 
Avec `node->children[i]->color != first_color`, on compare les adresses de mémoire des couleurs, pas les valeurs des couleurs.
Je ne sais pas si c'est une amélioration logique mais on peut en discuter.
Je l'ai corrigé ainsi :
`if (node->children[i]->children != NULL || 
    node->children[i]->color->red != first_color->red || 
    node->children[i]->color->green != first_color->green || 
    node->children[i]->color->blue != first_color->blue || 
    node->children[i]->color->alpha != first_color->alpha) {
    identical_leaves_in_node = false;
    break;
}`

## minimisation (simple 2) :

`hash_color` : Calcule une valeur de hachage unique pour une couleur donnée.

`create_hash_table` : Crée et initialise une nouvelle table de hachage pour stocker les feuilles uniques par couleur.

`free_hash_table` : Libère la mémoire allouée pour la table de hachage et tous ses nœuds.

`find_leaf` : Recherche une feuille dans la table de hachage à partir de sa couleur. Retourne un pointeur vers la feuille si elle est trouvée, sinon retourne NULL.

`add_leaf` : Ajoute une feuille à la table de hachage en utilisant sa couleur comme clé.

`minimize_unique_leaves_aux` : Fonction récursive qui traverse l'arbre et minimise les feuilles. Si une feuille avec la même couleur existe déjà, elle remplace la feuille actuelle par cette feuille. Sinon, elle ajoute la feuille actuelle à la table de hachage.

`minimize_unique_leaves` : Fonction principale qui initialise la table de hachage, appelle la fonction auxiliaire pour minimiser l'arbre, puis libère la mémoire de la table de hachage.