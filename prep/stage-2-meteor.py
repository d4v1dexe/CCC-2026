from pathlib import Path
import re

eingang_bohrer = 0
n = 0
ordner = Path(__file__).parent
datei = "in_level-1_beispiel.txt"
zeilen = [z for z in (ordner / datei).read_text().splitlines() if z]
ausgabe_datei = ordner / "meteoriten_ausgabe.txt"

zahlen = []
meteoriten = []

location = None

for zeile in zeilen:
    # Prüfen, ob die Zeile zum Raster gehört
    if "#" in zeile or "S" in zeile:
        if "S" in zeile:
            # Index (Spalte) von 'S' ermitteln
            location = zeile.index("S")
            meteoriten.append(zeile)
        else:
            # Zeichen an der Position 'location' durch 'X' ersetzen
            if location is not None and location < len(zeile):
                
                if zeile[location] == "#":
                    meteoriten.append(zeile)
                    meteoriten.append("")                    
                else:
                    zeile_liste = list(zeile)
                    zeile_liste[location] = "X"
                    zeile = "".join(zeile_liste)
                    meteoriten.append(zeile)
    else:
        zahlen.append(zeile)

print(zeilen)
print("\n",zahlen)
print("\n", meteoriten)

ausgabe_datei.write_text("\n".join(meteoriten), encoding="utf-8")
