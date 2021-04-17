# Projektkurs inom dator och nätverksteknik

Grupp 5
Victor Pero, Linus Ekman, Alma Garpenfeldt, Kristoffer Pyddoke, Linus Silfver

## Kompilera koden
### Windows
MinGW32 måste vara installerad och ha följt den [här guiden](https://www.youtube.com/watch?v=UvJt9MZs_M8) för att installera *SDL*. *SDL_net* och *SDL_image* behöver även vara installerade på samma sätt som man installerade SDL från videon.

För att kompilera:
```
mingw32-make
```

### Mac
-

### Linux
Installera gcc och make om de redan inte är installerade.

Installera SDL, SDL_net och SDL_image med:
Debian baserade system:
```
sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-net-2.0-0 libsdl2-net-dev libsdl2-image-2.0-0 libsdl2-image-dev
```

För att kompilera:
```
make
```
