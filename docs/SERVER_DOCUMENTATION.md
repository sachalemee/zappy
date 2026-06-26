# Comment fonctionne le **Serveur Zappy** ?

Le serveur (`zappy_server`) est le cœur du projet. Il est responsable de la gestion du monde, des joueurs, des ressources et de l'exécution des commandes. Tous les clients (IA et GUI) passent par lui, ce qui garantit que l'état du jeu reste unique et synchronisé.

Développé en **C**, le serveur repose sur une architecture **monothread** utilisant `poll()` pour gérer plusieurs connexions TCP simultanément, conformément au sujet.

## 1. Démarrage du serveur

Le serveur se lance avec les paramètres suivants :

```bash
./zappy_server -p port -x width -y height -n team1 team2 ... -c clientsNb -f freq
```

| Option      | Description                                         |
| ----------- | --------------------------------------------------- |
| `-p`        | Port TCP d'écoute                                   |
| `-x` / `-y` | Dimensions de la carte                              |
| `-n`        | Noms des équipes                                    |
| `-c`        | Nombre de slots disponibles au lancement par équipe |
| `-f`        | Fréquence du serveur (vitesse du jeu)               |

La fréquence influence directement la durée des actions :

```
durée = coût de l'action / fréquence
```

Par exemple, une commande `Forward` coûte **7 unités de temps**. Avec une fréquence de **100**, elle dure **70 ms**.

---

## 2. Architecture réseau

Le serveur fonctionne sur **un seul thread** et utilise `poll()` pour gérer toutes les connexions réseau.

La boucle principale surveille :

* le socket d'écoute pour accepter les nouveaux clients ;
* les sockets des IA ;
* les sockets du GUI.

Chaque client possède un buffer de réception. Les données sont accumulées jusqu'à la réception d'un caractère `\n`, ce qui permet de reconstruire correctement les commandes même lorsqu'elles arrivent en plusieurs paquets TCP.

Lorsqu'un client se connecte, le serveur envoie :

```text
WELCOME
```

Le client peut ensuite :

* envoyer le nom d'une équipe pour rejoindre une partie ;
* envoyer `GRAPHIC` afin de devenir le client graphique.

---

## 3. Gestion des commandes

Le serveur fonctionne en **temps réel simulé**.

Chaque joueur possède une file pouvant contenir jusqu'à **10 commandes**. Les commandes sont exécutées dans leur ordre d'arrivée.

Lorsqu'un joueur est libre :

1. la première commande est lue ;
2. sa durée d'exécution est calculée ;
3. le joueur est marqué comme occupé.

Une fois le délai écoulé, l'action est appliquée, les événements GUI sont envoyés et la commande est retirée de la file.

Ainsi, une commande longue ne bloque jamais le reste du serveur.

---

## 4. Joueurs, nourriture et œufs

Chaque joueur commence avec **10 unités de nourriture**, soit **1260 unités de temps** de survie.

Toutes les **126 unités de temps**, une unité de nourriture est consommée. Si le joueur n'a plus de nourriture, il reçoit `dead` puis est supprimé de la partie.

La commande **Fork** crée un nouvel œuf appartenant à l'équipe du joueur. Lorsqu'un nouveau client rejoint cette équipe, un œuf disponible est utilisé pour faire apparaître le nouveau joueur.

En cas de déconnexion brutale, toutes les ressources associées au joueur sont correctement libérées afin d'éviter les joueurs fantômes ou les fuites mémoire.

---

## 5. La carte et les ressources

Le monde de Zappy est un **tore** : lorsqu'un joueur quitte la carte par un bord, il réapparaît du côté opposé. Cette logique s'applique également à la vision (`Look`) et aux messages (`Broadcast`).

Les ressources sont générées au lancement puis toutes les **20 unités de temps** afin de conserver les densités définies par le sujet.

| Ressource | Densité |
| --------- | ------- |
| Food      | 0.50    |
| Linemate  | 0.30    |
| Deraumere | 0.15    |
| Sibur     | 0.10    |
| Mendiane  | 0.10    |
| Phiras    | 0.08    |
| Thystame  | 0.05    |

---

## 6. Les incantations

Les joueurs peuvent monter de niveau grâce à une **incantation**.

Pour réussir, plusieurs conditions doivent être réunies :

* tous les joueurs doivent être sur la même case ;
* ils doivent être du même niveau ;
* les ressources nécessaires doivent être présentes sur la tuile.

Pendant toute la durée de l'incantation, les joueurs concernés sont immobilisés.

Si les conditions sont toujours valides à la fin de l'action, les pierres sont consommées, tous les participants gagnent un niveau et le GUI est immédiatement informé.

---

## 7. Interface graphique (GUI)

Le GUI est un client observateur qui reçoit l'intégralité des événements du serveur.

Lors de sa connexion (`GRAPHIC`), le serveur lui envoie immédiatement :

* la taille de la carte (`msz`) ;
* la fréquence (`sgt`) ;
* les équipes (`tna`) ;
* toutes les tuiles (`bct`) ;
* les joueurs (`pnw`) ;
* les œufs (`enw`).

Pendant toute la partie, chaque action importante génère les événements correspondants (`ppo`, `pgt`, `pdr`, `pex`, `pfk`, `pic`, `pie`, `plv`, `pdi`, `seg`...), ce qui permet au GUI de rester parfaitement synchronisé avec le serveur.

---

## 8. Condition de victoire

À chaque itération de la boucle principale, le serveur vérifie le niveau des joueurs.

La partie se termine dès que **6 joueurs d'une même équipe atteignent le niveau 8**.

Le serveur envoie alors :

```text
seg <NomEquipe>
```

au GUI, ferme proprement toutes les connexions réseau et libère l'ensemble des ressources avant de quitter.