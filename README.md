# Four Solaire
 
![enter image description here](https://upload.wikimedia.org/wikipedia/commons/d/df/Foursolaire.JPG)

**Ronan Defours** M2M - Master Génie Informatique - UGA

Lien du **[ScreenCast de la présentation](https://drive.google.com/open?id=1YX3cjcwudY5AtI17BfTWDBc2GfoZuF0J)**

## Introduction
Le but du four solaire connecté et la surveillance de la température et de la cuisson des aliments d'un four solaire à distance.

## Liste des fichiers

 - **README.md** fichier markdown regroupant rapport technique, fiche ACS et fiche "Security&Privacy"
 - **fourSolaire.ino** code source
 - **fourSolaire.pdf** présentation au format PDF
 - **fourSolaireAvecNotes.odp** présentation au format ODP avec le discour dans les rédigé dans les notes

## Ressources

 - Nucleo F401-RE
 - I-Nucleo-LRWAN1
 - Dévellopement sur l'IDE Arduino

## Déroulement d'un Cycle
Le logiciel déroule des cycles de 10 secondes comprenant plusieurs étapes:

 - Mesure de la température
 - Calcul de la cuisson (si cuisson en cour)
 - Envoie de message si nécessaire
 - Réception de messages

## Fonctionnalités

 - Calcul de la cuisson, proportionnellement à la température par rapport à la température souhaité
 - Mesure du temps de cuisson nécessaire
 - Message LoRa d'avertissement si 75% cuit
 - Message LoRa quand la cuisson est complète
 - réception de code par LoRa pour:
   - déclencher l'envoie de la température actuelle du four
   - déclencher l'envoie du temps écoulé depuis le début de la cuisson
   - stopper la mesure de cuisson en cour
   - reprendre/démarrer la mesure de la cuisson
   - recommencer la mesure de la cuisson

## Analyse Cycle de Vie
### Four Solaire
D'après une [étude](https://www.sciencedirect.com/science/article/pii/S0048969718330894) de l'Université de Manchester qui compare les fours solaires et les fours à micro-ondes, les fours solaires permettrait de réduire les coûts annuels en **[cycles de vie](https://fr.wikipedia.org/wiki/Co%C3%BBt_du_cycle_de_vie) de 40% et l'impact environnemental de près de 65%**. Il est à noter que cette étude prend l'exemple de conditions semblables à celles de l'Espagne, et qu'elle part du principe que les fours solaires sont majoritairement fabriqués à partir de matériaux réutilisés.
Les fours solaires permettrait une réduction le consommation d’électricité ainsi que la production de CO2.

Graphical abstract de l'étude:
![Graphical abstract](https://ars.els-cdn.com/content/image/1-s2.0-S0048969718330894-ga1.jpg)

On retrouve fortemment ici les axes 2 et 3 du [SNTEDD](https://www.ecologique-solidaire.gouv.fr/strategie-nationale-transition-ecologique-vers-developpement-durable-2015-2020), à savoir d'engager dans l'économie circulaire et sobre en carbonne, ainsi que prévenir et réduire les inégalités environnementales, sociales et territoriales.
Il est intéressant de remarquer que la pratique de fabriquer son propre four solaire s'inscrit dans l'axe 7 (Éduquer, former et sensibiliser), car c'est une action qui rend concrète la gestion des ressources au sein même d'une famille.
### Matériel de communication à distance
Ce projet de four solaire connecté s'inscrit dans l'axe 6 lié à la production de connaissance, mais on peut aussi imaginer la mettre en lien avec l'axe 4: si la confection de four solaire devient courante, la vente de produit en lien peut voir le jour. Comme par exemple un module pour rendre un four solaire artisanal connecté. Ce genre d'offre pourrait avoir un effet positif sur le taux d'adoption des fours solaires, créant un cercle vertueux.

On peut toutefois noter que connecter son four solaire n'est pas aussi essentiel que le besoin de cuire sa nourriture. Or, les composants nécessaires utilisent des [terres rares](https://fr.wikipedia.org/wiki/Terre_rare#Cons%C3%A9quences_environnementales)  dont l'extraction et le raffinage entraînent le rejet de nombreux éléments toxiques. 

## Privacy & Security
La partie Privacy n'est pas un enjeu majeur de ce projet car les données échangées et enregistrés ne comportent aucune données personnelles et/ou sensibles. 
Toutefois la communication par réseaux LoRa entraîne de nombreux soucis de sécurité propre à la fois aux communications M2M, aux communication à distances ainsi qu'aux communications LoRa. Dans une [étude](https://www.researchgate.net/publication/329858421_Security_Risk_Analysis_of_LoRaWAN_and_Future_Directions) de 2018 sur les vulnérabilités des communications LoRa, il est montré que les principale vulnérabilité de LoRa sont: 
 - la capture physique des matériels
 - les gateways malicieuses 
 - les attaques par rejeu 
Même si le cas d'un four solaire peux sembler comporter des risques minimes, ces vulnérabilités sont à prendre en compte lors de la mise en place d'un tel projet.

