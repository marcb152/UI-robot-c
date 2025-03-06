# Projet robot Systèmes Embarqués

## Fonctionnalités

- Interface GTK-3 (client)
- Echange client/serveur via socket
- Multithreading basique
- Serveur de commande du robot (version 2D Java)
- Ajout/suppression dynamique de `step`
- Chargement/sauvegarde sur le serveur

## Structure du projet

```js
RM6
 ├─ bin // Executables
 │  └─ path.txt
 │  └─ client // Executable client
 │  └─ server // Executable serveur
 └─ src
    ├─ Makefile // Makefile de compilation/nettoyage global
    ├─ README.md
    ├─ client
    │  ├─ Makefile // Makefile client
    │  ├─ client.c
    │  ├─ client.h
    │  ├─ main.c
    │  └─ ui
    │     ├─ ihm.c // Interface GTK3
    │     ├─ ihm.h
    │     ├─ ui_logic.c // Interface terminal (non-utilisée)
    │     └─ ui_logic.h
    ├─ common // Headers communs client/serveur
    │  ├─ pilot_common.h
    │  └─ socket_common.h
    ├─ server
    │  ├─ Makefile // Makefile serveur
    │  ├─ main.c
    │  ├─ robot_app // Contrôle du robot
    │  │  ├─ copilot.c
    │  │  ├─ copilot.h
    │  │  ├─ pilot.c
    │  │  ├─ pilot.h
    │  │  ├─ robot.c
    │  │  └─ robot.h
    │  ├─ server.c
    │  └─ server.h
    └─ utils.h
```

## Installation

Ce projet utilise le **Simulateur MrPizz version 2D (Java) sur le port 12345**.

> [!CAUTION]
> Il est nécessaire de lancer les programmes dans l'ordre suivant:
>   1. Simulateur robot MrPizz Java
>   2. Serveur
>   3. Client

## Auteurs

- [@Marc](https://github.com/marcb152)
- [@Tom](https://github.com/TomSilfCreative)
- [@Copilot](https://github.com/features/copilot)
