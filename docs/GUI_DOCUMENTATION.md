# Comment fonctionne le **Rendu graphique** sur notre Zappy ?

Le GUI (`zappy_gui`) est un client graphique 3D écrit en C++20 avec [raylib 5.5](https://www.raylib.com/). Il se connecte au serveur Zappy en TCP, rejoue le protocole texte du serveur pour reconstruire l'état de la partie (carte, ressources, joueurs, œufs) et l'affiche en 3D, avec une caméra libre, du son et des particules.

## 1. Connexion au serveur

Au lancement, **aucun argument CLI n'est requis** : `./zappy_gui` ouvre directement le menu, qui propose deux champs pré-remplis :
- Host : `127.0.0.1`
- Port : `4242`

### Établissement de la connexion (`network/`)

1. `network_client::connectTo(host, port)` (src/gui/network/network_client.cpp) résout l'adresse via `getaddrinfo`, ouvre un socket TCP non bloquant avec un timeout de connexion de 2s, puis bascule le polling en lecture ligne par ligne.
2. Une fois connecté, `connection_manager` gère le handshake attendu par le serveur Zappy :
   - Le serveur envoie `WELCOME`.
   - Le GUI répond `GRAPHIC\n` pour s'identifier comme client graphique (et non un client IA).
3. Si la connexion réussit, `application::_inGame` passe à `true` et la boucle bascule sur `updateGame()`.

### Commandes du protocole serveur interprétées par `gamestate`

| Commande | Effet |
|---|---|
| `msz <w> <h>` | Taille de la carte |
| `tme <t>` | Unité de temps serveur |
| `tst <x> <y> <r0..r6>` | Quantités de ressources sur une tuile |
| `teg <nom>` | Déclaration d'une équipe |
| `pnw #n <x> <y> <ori> <niv> <équipe>` | Apparition d'un joueur |
| `ppo #n <x> <y> <ori>` | Déplacement / orientation d'un joueur |
| `plv #n <niv>` | Changement de niveau |
| `pin #n <r0..r6>` | Inventaire d'un joueur |
| `pdi #n` | Mort d'un joueur |
| `enw #n <x> <y>` | Apparition d'un œuf |
| `ebo #n` | Éclosion d'un œuf |
| `seg <équipe>` | Fin de partie / équipe gagnante |

`connection_manager` déclenche en parallèle des effets audio/visuels sur ces événements (voir §5).

## 2. Rendu 3D (`renderer`)

Le rendu utilise une `Camera3D` raylib (FOV par défaut 45°) et des modèles `.glb` chargés depuis `src/gui/models/` :

- `ingot.glb`, `bread.glb`, `bean.glb`, `diamond.glb` - ressources sur les tuiles
- `villager.glb` - joueurs
- `egg.glb` - œufs
- `grass.glb`, `island.glb` - sol / décor

Le `renderer` dessine, à chaque frame :
- la grille de tuiles et les ressources posées dessus,
- les joueurs (label de niveau au-dessus, masquable avec **L**),
- les œufs,
- un ciel et un sol dont la teinte varie (jour/nuit) selon une phase de cycle, avec un soleil/une lune en orbite,
- le temps écoulé, ainsi que les écrans de victoire/défaite en fin de partie.

## 3. Caméra et contrôles

| Touche / action | Effet |
|---|---|
| `Z` / `S` | Avancer / reculer |
| `Q` / `D` | Aller à gauche / droite |
| `A` / `E` | Monter / descendre |
| `X` | Verrouiller/déverrouiller la souris pour orienter la vue |
| Souris (verrouillée) | Rotation de la caméra |
| Molette souris | Zoom (modifie le FOV, borné entre 10° et 90°) |
| Boutons `+`/`-` (coin haut-droit) | Zoom alternatif à la souris |
| `TAB` | Afficher/masquer le panneau liste d'équipes (`TeamList`) |
| `L` | Afficher/masquer les labels de niveau des joueurs |
| Bouton `S` (cliquable souris) | Ouvrir/fermer le panneau de réglages |

Tant que le panneau de réglages est ouvert, la caméra et la liste d'équipes ne réagissent plus aux entrées (`application::updateGame`).

## 4. Panneau de réglages (`settings`)

Panneau modal (760x700) affichant :
- les contrôles disponibles (rappel clavier),
- un curseur de sensibilité souris (0–500, valeur par défaut 319),
- un toggle plein écran,
- des curseurs de volume séparés pour la musique et les effets sonores,
- des boutons +/- pour ajuster la fréquence serveur (envoie la commande `sst` au serveur).

## 5. Audio (`AudioManager`)

Géré via le système audio raylib :
- Musique d'ambiance en streaming (`ambient.ogg`, volume 0.4 par défaut), rejouée en boucle.
- 10 effets sonores ponctuels : Spawn, Death, EggLay, EggCrack, Food (PickUp/Drop), LevelUp, Incantation, IncantationFail.
- Volume global mutable, volume musique indépendant, réglables depuis le panneau de réglages.

Le déclenchement des sons est piloté par `connection_manager` en fonction des événements reçus du serveur (ex. `pnw` → Spawn, `pdi` → Death, `pic` → début d'incantation + particules violettes, `pie` → succès d'incantation + particules dorées, `plv` → LevelUp, `pgt`/`pdr` → PickUp/Drop).

## 6. Build et lancement

```bash
cmake -B build -S .
cmake --build build -j$(nproc)
```

Le binaire `zappy_gui` est produit à la racine du dépôt. Le lancer sans argument :

```bash
./zappy_gui
```