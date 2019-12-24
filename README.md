# Mensch-aergere-das-Netzwerk-nicht
Abschlussprojekt für die C++-Vorlesung. Ziel ist ein Mensch ärgere dich nicht zu programmieren, das man über das Netzwerk spielen kann. Auch sollen Computergegner mitspielen.

## Anforderungskriterien
### Muss-Kriterien
- [ ] Funktionierendes Spiel
- [ ] Netzwerk mit mindestens 2 echten Spielern
- [ ] Funktionierender Com

### Soll-Kriterien
- [ ] GUI
- [ ] Speicherstände
- [ ] Netzwerk mit bis zu 4 Spielern
- [ ] Intelligenter Com

### Wunsch-Kriterien
- [ ] verschlüsselte Speicherstände
- [ ] fancy Grafik
- [ ] Statistiken
- [ ] Mini-Game beim Warten

## Kompilieren
Final sollte es möglich sein, dass das Projekt einfach mit dem Makefile kompiliert wird. </br>
Momentaner Stand (UNIX/LINUX):
<pre><code>
make && ./src/main
</code></pre>

## Netzwerkprotokoll
Für den Netzwerkverkehr zwischen Client und Server soll ein Protokoll definiert werden. </br>
Es besteht über das ganze Spiel hinweg eine Verbindung zwischen Client und Server. </br>
Sämtliche Berechnung der Spiels erfolgt beim Server. Der Client bekommt ausschließlich Spielinformationen übermittelt.
Der Client selbst tätigt nur drei Dinge:
1. Einen Befehl übermitteln, um den Würfel zu werfen. (Übermittlung "[Spielernummer]D")
2. Bei einer Wahl mehrerer Spielfiguren, soll er/sie die wählen mit der er/sie ziehen will. (Übermittlung "[Spielernummer][Figurnummer]")
3. Nachfrage nach einem aktuellen Spielfeld. (Übermittlung "N")
Nach jedem dieser Schritte wird dem Client vom Server ein aktueller Stand des Spielbretts, bzw. des Würfels, mitgeteilt. </br>
Auch nach und während des Zuges eines anderen Spielers wird jedem Client ein aktuelles Spielfeld übermittelt. </br></br>
### Spielbrettinformationen
Die Spielbrettinformationen werden über eine festgelegte Zahlenfolge übermittelt, die wie folgt zusammengesetzt ist: </br>
Gegeben: Spielbrett mit 40 Feldern und jeweils zwei 4-fach Start- und Ziel-Häuser für jeden Spieler. </br>
Die 40 Felder werden indexiert, angefangen mit 0 beim Startfeld des gelben Spielers, bis 39. </br>
Die Spieler erhalten folgenden Zahlencode: Gelb 1; Blau 2; Rot 3; Grün 4; Die Reihenfolge bleibt relevant. </br></br>
Nachrichtenformat:
<pre><code>
[0-4](0|1)(0|1)(0|1)(0|1)[0-4](0|1)(0|1)(0|1)(0|1)[0-4](0|1)(0|1)(0|1)(0|1)[0-4](0|1)(0|1)(0|1)(0|1) </br>
// Hausinformationen von Gelb bis Grün. Erste Zahl: Anzahl der Figuren im Starthaus. 4 Bit: Von außen nach innen besetztes Haus </br>
[0-4][0-4][0-4][0-4] ... [0-4] </br>
// 40 Zahlen von 0 bis 4 für das Spielfeld. Angefangen bei Index 0. Ende bei Index 39. </br>
[1-4] </br>
// Aktueller Spieler </br>
[1-6] </br>
// Aktuelle Würfelzahl
[0-4]
// Gewinner: 0 = keiner bisher gewonnen.
</code></pre>