from pathlib import Path

# Ordnerpfad festlegen
input_ordner = Path(r"C:\Users\david\coding_projects\CCC-2026\prep\input")

eingabe_pfad = input_ordner / "in_level-1_1-small.txt"
ausgabe_pfad = input_ordner / "ausgabe.txt"

# 1. Datei einlesen (filtert leere Zeilen direkt heraus)
zeilen = [z.strip() for z in eingabe_pfad.read_text(encoding="utf-8").splitlines() if z.strip()]

# Erste Zeile ist 'n'
n = int(zeilen[0])

# Paare einlesen und in der Liste speichern
asteroids = []
for i in range(1, n + 1):
    x, y = map(int, zeilen[i].split())
    asteroids.append((x, y))

# 2. Nur die Asteroiden-Zeichnungen in 'ausgabe.txt' speichern
with open(ausgabe_pfad, "w", encoding="utf-8") as f:
    for x, y in asteroids:
        yh = "#" * y
        yd = ":" * y
        
        print("#", yh, "#", sep="", file=f)
        for _ in range(x):
            print("#", yd, "#", sep="", file=f)
        print("#", yh, "#", sep="", file=f)
        print(file=f)

print(f"Fertig! Das Ergebnis wurde in '{ausgabe_pfad.name}' gespeichert.")
