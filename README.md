## Instructions pour la compilation et l'exécution du jeu

1. **Cloner le dépôt :**
   ```
   $ git clone "https://github.com/louis-jean0/HAI819I-moteurjeux"  
   ```

2. **Compiler :** 
```
   $ mkdir build
   $ cd build
   $ cmake ..
   $ make
```
### - <span style="color: red;">conseil: ne compilez pas avec make -j pour la première compilation mais prenez la moitié des threads possibles dans votre ordinateur</span>

3. **Jouer :** 
	```
    $ ./main 
    ```

4. **Potentiel Problèmes :**

Si votre ordinateur n'est pas très performant on vous conseille de remplacer le fichier src/scene.cpp par celui dans le dossier ordi_lent.
Si au moment de compiler il y a des problèmes avec glm on vous conseille de remplacer le CMakelIsts.txt par celui dans le dossier ordi_lent.