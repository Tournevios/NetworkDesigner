# NetworkDesigner — Functional Specification

> **Purpose:** Rétro-documentation iso-fonctionnelle, base de référence pour la modernisation de la stack technique.  
> **Audience:** Développeurs, contributeurs, recetteurs de la migration.

---

## 1. Vue d'ensemble

**NetworkDesigner** est une application desktop Qt (C++) pour la conception, la simulation et l'analyse de réseaux de neurones à états discrets. Elle repose sur des dynamiques de type machine de Boltzmann.

### Fonctionnalités principales

| Domaine | Fonctionnalité |
|---|---|
| Conception | Création visuelle de neurones et synapses par glisser-déposer |
| Paramétrage | Poids, délais, seuils, température, nombre d'états par neurone |
| Planification | Schémas de mise à jour : Parallèle, Séquentiel, Blocs |
| Simulation | Attracteurs, bassins d'attraction, activité, diffusion |
| Persistance | Chargement/sauvegarde au format XML `.nml` |
| Export | Format Prolog GNBox Premodel |

### Architecture générale

```
┌─────────────────────────────────────────────────────────────┐
│                        Qt Main Window                        │
│  ┌─────────────┐  ┌──────────────────┐  ┌───────────────┐  │
│  │  DesignPlan │  │ EvenementHandler │  │  Panels (UI)  │  │
│  │  (Canvas)   │  │ (Slots/Signals)  │  │  Spinboxes… │  │
│  └──────┬──────┘  └────────┬─────────┘  └───────────────┘  │
│         │                  │                                  │
│  ┌──────▼──────────────────▼────────────┐                   │
│  │              Network Model            │                   │
│  │  Network → [Neuron → [Synapse]]       │                   │
│  │  UpdateSchedulingPlan → [UpdateBlock] │                   │
│  └──────────────────┬────────────────────┘                   │
│                     │                                         │
│  ┌──────────────────▼────────────────────┐                   │
│  │              Computer                  │                   │
│  │  computeP / computeS / computeBS / BP │                   │
│  └──────────────────┬────────────────────┘                   │
│                     │                                         │
│  ┌──────────────────▼────────────────────┐                   │
│  │           Simulations                  │                   │
│  │  Attractors  Activity  Diffusion       │                   │
│  └────────────────────────────────────────┘                   │
└─────────────────────────────────────────────────────────────┘
```

Séparation **Modèle / Vue** via le système de signaux/slots Qt.

---

## 2. Modèle de données

### 2.1 Network

Conteneur racine de tous les neurones.

| Attribut | Type | Description |
|---|---|---|
| `neurons` | `vector<unique_ptr<Neuron>>` | Liste ordonnée (index = position) |
| `temperature` | `double` | Température globale du réseau |
| `uniformalTemperature` | `bool` | `true` → tous les neurones utilisent la température réseau |

**Méthodes clés :**
- `addNeuron(Neuron*)` — ajoute et auto-indexe
- `delNeuron(int index)` — supprime et met à jour toutes les références
- `getNeuron(int x, int y, int radius)` — recherche spatiale (événements souris)
- `getSynapse(int x, int y)` — hit-testing synapse
- `getState(bool*)` — export de l'état complet du réseau
- `drawMe(QPainter*, scale, transX, transY)` — rendu : synapses puis neurones

### 2.2 Neuron

Unité computationnelle avec dynamique multi-états.

| Attribut | Type | Description |
|---|---|---|
| `index` | `int` | Identifiant unique dans le réseau |
| `state` | `int` | État courant : `0` à `nbStates-1` |
| `nbStates` | `int` | Nombre d'états possibles (défaut : 2) |
| `threshold` | `vector<double>` | `threshold[i]` = seuil pour atteindre l'état `i+1` |
| `temperature` | `double` | Température individuelle (si non uniforme) |
| `nodeID` | `string` | Identifiant utilisateur pour les exports |
| `synapses` | `vector<unique_ptr<Synapse>>` | Synapses entrantes |
| `hasANewState` | `bool` | Flag : un nouvel état a été calculé |
| `theNewState` | `int` | Prochain état (en attente de `substitute()`) |
| `x, y` | `double` | Position sur le canvas |
| `selected` | `bool` | Sélection UI |
| `yellowMe` | `bool` | Appartenance au bloc de mise à jour courant |

**Gestion des seuils :**
- État 0 : seuil effectif = `-1000.0` (toujours atteignable)
- État `i > 0` : `getThreshold(i)` retourne `threshold[i-1]`

