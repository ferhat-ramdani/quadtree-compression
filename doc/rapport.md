# Projet: Compression d'Images avec des Quadtrees 
## Binôme: Ferhat RAMDANI / Ghassen HNID 
## Date: ---------------

## Introduction :

Ce projet consiste à implémenter une compression d'image utilisant des quadtrees. Nous avons structuré notre travail en plusieurs phases, incluant la mise en place de la structure initiale, le développement de la partie graphique, et l'ajout de fonctionnalités de minimisation sans perte et avec perte.

## Répartition des tâches :
Pour assurer une répartition équitable des tâches et une bonne progression, nous avons divisé le travail comme suit :

1. **Ghassen** :
   - Début de la partie graphique (interface utilisateur).
   - Conception et gestion des boutons de l'interface graphique.
   - Réflexion sur la première minimisation sans perte.
   - Rédaction du rapport (au fur et à mesure).
   - Implémentation de la deuxième minimisation sans perte.
   - Réécriture des fonctions save et load de binary_converter.c en utilisant un buffer bit à bit.

2. **Ferhat** :
   - Développement de la partie backend initiale (Binary_converter, c_tree, et bw_tree color.c).
   - Conception de la minimisation avec perte.
   - Merge les deux travaux initiaux des deux membres du binome.
   - Ajout de fonctionnalités frontend (bouton save noir et blanc et gestion des extensions).
   - Rédaction du rapport (au fur et à mesure).
   - Implémentation de la minimisation avec perte

À la fin, nous avons fait plusieurs réunions pour nous expliquer le code, fusionner nos travaux et synchroniser nos branches (main, ghassen, et ferhat).

## Avancements et Réflexions :
1. **Première minimisation sans perte** :
   - Fonctionne parfaitement. Réduit les nœuds en combinant les feuilles identiques.

2. **Deuxième minimisation sans perte** :
   - Réflexion et implémentation réussies. Remplace toutes les feuilles identiques par une référence unique. (en phase de test)

3. **Minimisation avec perte** :
   - En cours de développement. Vise à réduire davantage la taille de l'arbre en tolérant une perte de qualité.

4. **Réécriture des fonctions save et load** :
   - Utilisation d'un buffer pour gérer les données bit à bit. Améliore l'efficacité de la sauvegarde et du chargement des arbres.

## Conclusion :

Ce projet a permis de combiner nos compétences en développement backend et frontend pour réaliser une application de compression d'images avec quadtrees. Les deux minimisations sans perte sont fonctionnelles, et nous travaillons actuellement sur des améliorations supplémentaires. Nous avons également appris à collaborer efficacement en équipe, en partageant les tâches et en fusionnant nos travaux régulièrement.
