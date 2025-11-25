# 📘 RAPPORT TECHNIQUE DÉTAILLÉ – Système de Mesure Température & Humidité avec LCD I2C et Bargraph LED 74HC595
## 1. Titre du projet
Système intégré de mesure, d’affichage et d’alerte utilisant Arduino, LCD I2C et registre à décalage 74HC595

Ce projet consiste à concevoir un système capable de mesurer des grandeurs physiques (température et humidité du sol), de les afficher sur un écran à faible consommation de broches (I2C), et d'utiliser un registre à décalage pour piloter un bargraph LED représentant visuellement l’évolution de la température.

## 2. Objectif du projet

L’objectif du projet est d’apprendre à interfacer plusieurs types de capteurs, à utiliser différents protocoles de communication numérique et à concevoir une interface utilisateur simplifiée.
Le système doit être capable de :

Lire la température ambiante via le capteur TMP36.

Lire le taux d’humidité du sol avec un capteur analogique.

Afficher les valeurs sur un écran LCD piloté en I2C, afin d’utiliser seulement 2 broches du microcontrôleur.

Représenter graphiquement la température à l’aide d’un bargraph de LEDs commandé par un registre à décalage 74HC595 simulant un protocole SPI-like.

Avertir l’utilisateur si la température dépasse un seuil configurable.

Permettre à l’utilisateur de modifier ce seuil grâce à un bouton.

Ce mini-projet regroupe donc de nombreuses compétences importantes en électronique embarquée : lecture analogique, affichage numérique, protocole de communication, interaction utilisateur, et algorithmes non bloquants.

## 3. Matériel utilisé

Chaque composant utilisé a une fonction précise dans le projet :

✔️ Arduino Uno R3

C’est le cœur du système. Il gère les entrées analogiques, les calculs, le pilotage des modules, ainsi que l’affichage. On utilise ici un Uno car il possède suffisamment de broches et une bonne compatibilité avec les bibliothèques LCD et capteurs.

✔️ Écran LCD 16×2 + interface I2C (PCF8574)

Le module LCD permet d’afficher des informations textuelles. Le convertisseur I2C intégré réduit les besoins en broches en passant de 6–8 fils à seulement 2 fils (SDA et SCL).

✔️ Capteur TMP36

Un capteur de température analogique simple, précis et à sortie linéaire. Il fournit une tension proportionnelle à la température.

✔️ Capteur d’humidité du sol

Ce capteur mesure la conductivité du sol : plus il retient d’eau, plus la valeur analogique diminue. Cela permet de surveiller l’arrosage d’une plante ou d’un sol.

✔️ Registre à décalage 74HC595

Ce module augmente le nombre de sorties de l’Arduino. Il est parfait pour piloter un bargraph LED sans utiliser 8 broches directes de l’Uno.

✔️ LEDs + résistances 220 Ω

Elles composent le bargraph donnant une vision instantanée de la température. Les résistances évitent d’endommager les LEDs ou le 74HC595.

✔️ Bouton poussoir

Il permet d’interagir avec le système : changer de mode et configurer le seuil.

✔️ LED rouge d’alerte

Elle signale visuellement tout dépassement de seuil, ce qui rend le système plus intuitif.

## 4. Description des capteurs et modules
🔹 4.1 TMP36 – Capteur de température

Le TMP36 est un capteur de température analogique facile à utiliser.
Il possède une caractéristique linéaire :

sa tension de sortie augmente de 10 mV par degré Celsius

il fournit 0,5 V à 0°C

L’Arduino lit cette tension sur une broche analogique, puis convertit la valeur en °C.
L’avantage du TMP36 est qu’il n’a besoin d’aucune calibration complexe.

🔹 4.2 Capteur d’humidité du sol

Ce capteur fonctionne en mesurant la conductivité du sol.

Sol sec → résistance grande → valeur analogique élevée

Sol humide → résistance petite → valeur analogique faible

Ce capteur est utile pour automatiser un système d’arrosage ou simplement surveiller l’humidité.

🔹 4.3 Écran LCD I2C

Le module LCD avec interface I2C utilise seulement deux fils :

SDA (data)

SCL (clock)

Il possède une adresse (généralement 0x27) que l’Arduino interroge pour envoyer du texte.
L’I2C permet de connecter plusieurs modules sur les mêmes lignes, ce qui économise beaucoup de broches.

