<h1 align="center">
  <a href="ссылка на видео"><img src="https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/img/pumpkin.jpg" alt="Хеллоуинская тыква" width="800"></a>
  <br>
    Хеллоуинская тыква
  <br>
</h1>

<p align="center">
  <a href="https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/README.md">Русский</a> •
  <a href="https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/README-en.md">English(Английский)</a> 
</p>

# Описание проекта
Тыква на хеллоуин на скорую руку

# Основные детали тыквы
* arduino pro mini;
* tp4056 с защитой;
* ckcs bs01 (или другой) повышающий модуль питания 3В-4.2В (li-ion) -> 5В;
* тумблер KCD1-11;
* 18650 аккумулятор;
* sg90 сервопривод;
* модуль из 8 светодиодов WS2812 (кольцо);
* небольшой динамик (я снял со старого телефона);
* hw-517 или другой транзисторный ключ; 
* [тыква, корпус и т.д.](ссылка на thingiverse)

### Крепеж
* Саморез DIN7981 2.9x9.5 x4;
* Саморез DIN7982 2.2x9.5 x5;

# Схема подключения
![Схема подключения](https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/img/schematic.png)

# Прошивка и звук
Прошивка для arduino pro mini: **halloween_pumpkin/halloween_pumpkin.ino**     
Рядом лежит заголовочный файл **sound.h** с воспоизводимой аудиодорожкой.    
При желании его можно заменить на свое аудио. При помощи программы 
[Audacity](https://www.audacityteam.org) и скрипта **sound2h.py**. 
Подробности смотрите в видео.

