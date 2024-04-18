# IOT-arduino

Munkhdorj ERDENEBAATAR, Clément JEANNE DIT FOUQUE

M2 IWOCS

## Description
Ce projet consiste à automatiser la gestion des serres, Arduino est utilisé pour surveiller la température et l'humidité grâce à des capteurs DHT. 
Il est possible de vérifier la condition d'une serre avec un LED RGB(rouge:mauvaise, bleu:neutre, vert: bonne).

Il est également possible de visualiser ces données via l'interface Web. L'application Web est developpé en React, les données sont sauvegardées dans une base de données MongoDB.

- Lancer l'application:
  - Brancher le composant arduino
  - Charger le code arduino dans le composant
  - Aller dans la racine du projet et lancer ces commandes:
    - docker compose build
    - docker compose up
