# Rapport — Level3

## Partie mandatory

### Observation initiale
Lors de son exécution, le binaire demande un mot de passe.  
Si la valeur saisie est correcte, il affiche **"Good job"**, sinon **"Nope"**.  

L’objectif est d’analyser le binaire pour :
- Comprendre la méthode de vérification.
- Retrouver la valeur attendue.
- Reproduire la logique en C.

### Méthodologie
Nous avons procédé en plusieurs étapes :
1. **Désassemblage du binaire** avec `radare2` et `objdump`.
2. **Suivi de l’exécution** avec `gdb` pour observer les registres et la mémoire.
3. **Identification de la comparaison clé** qui décide du succès ou de l’échec.

### Analyse du code assembleur
Le binaire effectue un traitement sur l’entrée utilisateur, puis une série de vérifications.  
La différence par rapport aux niveaux précédents :
- Une comparaison (`cmp`) est effectuée **entre deux registres censés contenir des valeurs distinctes** : l’un avec la donnée de l’utilisateur, l’autre avec une valeur calculée.
- Le résultat de ce `cmp` est utilisé dans un saut conditionnel (`je` ou `jne`) pour décider si le programme va vers la branche "Good job" ou "Nope".

En lisant attentivement l’ASM, nous avons identifié le `cmp` clé.  
Normalement, il compare deux valeurs différentes, ce qui mène au saut vers l’échec si elles ne correspondent pas.

### Récupération du mot de passe
En plaçant un point d’arrêt avant le `cmp` final, nous avons inspecté les registres pour comprendre quelles valeurs étaient comparées.  
Cette étape permet de voir la transformation appliquée à l’entrée et de retrouver la valeur attendue.

---

## Bonus — Patch du binaire

Pour contourner la vérification et forcer la réussite, nous avons appliqué un patch minimal :

### 1. Modification du `cmp`
Nous avons modifié l’instruction `cmp` pour qu’elle compare **le même registre avec lui-même** (`cmp eax, eax` au lieu de `cmp eax, ecx`).  
De cette façon, la comparaison renvoie toujours égalité (`ZF = 1`).

### 2. Effet sur le saut conditionnel
Puisque le `cmp` est toujours vrai, le saut conditionnel (`je`) prend systématiquement la branche menant au bloc **"Good job"**, peu importe l’entrée saisie.

### 3. Résultat
Dès que le binaire atteint cette comparaison finale, il considère la vérification comme réussie, et affiche **"Good job"**.

---

### Conclusion
Pour Level3, le patch s’est limité à :
- Modifier un `cmp` pour comparer un registre à lui-même.
- Forcer le saut conditionnel vers la branche de succès.

Cette méthode est plus discrète qu’un patch direct du `jmp`, car elle conserve la structure originale du flux d’exécution tout en garantissant que la condition de succès est toujours remplie.

