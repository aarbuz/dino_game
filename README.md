# 🦖 Dino Game (Hardware Edition)

[![Status](https://img.shields.io/badge/Status-Zakończony-success.svg)]()
[![Platforma](https://img.shields.io/badge/Platforma-Arduino-blue.svg)]()

> Klasyczna gra z ukrytego trybu Google Chrome, przeniesiona do świata fizycznego. Projekt stworzony w celach edukacyjnych i dla czystej satysfakcji inżynierskiej.

---

## 🎮 Demo
![Zdjęcie 1](assets/photo1.png)
![Zdjęcie 2](assets/photo2.png)
![Film](assets/video.mov)
## 💡 Opis Projektu
Projekt to replika popularnej gry "T-Rex Runner". Skaczący dinozaur, przeszkody generowane z rosnącą prędkością i zliczanie punktów. Całość obsługiwana fizycznym przyciskiem i wyświetlana na małym ekranie. 

### Główne funkcje:
* Płynna animacja skoku (uwzględniająca grawitację).
* Zliczanie najlepszego wyniku (High Score).
* Dynamiczny poziom trudności (gra przyspiesza z czasem).

---

## 🛠️ Użyty Sprzęt (Hardware)
Jeśli chcesz zbudować to samemu, będziesz potrzebować:
* Mikrokontroler: **[np. Arduino Nano / ESP32]**
* Wyświetlacz: **[np. OLED 0.96" I2C / LCD TFT]**
* Sterowanie: **Fizyczny przycisk / Joystick**
* Kable zworkowe, płytka stykowa.
![Zdjęcie 3](assets/photo3.png
)
## 💻 Użyte Technologie (Software)
* Język: **C / C++** (Arduino IDE)
* Biblioteki:
  * `[np. Adafruit_GFX]`
  * `[np. Adafruit_SSD1306]`

---

## 🚀 Jak to uruchomić?

1. Pobierz repozytorium na swój komputer (przycisk `Code` -> `Download ZIP` lub przez komendę `git clone`).
2. Otwórz plik `dino_game.ino` w środowisku Arduino IDE.
3. Podłącz płytkę zgodnie ze schematem sygnałowym (zobacz sekcję poniżej).
4. Zainstaluj wymagane biblioteki w Menedżerze Bibliotek Arduino.
5. Skompiluj i wgraj kod na swoją płytkę.

## 🕹️ Sterowanie
* **Kliknięcie:** Skok
* **Przytrzymanie:** (Opcjonalnie, jeśli dodałeś schylanie się)

---
*Stworzone z pasją do elektroniki i kodowania. Jeśli projekt Ci się podoba, zostaw ⭐ na górze repozytorium!*
