n = int(input())

asteroids = []

for _ in range(n):
    x, y = map(int, input().split())
    asteroids.append((x, y))

# erst alle Zahlen
for x, y in asteroids:
    print(x, y)

# dann alle Asteroiden
for x, y in asteroids:
    yh = "#" * y
    yd = ":" * y
    print("#",yh,"#")
    for i in range(x):
        print("#",yd,"#")
    print("#",yh,"#")
