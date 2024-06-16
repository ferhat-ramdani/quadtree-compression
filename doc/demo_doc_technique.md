### Architecture :
- binary_converter.c / binary_converter.h:

    Fonctionnalité: Sauvegarde et charge un quadtree au format binaire.
    Fonctions clés:
        save_quadtree_binary(): Sauvegarde un quadtree dans un fichier binaire.
        load_quadtree_binary(): Charge un quadtree depuis un fichier binaire.
        save_node() et load_node(): Fonctions auxiliaires pour parcourir et (dé)sérialiser les nœuds.

- bw_tree.c / bw_tree.h:

    Fonctionnalité: Manipule des arbres en noir et blanc.
    Fonctions clés:
        create_bw_node(): Crée un nœud avec des enfants.
        create_bw_leaf(): Crée une feuille.
        display_bw_tree(): Affiche l'arbre en noir et blanc.
        bw_distance(): Calcule la distance entre deux arbres.
        free_bw_tree(): Libère la mémoire allouée pour l'arbre.

- c_tree.c / c_tree.h:

    Fonctionnalité: Manipule des arbres colorés.
    Fonctions clés:
        create_c_node(): Crée un nœud avec des enfants.
        create_c_leaf(): Crée une feuille.
        display_c_tree(): Affiche l'arbre coloré.
        c_exact_distance() et c_mean_distance(): Calculent les distances entre les arbres.
        mean_color(): Calcule la couleur moyenne des enfants.
        free_c_tree(): Libère la mémoire allouée pour l'arbre.

- color.c / color.h:

    Fonctionnalité: Manipule des couleurs.
    Fonctions clés:
        create_color(): Crée une couleur.
        print_color(): Affiche une couleur.
        c_c_distance(): Calcule la distance entre deux couleurs.
        free_color(): Libère la mémoire allouée pour une couleur.

- gui.c / gui.h:

    Fonctionnalité: Gère l'interface graphique pour l'application de compression d'image.
    Fonctions clés:
        initialize_window(): Initialise la fenêtre graphique.
        draw_buttons(): Dessine les boutons de l'interface.
        handle_buttons(): Gère les clics sur les boutons et les actions correspondantes.
        average_color(): Calcule la couleur moyenne d'une zone d'une image.
        create_c_tree_from_image(): Crée un quadtree à partir d'une image.
        draw_c_tree_as_image(): Dessine le quadtree comme une image.
        approximate_image(): Approxime une image en utilisant un quadtree.

- minimizer.c / minimizer.h:

    Fonctionnalité: Minimise un quadtree en combinant des feuilles identiques.
    Fonction clé:
        minimize_identical_leaves_in_node(): Minimise les nœuds du quadtree en combinant des feuilles identiques.

- main.c:

    Fonctionnalité: Point d'entrée de l'application.
    Fonctionnalité clé:
        Initialisation de la fenêtre graphique.
        Gestion des boutons et des actions utilisateur.

- makefile:

    Fonctionnalité: Automatisation de la compilation.
    Cibles clés:
        all: Compile le programme principal.
        debug: Compile le programme avec les informations de débogage.
        clean: Supprime les fichiers compilés.




### minimisation (simple 2):

Table de hachage : Nous avons défini une structure HashTable pour stocker les feuilles uniques par couleur. La fonction hash_color calcule une clé de hachage pour une couleur donnée.

Fonctions de la table de hachage :
    create_hash_table : Initialise la table de hachage.
    free_hash_table : Libère la mémoire de la table de hachage.
    find_leaf : Recherche une feuille dans la table de hachage.
    add_leaf : Ajoute une feuille dans la table de hachage.
Minimisation :
    minimize_unique_leaves_aux : Fonction auxiliaire pour traverser l'arbre et minimiser les feuilles.
    minimize_unique_leaves : Fonction principale qui crée la table de hachage et appelle la fonction auxiliaire."
