# Rapport — Level2

## Partie mandatory

### Observation initiale
À l’exécution, le binaire demande à l’utilisateur un mot de passe.  
Si la valeur saisie est correcte, il affiche **"Good job"**, sinon **"Nope"**.  
L’objectif de la partie obligatoire est donc :
- Comprendre comment le mot de passe est vérifié.
- Retrouver la valeur attendue.
- Reproduire le fonctionnement en C.

### Méthodologie
Nous avons découpé la démarche en quatre étapes :
1. **Analyse du code assembleur** pour repérer les comparaisons et appels critiques.
2. **Identification des adresses** où sont effectuées les vérifications.
3. **Observation à l’exécution** pour comprendre la logique de contrôle.
4. **Récupération du mot de passe** en mémoire.

Le désassemblage (`objdump`, `radare2`) et le débogage (`gdb`) montrent que :
- Le programme vérifie d’abord séparément les **deux premiers caractères** de l’entrée avec deux `cmp`.
- Ensuite, un appel à `strcmp` compare l’ensemble de la chaîne à la référence interne.

En suivant l’exécution pas à pas avec GDB, nous avons retrouvé les pointeurs des deux chaînes passées à `strcmp`.  
La valeur attendue correspond au mot de passe interne utilisé par la comparaison finale.  
Une exécution avec ce mot de passe valide confirme l’affichage **"Good job"**.

---

## Bonus — Patch du binaire

Pour automatiser la réussite sans fournir le vrai mot de passe, nous avons modifié le binaire à trois endroits clés :

### 1. Neutralisation des deux premières vérifications
Les deux `cmp` qui vérifient le 1er et le 2e caractère ont été modifiés pour comparer **le même registre à lui-même** (`cmp eax, eax`).  
Ainsi, ces conditions sont **toujours vraies** et laissent passer l’exécution vers les étapes suivantes.

### 2. Modification de la comparaison finale (`strcmp`)
Comme dans le **Level1**, nous avons intercepté la valeur de retour de `strcmp`.  
Juste après son appel, nous avons forcé `eax` à zéro (`xor eax, eax`), simulant un retour “chaînes égales”.

### 3. Résultat
Peu importe l’entrée saisie, les trois vérifications passent :
- Les deux premières ne bloquent plus.
- La comparaison finale est forcée à un succès.

Le programme affiche donc systématiquement **"Good job"**, sans qu’il soit possible de deviner depuis l’extérieur que le binaire a été modifié.

---

### Conclusion
Ce patch contourne toutes les vérifications du programme en :
- Neutralisant les comparaisons initiales.
- Forçant la réussite du contrôle final.
Cette méthode permet de valider le niveau même sans connaître le mot de passe original, tout en conservant une structure du code proche de l’original.