**Couleurs de rendu :**
| État | Couleur de remplissage |
|---|---|
| 0 | Rouge |
| 1 | Vert |
| ≥ 2 | Vert foncé |

Bordure : Bleu (sélectionné) / Jaune (bloc actif) / Noir (normal)

### 2.3 Synapse

Connexion dirigée entre deux neurones.

| Attribut | Type | Description |
|---|---|---|
| `baseNeuron` | `Neuron*` | Neurone source |
| `finalNeuron` | `Neuron*` | Neurone cible |
| `weight` | `double` | Force de la connexion (peut être négatif) |
| `delay` | `int` | Délai en pas de temps (0 = immédiat) |
| `states` | `queue<int>` | File FIFO pour implémenter le délai |
| `gExcentricity` | `float` | Décalage du point de contrôle de Bézier |
| `cx, cy` | `double` | Coordonnées du point de contrôle |

**Mécanisme de délai :**
```
getStateOfTheFinalNeuron():
  states.push(finalNeuron->state)
  if states.size() > delay:
    return states.pop_front()   // état d'il y a 'delay' pas
  else:
    return 0                    // file pas encore remplie
```

**Rendu :**
- Auto-synapse : ellipse autour du neurone source
- Autre synapse : courbe de Bézier quadratique avec flèche
- Couleur : Bleu (sélectionné) / Rouge (poids négatif) / Noir (positif)
- Style : Continu (delay=0) / Pointillé (delay>0)

**Formule Bézier :**
```
cx = (x1 + x2) * (0.5 + gExcentricity)
cy = (y1 + y2) * (0.5 + gExcentricity)
gExcentricity = +0.06 si baseIndex < finalIndex, sinon -0.06
```

### 2.4 UpdateSchedulingPlan & UpdateBlock

**UpdateSchedulingPlan** : conteneur ordonné de blocs de mise à jour.

**UpdateBlock** : groupe de neurones mis à jour ensemble selon une méthode.

| Attribut | Type | Description |
|---|---|---|
| `neuronsIndexs` | `vector<int>` | Indices des neurones du groupe |
| `UpdateMethods` | `int` | Méthode de mise à jour (constante ci-dessous) |

**Méthodes de mise à jour (constFile.h) :**

| Constante | Valeur | Comportement |
|---|---|---|
| `COMPUTE` | 0 | Calcul normal (Boltzmann) |
| `FIXE` | 1 | Non utilisé |
| `FIXE_1` | 2 | Forcer état 1 |
| `FIXE_0` | 3 | Forcer état 0 |
| `FIXE_01` | 4 | Alterner : 0, 1, 0, 1, … |
| `FIXE_10` | 5 | Alterner : 1, 0, 1, 0, … |
| `RANDOMLY` | 6 | État aléatoire à chaque pas |

---

## 3. Algorithmes de simulation

### 3.1 Calcul d'état d'un neurone (Boltzmann)

**Entrées :**
- États courants des neurones voisins (avec délais appliqués)
- Poids des synapses entrantes
- Seuils du neurone
- Température `T`

#### Cas déterministe (T = 0)

```
pour chaque état i de 0 à nbStates-2 :
  sum[i] = Σ(weight[j] * delayed_state[j]) - threshold[i]

nouvel_état = 0
pour i de 0 à nbStates-2 :
  si sum[i] >= 0 ET (i == nbStates-2 OU sum[i+1] < 0) :
    nouvel_état = i+1
    arrêt
```

Le neurone monte jusqu'au plus haut état dont le seuil est franchi.

#### Cas stochastique (T > 0)

```
boltzmannTerm[i] = 1 / (1 + exp(-sum[i-1] / T))

Direction bias (conditions aux bords) :
  état == 0          → prob_montée = 0.1
  état == nbStates-1 → prob_montée = 0.9
  sinon              → prob_montée = aléatoire(0,1)

Probabilités de transition construites par produit de termes de Boltzmann.
Sélection par roulette (tirage aléatoire sur la distribution).
```

**Propriétés :**
- T=0 : déterministe, suit le paysage énergétique
- T→∞ : uniforme aléatoire
- Implémente une distribution de Boltzmann discrète

### 3.2 Schémas de mise à jour (Computer)

**Synchrony rate** : probabilité qu'un neurone calcule à chaque pas (défaut : 1.0).

#### Parallèle (computeP)
```
pour chaque itération :
  pour chaque neurone : si aléatoire() <= syncRate → compute()
  pour chaque neurone : substitute()          // application simultanée
  émettre tick()
```
Tous les neurones voient le même état précédent avant mise à jour.

