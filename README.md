# CEPR — Conseil Electoral Provisoire
Projet Algorithmes et Structures de Donnees — gestion des elections presidentielles.

## Structure du dossier

```
CEPR/
├── raylib/            Bibliotheque Raylib (deja incluse, rien a installer)
│   ├── include/        raylib.h
│   └── lib/             libraylib.a
├── toolchain/          Compilateur GCC/MinGW embarque (deja inclus, rien a installer)
│   └── bin/              gcc.exe, mingw32-make.exe, etc.
├── src/                Tous les fichiers source du projet
│   ├── election.h        Structures, types et constantes globales
│   ├── modules.h          Prototypes des fonctions de chaque module
│   ├── ge_parti.c          Module Partis Politiques
│   ├── ge_BV.c              Module Bureaux de Vote
│   ├── ge_elect.c            Module Electeurs
│   ├── ge_candi.c              Module Candidats
│   ├── ge_votes.c                Module Votes
│   ├── rapp.c                      Module Rapports et Resultats
│   └── velectro.c                    Menu general + main (interface Raylib)
├── Makefile            Compile automatiquement tous les .c de src/ en CEPR.exe
└── CEPR.exe            Genere apres compilation (n'existe pas au premier clone)
```

Rien a installer : Raylib et le compilateur GCC sont deja inclus dans le
depot. Il suffit de cloner et de compiler.

## Compiler en ligne de commande (fonctionne partout, sans IDE)

Dans une invite de commandes, a la racine du dossier `CEPR/` :

```
toolchain\bin\mingw32-make.exe
```

Ca produit `CEPR.exe`. Pour nettoyer avant de recompiler :
```
toolchain\bin\mingw32-make.exe clean
```

## Compiler avec Visual Studio 2026

Le projet est deja configure (`CEPR_VS2026.slnx`) pour appeler le Makefile
via `mingw32-make.exe` embarque dans `toolchain/`, avec des chemins relatifs
au projet — aucune adaptation necessaire, meme si vous clonez le depot
ailleurs sur votre disque. Ouvre simplement `CEPR_VS2026.slnx` et fais
**Build → Rebuild Solution**.

## Compiler avec Code::Blocks

1. **File → New → Project → Makefile** (pas "Empty Project")
2. Pointe vers le `Makefile` a la racine de `CEPR/`
3. Ajoute les fichiers `src/*.c` et `src/*.h` au projet pour les editer
   (**Project → Add files...**)
4. Verifie dans **Project → Build options → Search directories** que
   `raylib/include` figure bien (normalement deja gere par le Makefile,
   cette etape sert surtout a l'auto-completion de Code::Blocks)
5. **Build → Build** (F9)

Si ton compilateur n'est pas dans `C:\raylib\w64devkit\bin`, modifie la
variable `CC` en haut du `Makefile` pour pointer vers ton propre `gcc.exe`.

## Ajouter un nouveau fichier .c

Place-le simplement dans `src/`. Le Makefile le detecte automatiquement
(`wildcard src/*.c`) — aucune configuration a modifier, ni dans le
Makefile, ni dans VS, ni dans Code::Blocks.