🔹 4.4 Registre à décalage 74HC595

Le 74HC595 est un composant numérique qui permet d'obtenir 8 sorties supplémentaires à partir d’une seule broche de données.

Fonctionnement :

L’Arduino envoie un octet, bit par bit.

À chaque impulsion d’horloge, le registre "décale" ses bits.

Lorsque les 8 bits sont envoyés, on active la broche latch pour les afficher.

Le 74HC595 est donc très utile pour piloter un grand nombre de LEDs tout en économisant les broches.

🔹 4.5 LED Rouge d’alerte

Elle s’allume ou clignote lorsque la température dépasse un seuil.
Cela permet à l’utilisateur de détecter rapidement un problème sans même regarder l’écran.

🔹 4.6 Bouton poussoir

Le bouton effectue plusieurs actions selon la durée d'appui :

Appui court : changer de mode ou augmenter le seuil

Appui long (2 secondes) : passer en mode configuration

Pour éviter les rebonds mécaniques, la gestion du bouton utilise millis() au lieu de delay().

## 5. Protocole I2C

Le protocole I2C (Inter-Integrated Circuit) est un bus de communication série permettant de connecter plusieurs périphériques sur seulement deux lignes.

Avantages :

utilise très peu de broches

peut connecter plusieurs modules simultanément

simple et fiable

Dans ce projet, il est utilisé pour le LCD.
L’Arduino envoie au module I2C les caractères à afficher. Le circuit PCF8574 se charge ensuite de piloter physiquement le LCD sans intervention supplémentaire.

## 6. Protocole SPI-like (74HC595)

Bien que le 74HC595 ne soit pas un périphérique SPI officiel, son fonctionnement est très proche :

DS → MOSI

Clock → SCK

Latch → SS

Il reçoit les données de manière séquentielle et les sort en parallèle.
Ce protocole est utile pour :

multiplier les sorties de l’Arduino

piloter des LEDs

créer des indicateurs visuels

Dans notre projet, il sert à afficher la température sous forme de bargraph LED.

## 7. Principe de fonctionnement global

Le fonctionnement général du système peut être résumé en étapes :

✔️ 1. Lecture des capteurs

L’Arduino lit le TMP36 et le capteur d’humidité toutes les quelques millisecondes sans bloquer le programme.

✔️ 2. Conversion et traitement

Les valeurs analogiques sont converties en °C et en % d’humidité.
Le bargraph est calculé selon la température.

✔️ 3. Affichage I2C

Le texte est écrit sur le LCD en fonction du mode choisi.

✔️ 4. Bargraph LED

Le registre à décalage illumine de 0 à 8 LEDs selon la température mesurée.

✔️ 5. Gestion de l’alerte

Si la température dépasse le seuil configuré, la LED rouge se met à clignoter.

✔️ 6. Navigation via bouton

L’utilisateur peut changer le mode et régler le seuil facilement.

## 8. Modes d’affichage

Le système possède plusieurs modes pour s’adapter à l’affichage désiré.

🔸 Mode 0 – Affichage température

Affiche la température mesurée ainsi que la tension issue du TMP36.

🔸 Mode 1 – Affichage humidité du sol

Affiche la valeur analogique du capteur d’humidité.

🔸 Mode 2 – Mode mixte

Combine température, seuil, et humidité du sol.
Ce mode est utile pour avoir une vue d’ensemble.

🔸 Mode configuration du seuil

Accessible par un appui long.
Dans ce mode, chaque appui court augmente le seuil d’alerte de 1°C.

## 9. Sécurité du 74HC595

Le 74HC595 est sensible au courant :

20 mA max par sortie

70 mA max pour l’ensemble du composant

C’est pourquoi les LEDs sont obligatoirement protégées par des résistances.
Sans cela, le 74HC595 peut surchauffer, se dégrader ou se détruire.

## 10. Conclusion

Ce mini-projet est un excellent exercice complet combinant :

électronique analogique

modules numériques

protocoles de communication avancés

interface utilisateur

gestion d’affichage

algorithmes non bloquants

Il constitue une base solide pour des projets plus avancés comme un système domotique, un contrôleur d’arrosage automatique ou un système de monitoring environnemental.
