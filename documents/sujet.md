# Méthodes ensemblistes pour la modélisation
On dispose d'un jeu de données regroupant les exécutions d'un programme avec plusieurs
configurations. On souhaite modéliser ce programme via des techniques d'IA afin de pouvoir
plus facilement explorer l'espace des configurations. L'objectif de ce projet est
d'implémenter en C ou C++ une technique ensembliste pour la régression, à partir de zéro.
Plus spécifiquement, on s'intéressera aux méthodes de bagging (et optionnellement de
boosting) se basant sur les arbres de décisions.
Pour cela, les étudiants devront réaliser un travail bibliographique pour mieux comprendre
le domaine des techniques ensemblistes et le fonctionnement des arbres de décisions, puis
implémenter un ou plusieurs de ces algorithmes. Le projet se composera en deux phases :


## Premier semestre
    * Implémentation d'un arbre de décision pour la régression, module de chargement
    de données
    * Implémentation de la méthode de bagging (et optionnellement de boosting) se
    basant sur les arbres de décisions
    * Évaluation de précision et profilage de performance, cross-validation par
    technique des k-folds, impact des hyper-paramètres
    
## Second semestre
    * Parallélisation de la construction des arbres, méthodes ensemblistes distribués
    * Mesures de performances et comparaisons des différentes versions. Mesure de
    l'impact des optimisations sur la précision finale du modèle.
    * (optionnel) Choix du format de stockage (float, double, short, int, long) et impact
    sur la performance et la précision
    Une attention particulière sera portée sur la qualité logicielle. Les données d'entraînement
    seront mises à disposition par l'encadrant pendant le premier semestre. 