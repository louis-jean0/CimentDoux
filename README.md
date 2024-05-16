## Instructions pour la compilation et l'exécution du jeu

1. **Cloner le dépôt :**
   ```
   $ git clone <url_du_dépôt>
   $ cd moteur
   $ mkdir build
   $ cd build
   $ cmake ..
   $ make
   $ ./main 
   ```
-Tout d'abord vous devez clone le dépot 
-Ensuite placer vous dans le dossier moteur avec la commande cd moteur
-taper la commande mkdir build
-ensuite placer vous dans ce dossier avec la commande cd build
-maintenant taper la commande cmake ..
-ensuite faite make (conseil: ne compilez pas avec make -j pour la première compilation mais prenez la moitié des threads possibles dans votre ordinateur)
-enfin vous pouvez executer le jeu avec la commande ./main