#### Séquentiel (computeS)
```
pour chaque itération :
  pour chaque neurone :
    si aléatoire() <= syncRate → compute() puis substitute()  // immédiat
  émettre tick()
```
Chaque neurone voit les mises à jour des neurones précédents dans la séquence.

#### Blocs Séquentiels (computeBS)
```
pour chaque itération :
  pour chaque bloc dans UpdateSchedulingPlan :
    si méthode == COMPUTE :
      pour chaque neurone du bloc : compute()
      pour chaque neurone du bloc : substitute()
    sinon :
      appliquer état fixe/aléatoire selon méthode
  émettre tick()
```
Les blocs s'exécutent séquentiellement, les neurones au sein d'un bloc en parallèle.

### 3.3 Analyse des attracteurs et bassins d'attraction

**Objectif :** Partitionner l'espace d'états complet en attracteurs (cycles) et leurs bassins (états qui y convergent).

**Complexité :** O(∏ nbStates[i]) — exponentielle en nombre de neurones. Limite pratique ≈ 10–15 neurones binaires.

#### Algorithme principal

```
totalStates = ∏(nbStates[i])
visités = 0

tant que visités < totalStates :
  état = état_non_visité_aléatoire()
  si état déjà dans un bassin connu → marquer visité, continuer

  trajectoire = []
  courant = état

  répéter jusqu'à max_iterations (2000) :
    trajectoire.ajouter(courant)
    suivant = calculer_suivant(courant)

    si suivant dans trajectoire :
      // Cycle détecté → attracteur
      attracteur = trajectoire[index(suivant):]
      bassin = exploration_arrière(attracteur)
      stocker(attracteur, bassin)
      visités += bassin.cardinalité()
      arrêt

    courant = suivant
```

#### Exploration arrière (BFS inverse)

```
exploration_arrière(attracteur) :
  bassin = copie(attracteur)
  file = copie(attracteur)

  tant que file non vide :
    état = file.défiler()
    prédécesseurs = predecesseursDe(état)
    pour pred dans prédécesseurs :
      si pred pas dans bassin :
        bassin.ajouter(pred)
        file.enfiler(pred)

  retourner bassin
```

#### Calcul des prédécesseurs

```
predecesseursDe(état_cible) :
  résultat = ensemble_universel

  pour chaque neurone_i :
    solutions_i = résoudre(neurone_i, état_cible[i])
    résultat = résultat ∩ solutions_i

  retourner résultat

résoudre(neurone_i, état_cible) :
  // Énumère toutes les combinaisons d'états des voisins
  // Simule compute() pour chaque combinaison
  // Retourne les configurations menant à état_cible
```

### 3.4 États pseudo (représentation compressée)

Pour compresser les ensembles d'états, `NetworkState` utilise des pseudo-états négatifs :

| Valeur | Signification |
|---|---|
| `>= 0` | État discret exact |
| `-1` | Incohérent (aucun état valide) |
| `-n` (n>1) | Ensemble d'états : bit `i` mis = état `i` possible |

**Exemples :**
- `-3` = `0b11` = états {0, 1}
- `-7` = `0b111` = états {0, 1, 2}
- `-5` = `0b101` = états {0, 2}

**Opérations :**
```
union(-a, -b)        = -(a | b)     // OR bit-à-bit
intersection(-a, -b) = -(a & b)     // AND bit-à-bit (→ -1 si 0)
différence(-a, -b)   = -(a & ~b)    // ET-NOT bit-à-bit
```

---

## 4. Format de fichier NML

Format XML propriétaire, extension `.nml`.

### Structure générale

```xml
<?xml version="1.0"?>
<Configuration>
  <Network Temperature="…" UniformalTemperature="…" Nb_Neurons="…">
    <Neuron Index="…" NodeID="…" State="…" NbStates="…"
            Temperature="…" Nb_Neighbors="…" x="…" y="…">
      <State Index="1" Threshold="…"/>
      <!-- une balise State par état > 0 -->
      <Synapse NeighborIndex="…" Weight="…" Delay="…" CX="…" CY="…"/>
      <!-- une balise Synapse par connexion sortante -->
    </Neuron>
  </Network>
  <UpdateSchedulingPlan Nb_UpdateBlocks="…">
    <Block Size="…" Calculus="…">
      <NeuronIndex Index="…"/>
    </Block>
  </UpdateSchedulingPlan>
</Configuration>
```

### Attributs de référence

**`<Network>`**

