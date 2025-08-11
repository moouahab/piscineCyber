# Level1 — Reverse Engineering

## Partie mandatory

### Observation initiale
Lors de l’exécution, le binaire demande un mot de passe :  
- Si la valeur saisie est correcte → affichage de `Good job`  
- Sinon → affichage de `Nope`  

L’objectif est de comprendre la logique de vérification du mot de passe, de retrouver cette valeur, et de reproduire le comportement en C.

---

### Méthodologie
1. **Analyse du code assembleur** : repérer les appels critiques (`scanf`, `strcmp`).
2. **Identification des adresses** liées à la comparaison.
3. **Observation en mémoire** juste avant la comparaison.
4. **Extraction du mot de passe** et validation.

---

### Analyse du code assembleur
- Le programme lit l’entrée utilisateur via `scanf`.
- Appelle `strcmp` pour comparer la saisie (`s1`) avec une référence interne (`s2`).
- `cmp eax, 0` puis `jne` → saute vers la branche d’échec si différent de 0.  
- Si `strcmp` retourne 0 (égalité), le saut n’est pas pris → exécution de la branche succès (`Good job`).

---

### Récupération du mot de passe
Sous GDB :
- **Breakpoint** juste avant l’appel à `strcmp`.
- Inspection des registres :  
  - `ecx` → pointeur sur la saisie utilisateur.
  - `edx` → pointeur sur la chaîne de référence.
- En lisant la mémoire à l’adresse pointée par `edx` : `__stack_check`.  

Test du binaire avec `__stack_check` → message `Good job`.

---

### Conclusion mandatory
L’approche (lecture ASM + inspection registres + extraction valeur) permet d’identifier et de prouver le mot de passe attendu.  
Cette analyse répond aux attentes : compréhension du binaire, récupération du mot de passe valide, reproduction en C.

---

## Bonus — Patch du binaire

### Objectif
Faire afficher `Good job` quel que soit le mot de passe saisi.

### Étapes
1. **Analyse avec Radare2** du code assembleur.
2. Identification d’un `cmp` suivi d’un `jne 0x1260` (branche échec).
3. Plutôt que de remplacer par un `jmp` (trop évident), modification de la **cible du saut**.

### Patch appliqué
- Modification de `jne 0x1260` → `jne 0x124a`.
- Résultat : le saut atterrit directement dans la branche succès, contournant la vérification.
- Avantage : logique de contrôle conservée en apparence, patch discret.

---

### Conclusion bonus
Ce patch force la réussite sans changer drastiquement la structure du programme, et évite les signatures typiques d’un contournement grossier.

