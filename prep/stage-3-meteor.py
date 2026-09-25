from pathlib import Path
import re

eingang_bohrer = 0
n = 0
ordner = Path(r"C:\Users\david\coding_projects\CCC-2026\prep\input")
datei = "in_level-3_1-small.txt"
zeilen = [z for z in (ordner / datei).read_text().splitlines() if z]
ausgabe_datei = ordner / "ausgabel3.txt"

x_replace = 2
y_replace = 2
var = 0

zahlen = []
meteoriten = []

def ist_raster(z):
    # Alles, was nicht nur aus Zahlen/Leerzeichen besteht, gehört zum Raster
    return not re.fullmatch(r"[\d\s]+", z)

i = 0
while i < len(zeilen):
    if ist_raster(zeilen[i]):
        # Alle zusammenhängenden Rasterzeilen = ein Meteorit
        block = []
        while i < len(zeilen) and ist_raster(zeilen[i]):
            block.append(zeilen[i])
            i += 1

        x = len(block[0])   # Breite des Meteoriten
        y = len(block)      # Länge (Höhe) des Meteoriten
        print(f"Meteorit: x = {x}, y = {y}")

        location = None
        seitwaerts = 0
        for zeile in block:
            if "S" in zeile:
                location = zeile.index("S")
                meteoriten.append(zeile)
            elif location is not None and location < len(zeile):
                if zeile[location] == "#":
                    meteoriten.append(zeile)
                    meteoriten.append("")
                else:
                    zeile_liste = list(zeile)
                    if seitwaerts == 1:
                        c = location
                        while zeile_liste[c] != "#":
                            zeile_liste[c] = "X"
                            c -= 1
                    elif seitwaerts == 0:
                        zeile_liste[location] = "X"
                    zeile = "".join(zeile_liste)
                    meteoriten.append(zeile)
                    if y >= x and location != 2:
                        location = 2
                    elif x > y:
                        seitwaerts += 1
    else:
        zahlen.append(zeilen[i])
        i += 1
"""""
print(zeilen)
print("\n", zahlen)
print("\n", meteoriten)
"""""
ausgabe_datei.write_text("\n".join(meteoriten), encoding="utf-8")
