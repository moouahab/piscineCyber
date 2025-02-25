# 🕷️ Scorpion - Web Scraping et Métadonnées  

## 📌 Introduction  

Le projet **Arachnida** explore deux concepts essentiels :  
- **Le Web Scraping** : Automatisation de la collecte d’images sur un site web.  
- **Les Métadonnées** : Analyse et modification des informations cachées dans les fichiers.  

Ces notions sont largement utilisées en **cybersécurité**, **investigation numérique** et **gestion de données**.  

---

## 🕷️ Qu'est-ce que le Web Scraping ?  

Le **Web Scraping** est une technique permettant d’extraire automatiquement des données depuis un site web. Il est utilisé pour :  
✅ **Récupérer du contenu** (articles, images, vidéos).  
✅ **Analyser et structurer des données** issues de plusieurs sources.  
✅ **Automatiser des tâches** comme la veille technologique.  

**Exemple concret :**  
Les moteurs de recherche comme **Google** utilisent des robots (**crawlers**) pour **indexer** le web et afficher des résultats pertinents.  

---

## 🕷️ Le Web Scraping dans Arachnida - Spider  

Nous allons implémenter **Spider**, un programme capable de :  
- **Explorer un site web** et télécharger toutes ses images.  
- **Gérer la profondeur de l’exploration** (`-l`).  
- **Sauvegarder les fichiers** téléchargés (`-p`).  

### 🚀 Pourquoi apprendre le Web Scraping ?  
✅ **Collecter de l’information efficacement**.  
✅ **Utilisation en veille concurrentielle et OSINT** (*Open Source Intelligence*).  
✅ **Approfondir la gestion des réseaux et fichiers**.  

---

## 🦂 Qu'est-ce que les métadonnées ?  

Les **métadonnées** sont des **informations sur des données**. Elles sont souvent intégrées dans des fichiers comme des images, documents ou vidéos.  

### 📷 Exemple de métadonnées EXIF d’une image :  
- 📅 **Date de prise de vue** : 2023-06-10 15:30  
- 📍 **Coordonnées GPS** : 48.8566° N, 2.3522° E *(Paris)*  
- 📸 **Appareil utilisé** : Canon EOS 5D Mark IV  
- ⚙️ **Paramètres** : ISO 100, Ouverture f/2.8, Vitesse 1/500 sec  

### 🛡️ Métadonnées et Cybersécurité  
✅ **Investigation numérique** → Identifier qui a pris une photo et où.  
✅ **Protection de la vie privée** → Supprimer les données GPS avant de partager une image.  
✅ **Forensique numérique** → Analyser l’origine et l’historique d’un fichier.  

---

## 🦂 Analyse et modification des métadonnées - Scorpion  

Après avoir récupéré des images avec **Spider**, nous utiliserons **Scorpion** pour :  
✅ **Lire et afficher les métadonnées EXIF**.  
✅ **Modifier ou supprimer certaines métadonnées**.  
✅ **Offrir une interface graphique (bonus)** pour une gestion interactive.  

### 🚀 Pourquoi c'est utile ?  
✅ **Contrôle des données personnelles** avant publication.  
✅ **Analyse et investigation numérique**.  
✅ **Optimisation des fichiers multimédias** (nettoyage, anonymisation).  

---

## 📂 Installation et Compilation  

### 📦 Dépendances  
Assurez-vous d’avoir installé les bibliothèques suivantes :  
- **Exiv2** (gestion des métadonnées EXIF)  
- **Qt5** (interface graphique)  
- **CMake** (optionnel)  

### 🔧 Installation sous Linux (Ubuntu/Debian)
```sh
sudo apt update
sudo apt install g++ cmake qtbase5-dev libexiv2-dev
```

### 💻 Compilation du programme en mode terminal
```sh
g++ -o scorpion main.cpp -lexiv2
```

### 💻 Compilation avec l’interface graphique (Qt)
```sh
qmake && make
./scorpion
```
### 1️⃣ Afficher les métadonnées d’une image

```sh
./scorpion image.jpg
```

### 2️⃣ Modifier une métadonnée
```sh
    ./scorpion -m Exif.Photo.UserComment="Mon commentaire" image.jpg
```

### 3️⃣ Supprimer toutes les métadonnées EXIF

```sh
./scorpion -d image.jpg
```

## 🎨 Interface Graphique (GUI)
Lancez simplement :
```
    ./scorpion_gui
```
L’interface permet de :
 - Charger une image et afficher ses métadonnées.
 - Modifier les valeurs directement dans une table.
 - Sauvegarder les changements.


## 📜 Licence  

Projet développé dans un cadre éducatif.  
Utilisation libre sous licence **MIT**.  

---

## 👨‍💻 Auteurs  

- **Ouahab Mohamed** - Développement C++ - UI/UX Qt  

---

## 🎯 Prochaines améliorations possibles  

🚀 Ajouter un bouton pour **supprimer une seule métadonnée**.  
🚀 Améliorer **l'affichage des métadonnées** (filtrer, trier).  
🚀 Afficher une image avec **QLabel** pour voir le fichier ouvert.  

---

## 📩 Feedback  

Si vous avez des suggestions ou des bugs à signaler, contactez-moi ! 🚀  
