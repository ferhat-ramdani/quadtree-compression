# Image-Compression-with-Quadtrees
This project aims to provide a means of compressing 512x512 images using Quadtrees.



Notes :
(The image path is what you must write when you charge it)

Petite explication des modifs au niveau du binary_converter :
- BitBuffer : Une structure pour gérer un buffer de bits.
- init_bit_buffer(), flush_bit_buffer(), write_bit(), read_bit(), write_byte(), read_byte() : Fonctions pour manipuler les bits et les bytes.
- save_node() et load_node() : Modifiées pour utiliser le buffer de bits.
- save_quadtree_binary() et load_quadtree_binary() : Initialisent et utilisent le buffer de bits.
Cela devrait maintenant nous permettre de sauvegarder et charger les données bit à bit en utilisant un buffer, ce qui optimise aussi l'utilisation de l'espace disque :D.
Je noterai également toutes ces modifications dans le rapport ;)