| Attribut | Type | Description |
|---|---|---|
| `Temperature` | double | Température globale |
| `UniformalTemperature` | 0/1 | 1 = température réseau pour tous |
| `Nb_Neurons` | int | Nombre de neurones |

**`<Neuron>`**

| Attribut | Type | Description |
|---|---|---|
| `Index` | int | ID unique (ordre de création) |
| `NodeID` | string | Label utilisateur (défaut : `"noname"`) |
| `State` | int | État initial (0 à nbStates-1) |
| `NbStates` | int | Nombre d'états (défaut : 2) |
| `Temperature` | double | Température individuelle |
| `Nb_Neighbors` | int | Nombre de synapses sortantes |
| `x`, `y` | double | Position canvas |

**`<State>` (seuil)**

| Attribut | Type | Description |
|---|---|---|
| `Index` | int | Numéro d'état (1 à nbStates-1) |
| `Threshold` | double | Seuil d'activation |

Contrainte : exactement `NbStates - 1` éléments `<State>` par neurone.

**`<Synapse>`**

| Attribut | Type | Description |
|---|---|---|
| `NeighborIndex` | int | Index du neurone cible |
| `Weight` | double | Poids (positif = excitateur, négatif = inhibiteur) |
| `Delay` | int | Délai en pas de temps (≥ 0) |
| `CX`, `CY` | double | Point de contrôle Bézier |

Auto-synapse autorisée : `NeighborIndex == Index` du neurone parent.

**`<Block>`**

| Attribut | Type | Description |
|---|---|---|
| `Size` | int | Nombre de neurones dans le bloc |
| `Calculus` | int | Méthode (voir constantes COMPUTE/FIXE_*/RANDOMLY) |

### Exemple complet (2 neurones en boucle)

```xml
<?xml version="1.0"?>
<Configuration>
  <Network Temperature="0" UniformalTemperature="1" Nb_Neurons="2">
    <Neuron Index="0" NodeID="A" State="1" NbStates="2"
            Temperature="1" Nb_Neighbors="1" x="100" y="100">
      <State Index="1" Threshold="0.5"/>
      <Synapse NeighborIndex="1" Weight="1" Delay="0" CX="150" CY="100"/>
    </Neuron>
    <Neuron Index="1" NodeID="B" State="0" NbStates="2"
            Temperature="1" Nb_Neighbors="1" x="200" y="100">
      <State Index="1" Threshold="0.5"/>
      <Synapse NeighborIndex="0" Weight="1" Delay="1" CX="150" CY="100"/>
    </Neuron>
  </Network>
  <UpdateSchedulingPlan Nb_UpdateBlocks="1">
    <Block Size="2" Calculus="0">
      <NeuronIndex Index="0"/>
      <NeuronIndex Index="1"/>
    </Block>
  </UpdateSchedulingPlan>
</Configuration>
```

---

## 5. Interactions UI (DesignPlan)

### 5.1 Événements souris

| Bouton | Modificateur | Zone | Action |
|---|---|---|---|
| Gauche | Aucun | Zone vide | Créer un neurone |
| Gauche | Aucun | Neurone | Sélectionner (désélectionne les autres) |
| Gauche | Aucun | Drag neurone | Déplacer tous les neurones sélectionnés |
| Gauche | Aucun | Synapse | Basculer sélection de la synapse |
| Gauche | Ctrl | Neurone | Basculer dans la multi-sélection |
| Gauche | Shift* | Neurone | Basculer appartenance au bloc courant |
| Droit | Aucun | Neurone (appui) | Démarrer création de synapse |
| Droit | Aucun | Neurone (relâche) | Finaliser la synapse |
| Milieu | Aucun | Drag | Panoramique du canvas |
| Molette | Aucun | Haut | Zoom + (scale += 0.05, max 2.0) |
| Molette | Aucun | Bas | Zoom – (scale -= 0.05, min 0.0) |

*Shift uniquement si `currentUpdateBlock >= 0` (mode bloc actif)

### 5.2 Clavier

| Touche | Action |
|---|---|
| `Suppr` | Supprimer tous les neurones et synapses sélectionnés |

### 5.3 Signaux émis par DesignPlan

| Signal | Déclencheur |
|---|---|
| `neuronSelectedChanged(Neuron*)` | Sélection d'un neurone |
| `synapseSelectedChanged(Synapse*)` | Sélection d'une synapse |
| `blockModeInvoked(int)` | Ajout/retrait d'un neurone dans un bloc |
| `networkIsModified()` | Toute modification structurelle |
| `updateCalled()` | Mise à jour demandée |

