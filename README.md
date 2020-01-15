# Mensch-aergere-das-Netzwerk-nicht
Abschlussprojekt für die C++-Vorlesung. Ziel ist ein Mensch ärgere dich nicht zu programmieren, das man über das Netzwerk spielen kann. Auch sollen Computergegner mitspielen.

## Anforderungskriterien
### Muss-Kriterien
- [X] Funktionierendes Spiel
- [X] Netzwerk mit mindestens 2 echten Spielern
- [X] Funktionierender Com

### Soll-Kriterien
- [X] GUI
- [X] Speicherstände
- [X] Netzwerk mit bis zu 4 Spielern
- [X] Intelligenter Com

### Wunsch-Kriterien
- [ ] verschlüsselte Speicherstände
- [X] fancy Grafik
- [ ] Statistiken
- [X] Minigame beim Warten

## Kompilieren und Ausführen
Um das Programm (die Programme) erfolgreich zu kompilieren, werden zwei Bibliotheken benötigt. Boost und SFML. <br/>
Verwendete Versionen:
- Boost: 1.71.0 (Ordner: boost_1_71_0)
- SFML: 2.5.1 (Ordner: SFML-2.5.1)

Um das Makefile zu verwenden müssen beide Bibliotheken NEBEN den 'Mensch_aergere_das_Netzwerk_nicht' gelegt werden. <br/>
Anschließend kann einfach mit dem Make-Befehl kompiliert werden. <br/>
Unsere genutzen Bibliotheken sind unter diesem Link als ZIP zu finden: <br/> https://1drv.ms/u/s!ApqzbZWwlyPWlwXRW297SQe0WEjG?e=ajcOUs <br/>
Informationen zu SFML: https://www.sfml-dev.org/
<pre><code>
make
</code></pre>
Die fertigen Programme liegen unter dem Ordner 'compiledProgramms' und können wie gewohnt ausgeführt werden. <br/>
<b>WICHTIG: </b>Bei Ausführung des Clients muss zudem ein Befehl hinzugefügt werden! Grund dafür ist, dass die Bibliothek für SFML nicht im Standard-Pfad ist. Zum Ausführen des Clients vom Ordner 'compiledProgramms' aus ist folgender Befehl zu gebrauchen: <br/>
<pre><code>
export LD_LIBRARY_PATH=../../SFML-2.5.1/lib && ./client_main_programm
</code></pre>
Der Befehl muss so mit den 'Unds' ausgeführt werden, sonst funktioniert es nicht! <br/>
Hinweis: Natürlich können auch alle relativen Pfade zum ausführen angepasst werden. <br/>
Zunächst wird der Server gestartet, der das Spiel hostet und ein reines Kommandozeilen-Programm ist. Der Port muss selbst
gewählt werden. Danach können bis zu 4 Clients geöffnet werden. IP und Port werden über die CLI eingegeben, der Rest
wird über die GUI gesteuert. <br/>
Soll ein Save Game geladen werden muss der relative Pfand (relativ zum Pfad der ausführbaren Datei) als einziges Argument 
bei Aufruf mitgegeben werden.

## Netzwerkprotokoll
Für den Netzwerkverkehr zwischen Client und Server soll ein Protokoll definiert werden. <br/>
Es besteht über das ganze Spiel hinweg eine Verbindung zwischen Client und Server. <br/>
Sämtliche Berechnung der Spiels erfolgt beim Server. Der Client bekommt ausschließlich Spielinformationen übermittelt.
Der Client selbst tätigt drei Dinge:
1. Übermittlung der Spielzüge(Würfeln: Übermittlung "[Spielernummer]D", Mit Figur ziehen: "[Spielernummer][Figurnummer]")
2. Übermittlung anderer "spielorganisatorischer" Befehle (Siehe unten).
3. Nachfrage nach einem aktuellen Spielfeld. (Übermittlung "N")
Nach jedem dieser Schritte wird dem Client vom Server ein aktueller Stand des Spielbretts, bzw. des Würfels, mitgeteilt. <br/>
Auch nach und während des Zuges eines anderen Spielers wird jedem Client ein aktuelles Spielfeld übermittelt. <br/>
### Spielbrettinformationen
Die Spielbrettinformationen werden über eine festgelegte Zahlenfolge übermittelt, die wie folgt zusammengesetzt ist: <br/>
Gegeben: Spielbrett mit 40 Feldern und jeweils zwei 4-fach Start- und Ziel-Häuser für jeden Spieler. <br/>
Die 40 Felder werden indexiert, angefangen mit 20 beim Startfeld des gelben Spielers, bis 59. <br/>
Die Spieler erhalten folgenden Zahlencode: Gelb 1; Blau 2; Rot 3; Grün 4; Die Reihenfolge bleibt relevant. <br/>
Nachrichtenformat:
<pre><code>
[0-4](0|1)(0|1)(0|1)(0|1)[0-4](0|1)(0|1)(0|1)(0|1)[0-4](0|1)(0|1)(0|1)(0|1)[0-4](0|1)(0|1)(0|1)(0|1) <br/>
// Hausinformationen von Gelb bis Grün. Erste Zahl: Anzahl der Figuren im Starthaus. 4 Bit: Von außen nach innen besetztes Haus <br/>
[0-4][0-4][0-4][0-4] ... [0-4] <br/>
// 40 Zahlen von 0 bis 4 für das Spielfeld. Angefangen bei Index 20. Ende bei Index 59. <br/>
[0-4] <br/>
// Aktueller Spieler. 0 = Spiel noch nicht gestartet. <br/>
[1-6] <br/>
// Aktuelle Würfelzahl
[0-4]
// Gewinner: 0 = keiner bisher gewonnen.
</code></pre>