---

## 6. Paramètres globaux et comportements

### 6.1 Température

| Valeur | Effet |
|---|---|
| `T = 0` | Déterministe : suit le paysage énergétique |
| `T > 0` | Stochastique : transitions de Boltzmann |
| `T → ∞` | Sélection d'état uniforme aléatoire |

Peut être globale (réseau) ou locale (par neurone) selon `uniformalTemperature`.

### 6.2 Taux de synchronie

Probabilité (entre 0 et 1) qu'un neurone soit mis à jour à chaque pas.

- `1.0` : entièrement synchrone (tous les neurones)
- `0.5` : en moyenne la moitié des neurones par pas
- `0.0` : réseau figé

### 6.3 Valeurs par défaut à la création

Chaque élément créé dans DesignPlan hérite des valeurs par défaut configurables :

| Paramètre | Type | Portée |
|---|---|---|
| `defaultThreshold` | `vector<double>` | Par neurone créé |
| `defaultNbStates` | `int` | Nombre d'états |
| `defaultState` | `int` | État initial |
| `defaultWeight` | `double` | Poids de synapse |
| `defaultTemperature` | `double` | Température neurone |
| `defaultDelay` | `int` | Délai synapse |

---

## 7. Export GNBox Premodel

Génère un fichier Prolog pour l'outil GNBox.

**Format :**
```prolog
premodel(Premodel) :-
  Premodel = premodel(
    [x_0, x_1, …, x_n],
    [c(x_i, [i(x_0,1), i(x_1,1), …]), …],
    [1, 1, …, 1]
  ).
```

**Règles :**
- Chaque neurone avec au moins une entrée : liste ses voisins entrants
- Neurone sans entrée (Garden of Eden) : auto-interaction `i(x_i,1)`
- Seuils : toujours `1` (simplifié pour GNBox)

---

## 8. Flux d'exécution typique

```
1. Démarrage
   main() → QApplication + fenêtre principale
   EvenementHandler connecte les widgets Qt

2. Chargement réseau
   DesignPlan.load(path) → NetworkDesignerParser.load()
   → Parsing XML → Network + UpdateSchedulingPlan créés
   → Rendu canvas

3. Conception interactive
   Événements souris/clavier → DesignPlan
   → Signaux → EvenementHandler → mise à jour panneaux UI

4. Paramétrage
   Spinboxes/comboboxes → slots EvenementHandler
   → setters sur Network / Neuron / Synapse
   → repaint DesignPlan

5. Simulation
   Bouton Start → Computer.compute*()
   Pour chaque itération :
     - Neurons compute next state
     - substitute() (application)
     - tick() signal émis
     - Barre de progression mise à jour

6. Arrêt
   Bouton Stop → Computer.stopComputing() → stop=true

7. Sauvegarde
   DesignPlan.save(path) → NetworkDesignerParser.save()
   → QDomDocument → fichier .nml
```

---

## 9. Limitations connues

| Limitation | Impact |
|---|---|
| Pas d'apprentissage (poids statiques) | Réseau figé à la conception |
| Pas de plasticité synaptique | Pas de Hebbian ou STDP |
| Temps discret uniquement | Pas de dynamiques continues |
| Analyse attracteurs exponentielle | Limite pratique ≈ 10–15 neurones binaires |
| Simulation couplée à Qt event loop | Difficile à paralléliser ou headless |
| Graine aléatoire non contrôlable depuis l'UI | Non-reproductibilité |
| Mémoire non bornée pour les bassins | Risque OOM sur grands réseaux |

---

## 10. Cas de test de non-régression

Ces fichiers `.nml` inclus dans le projet servent de référence pour valider une migration :

| Fichier | Description | Résultat attendu |
|---|---|---|
| `Arabidopsis.nml` | Réseau régulateur floral | Attracteurs biologiques connus |
| `HeartBeat.nml` | Oscillateur cardiaque | Cycle périodique stable |
| `EukarioteCell.nml` | Cycle cellulaire eucaryote | Attracteurs de phase G1/S/G2/M |

**Protocole de validation :**
1. Charger le fichier `.nml`
2. Lancer simulation Parallèle, 100 itérations, T=0
3. Capturer l'état final de chaque neurone
4. Comparer avec la valeur de référence obtenue sur la version actuelle
5. Lancer analyse attracteurs — comparer le nombre et les états des attracteurs

---

*Document généré par rétro-documentation du code source. Mise à jour requise si le comportement fonctionnel est modifié.*