### Spiel beitreten
Jeder Client hat einem Server beizutreten. Die übrigen Plätze werden mit Computergegnern gefüllt.
Hierfür wird vom Client eine Nachricht mit folgendem Format gesendet: [Spielernummer]join <br/>
Damit ein Client nur eine Spielernummer zu Identifikation schickt, ist im Client eine Schranke
eingebaut.

### Spiel mit nicht maximaler Anzahl natürlicher Spieler starten
Wenn alle bereits beigetretenen Spieler dem Server mitteilen, dass es losgehen soll, werden die übrigen
Plätze mit COMs aufgefüllt! <br/>
Hierfür schicken die Clients '[Spielernummer]start' an den Server in der 'Anmelde-Phase'.
Das funktioniert auch bei einem Spieler! -> Man muss warten bis alle Spieler die mitspielen wollen
auch gejoint sind. <br/>
Mit dieser Logik wird es möglich, dass ein Spieler auch mit dem Befehl ...start joined. Er ist dann sofort als bereit gemeldet.

### Spiel unterbrechen und Save Game erstellen
Zu jedem Zeitpunkt im Spiel können natürliche Spieler die Beendung des Spiels fordern. <br/>
Hierfür müssen sie (in einem gleichbleibemdem Status!) dem Server '[Spielernummer]quit' senden.
Falls alle natürlichen Spieler das gemacht haben wird das Spiel im aktuellen Status beendet und
ein Save Game File im Order 'saveGames' hinterlegt. <br/>
Zu beachten ist, dass ein Spiel nur in Phasen des Spiels beendet werden kann, in denen noch gewürfelt
werden muss, da es sonst beim laden des Spielstands zu Inkonsistenzen kommen kann.

### Spielzug weitergeben
Im bestimmten Situationen wird es nötig, dass ein Spieler seinen Spielzug weitergibt und nicht zieht.
Das passiert z.B. wenn kein Zug mit der Würfelzahl gültig ist. <br/>
Infolgedessen schickt der Spieler folgenden Befehl -> '[Spielernummer]P' (P steht für 'pass'). <br/>
Diese Aufforderung ist auch möglich, wenn ein Spielzug theoretisch möglich ist. Es muss vorher gewürfelt worden sein.

## Bedienung der Spieloberfläche
Zu Beginn jedes Zuges muss der Spieler würfeln, dafür muss aud die Schaltfläche mit der Aufschrift "Wuerfeln" unter dem Spielwürfel geklickt werden. Das Ergebnis zeigt dann der Spielwürfel an.<br />
Danach kann der Spieler mit einer Figur seiner Wahl ziehen. Dafür muss diese einfach angeklickt werden. Ist der Zug gültig, wird gezogen und das Spiel geht weiter.<br />
Ist der Zug nicht erlaubt, erscheint eine Fehlermeldung und eine andere Figur kann ausgewählt werden.<br />
Ist mit der gewürfelten Zahl kein Zug möglich, kann der Zug über die Taste "Weitergeben" übersprungen werden. Der nächste Spieler ist nun an der Reihe.<br />
Wenn vor Beginn des Spieles noch auf weitere Spieler gewartet wird, kann der Spieler mit einem Klick auf "Starten" dem Server mitteilen, dass er auf keine weiteren Spieler mehr warten möchte.<br />
Haben alle Mitspieler dies getan, werden die restlichen Spieler mit COMs aufgefüllt und das Spiel gestartet.<br />
Damit alle Spieler immer auf dem aktuellen Stand gehalten werden, wird das Spielfeld wird auch aktualisiert, wenn der Spieler nicht an der Reihe ist.<br />
Das Spiel kann gespeichert werden. Dafür müssen alle Teilnehmer innerhalb eines Zuges die Taste "Speicheranfrage Senden" betätigen. <br />
Ist das erfolgt, speichert der Server das Spiel und die Spieler können ihr Programm beenden. Weitere Züge sind erst nach einem erneuten Start des Servers möglich.<br />
Wollen nicht alle Spieler das Spiel beenden, wird dieses normal fortgeführt.<br />

### Minigame
Während gewartet wird, dass andere Spieler ziehen, gibt es die Möglichkeit ein kleines Minispiel zu spielen.<br />
Rechts neben dem Spielfeld befindet sich ein kleines Fenster. In diesem kann auf "Neue Aufgabe" geklickt werden, um eine kleine mathematische Aufgabe zu erhalten.<br />
Die Eingabe des Ergebnisses erfolgt über die Tastatur. <br />
Ist das Ergebnis korrekt, verfärbt sich es grün.<br />
Bei manchen Geräten ist es nicht möglich, mit der Rückschritttaste die letze eingegebene Zahl zu löschen. <br />
Dafür kann entweder die Taste "z" (für zurück) oder "r" (für Rückschritttaste / return) genutzt werden. Mit "l" (für löschen) ist es ebenfalls möglich die gesamte Eingabe zu löschen.<br />
