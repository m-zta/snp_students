# 07 - Prozesse und Threads

---

![](./ein_mann_orchester.png)

[Quelle: https://www.wikiwand.com/de/Ein-Mann-Orchester](https://www.wikiwand.com/de/Ein-Mann-Orchester)

---

## 1. Übersicht

In diesem Praktikum werden wir uns mit Prozessen, Prozesshierarchien und Threads beschäftigen, um ein gutes Grundverständnis dieser Abstraktionen zu erhalten. Sie werden bestehenden Code analysieren und damit experimentieren. D.h. dies ist nicht ein «Codierungs»-Praktikum, sondern ein «Analyse»- und «Experimentier»-Praktikum.

---

### 1.1 Nachweis

Dieses Praktikum ist eine leicht abgewandelte Variante des ProcThreads Praktikum des Moduls BSY, angepasst an die Verhältnisse des SNP Moduls. Die Beispiele und Beschreibungen wurden, wo möglich, eins-zu-ein übernommen.

Als Autoren des BSY Praktikums sind genannt: M. Thaler, J. Zeman.

---

## 2. Lernziele

In diesem Praktikum werden Sie sich mit Prozessen, Prozesshierarchien und Threads beschäftigen. Sie erhalten einen vertieften Einblick und Verständnis zur Erzeugung, Steuerung und Terminierung von Prozessen unter Unix/Linux und Sie werden die unterschiedlichen Eigenschaften von Prozessen und Threads kennenlernen.

- Sie können Prozesse erzeugen und die Prozesshierarchie erklären
- Sie wissen was beim Erzeugen eines Prozesses vom Elternprozess vererbt wird
- Sie wissen wie man auf die Terminierung von Kindprozessen wartet
- Sie kennen die Unterschiede zwischen Prozessen und Threads

---

## 3. Aufgaben

Das Betriebssystem bietet Programme um die aktuellen Prozesse und Threads darzustellen.

Die Werkzeuge kommen mit einer Vielzahl von Optionen für die Auswahl und Darstellung der Daten, z.B. ob nur Prozesse oder auch Threads aufgelistet werden sollen, und ob alle Prozesse oder nur die «eigenen» Prozesse ausgewählt werden sollen, etc.

Siehe die entsprechenden `man` Pages für weitere Details.

Eine Auswahl, welche unter Umständen für die folgenden Aufgaben nützlich sind:

|                        |                                                                                                                                                                             |
| :--------------------- | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `ps`                   | Auflisten der Prozess Zustände zum gegebenen Zeitpunkt.                                                                                                                     |
| `pstree`               | Darstellung der gesamten Prozesshierarchie.                                                                                                                                 |
| `top`                  | Wie `ps`, aber die Darstellung wird in Zeitintervallen aufdatiert.                                                                                                          |
| `htop`                 | Wie `top`, aber zusätzlich dazu die Auslastung der CPU in einem System mit mehreren CPUs.                                                                                   |
| `lscpu`                | Auflisten der CPUs.                                                                                                                                                         |
| `cat`/`proc`/`cpuinfo` | Ähnlich zu `lscpu`, aber mit Zusatzinformationen wie enthaltene CPU Bugs (z.B. `bugs: cpu_meltdown spectre_v1 spect-re_v2 spec_store_bypass l1tf mds swapgs itlb_multihit`) |

---

### 3.1 Aufgabe 1: Prozess mit fork() erzeugen

**Ziele**

- Verstehen, wie mit `fork()` Prozesse erzeugt werden.
- Einfache Prozesshierarchien kennenlernen.
- Verstehen, wie ein Programm, das `fork()` aufruft, durchlaufen wird.

**Aufgaben**

1. Studieren Sie zuerst das Programm `ProcA1.c` und beschrieben Sie was geschieht.

✅ **Lösung**:

- Ein Integer `i` wird mit 5 initialisiert und ihr Wert ausgegeben.
- `fork()` wird aufgerufen. Dieser Systemaufruf erstellt einen neuen Prozess (Kindprozess), der eine exakte Kopie des aufrufenden Prozesses (Elternprozess) ist. Ab diesem Punkt laufen zwei Prozesse parallel.
- `fork()` gibt im Elternprozess die PID des Kindes zurück (`pid > 0`), im Kindprozess gibt es 0 zurück (`pid == 0`), und bei einem Fehler -1.
- **Im Kindprozess (`case 0`)**: `i` wird inkrementiert (5 -> 6). Es gibt seine eigene PID (`getpid()`), die PID seines Elternprozesses (`getppid()`, was die PID des ursprünglichen Prozesses ist) und den Wert von `i` (6) aus.
- **Im Elternprozess (`default`)**: `i` wird dekrementiert (5 -> 4). Es gibt seine eigene PID, die PID des Kindes (den Rückgabewert von `fork()`) und den Wert von `i` (4) aus. Der Elternprozess wartet dann mit `wait(&status)` darauf, dass der Kindprozess endet.
- **Beide Prozesse**: Nach dem `switch`-Block geben beide Prozesse noch die Zeile ". . . . . und wer bin ich ?" aus, bevor sie mit `exit(0)` terminieren.

2. Notieren Sie sich, was ausgegeben wird. Starten Sie das Programm und vergleichen Sie die Ausgabe mit ihren Notizen? Was ist gleich, was anders und wieso?

✅ **Lösung**:

Ausgabe:

```bash
i vor fork: 5


... wir sind die Eltern 39319 mit i=4 und Kind 39320,
    unsere Eltern sind 1315

... ich bin das Kind 39320 mit i=6, meine Eltern sind 39319

. . . . . und wer bin ich ?


. . . . . und wer bin ich ?
```

- **Erwartung:** Man erwartet zuerst die Ausgabe "i vor fork: 5". Danach geben Eltern- und Kindprozess ihre jeweiligen Zeilen aus. Wichtig ist, dass der Elternprozess `i=4` und der Kindprozess `i=6` hat, was zeigt, dass sie nach dem `fork()` unabhängige Kopien der Variable `i` besitzen. Die Reihenfolge der Ausgaben von Kind und Eltern nach dem `fork` ist nicht deterministisch und hängt vom Betriebssystem-Scheduler ab. Zum Schluss sollte die "wer bin ich?"-Zeile zweimal erscheinen (einmal vom Kind, einmal vom Elternteil, nachdem dieser mit `wait` fertig ist).
- **Vergleich mit der Ausgabe:** Die Ausgabe (`Eltern 39319 mit i=4`, `Kind 39320 mit i=6`) bestätigt genau dieses Verhalten: Die PIDs sind unterschiedlich, die Eltern-PID des Kindes ist die PID des Elternprozesses, und die Variable `i` hat in beiden Prozessen unterschiedliche Werte angenommen (4 im Eltern-, 6 im Kindprozess). Die "wer bin ich?"-Zeile erscheint ebenfalls zweimal. Das Ergebnis passt perfekt zur Funktionsweise von `fork()`.

---

### 3.2 Aufgabe 2: Prozess mit fork() und exec(): Programm Image ersetzen

**Ziele**

- An einem Beispiel die Funktion `execl()` kennenlernen.
- Verstehen, wie nach `fork()` ein neues Programm gestartet wird.

**Aufgaben**

1. Studieren Sie zuerst die Programme `ProcA2.c` und `ChildProcA2.c` ✅.
2. Starten Sie `ProcA2.e` und vergleichen Sie die Ausgabe mit der Ausgabe unter Aufgabe 1. Diskutieren und erklären Sie was gleich ist und was anders.

✅ **Lösung**:

Ausgabe:

```bash
i vor fork: 5


... wir sind die Eltern 39522 mit i=4 und Kind 39523,
    unsere Eltern sind 1315

... ich bin das Kind 39523 mit i=6, meine Eltern sind 39522

. . . . . und wer bin ich ?
```

- Die Ausgabe für `ProcA2.e` zeigt `Eltern 39522 mit i=4` und `Kind 39523 mit i=6`. Das ist bezüglich der PIDs und der Werte von `i` identisch zu Aufgabe 1.
- **Der Unterschied liegt im Mechanismus:** In Aufgabe 1 hat der Kindprozess selbst `i` erhöht und die Ausgabe formuliert. In Aufgabe 2 hat der Kindprozess zwar auch `i` erhöht, aber dann sein eigenes Programm durch `ChildProcA2.e` ersetzt. Dieses neue Programm hat den Wert `i=6` als Argument erhalten und die Ausgabe generiert.
- **"wer bin ich?"-Zeile:** Wie in der `thought`-Sektion analysiert, sollte diese Zeile in `ProcA2.c` nur einmal (vom Elternprozess nach dem `wait()`) ausgegeben werden, da `execl` im Kind normalerweise nicht zurückkehrt. Deine Ausgabe zeigt sie zweimal, genau wie bei Aufgabe 1. Das deutet darauf hin, dass auch der Kindprozess nach `execl` (oder dessen Scheitern in späteren Schritten) noch den Code nach dem `switch` erreicht und ausführt. Der wesentliche Unterschied bleibt aber, _welches_ Programm die "Kind"-Zeile (`... ich bin das Kind ...`) druckt.

3. Benennen Sie `ChildProcA2.e` auf `ChildProcA2.f` um (Shell Befehl `mv`) und überlegen Sie, was das Programm nun ausgibt. Starten Sie `ProcA2.e` und vergleichen Sie Ihre Überlegungen mit der Programmausgabe.

✅ **Lösung**:

Ausgabe:

```bash
i vor fork: 5


... wir sind die Eltern 39591 mit i=4 und Kind 39592,
    unsere Eltern sind 1315

execl not successful: No such file or directory

. . . . . und wer bin ich ?


. . . . . und wer bin ich ?
```

- **Überlegung:** Wenn `ChildProcA2.e` nicht gefunden wird, schlägt der `execl`-Aufruf fehl. `execl` gibt dann -1 zurück.
- **Ausgabe:** Die Bedingung `if (retval < 0)` in `ProcA2.c` wird wahr, und `perror("\nexecl not successful")` wird ausgeführt. `perror` gibt den String "execl not successful" gefolgt von der Systemfehlermeldung für "Datei nicht gefunden" aus ("No such file or directory"). Der Kindprozess wird _nicht_ ersetzt, führt den Code nach dem `if` aus (also `break`), und erreicht dann wie der Elternprozess die "wer bin ich?"-Zeile.
- **Vergleich:** Deine Ausgabe ("execl not successful: No such file or directory") bestätigt dies exakt.

4. Nennen Sie das Kindprogramm wieder `ChildProcA2.e` und geben Sie folgenden Befehl ein: `chmod -x ChildProcA2.e`. Starten Sie wiederum `ProcA2.e` und analysieren Sie die Ausgabe von `perror("...")`. Wieso verwenden wir `perror()`?

✅ **Lösung**:

Ausgabe:

```bash
i vor fork: 5


... wir sind die Eltern 39621 mit i=4 und Kind 39622,
    unsere Eltern sind 1315

execl not successful: Permission denied

. . . . . und wer bin ich ?


. . . . . und wer bin ich ?
```

- **Überlegung:** Wenn `ChildProcA2.e` keine Ausführungsrechte hat, schlägt `execl` ebenfalls fehl, aber aus einem anderen Grund.
- **Ausgabe:** `execl` gibt -1 zurück, `perror` wird aufgerufen und gibt "execl not successful" gefolgt von der Systemfehlermeldung für "Zugriff verweigert" aus ("Permission denied"). Der Kindprozess wird wieder nicht ersetzt und führt den Code nach dem `switch` aus.
- **Vergleich:** Deine Ausgabe ("execl not successful: Permission denied") bestätigt auch dies.
- **Warum `perror()`?** `perror()` ist nützlich, weil es nicht nur eine eigene Fehlermeldung ausgibt, sondern auch die systemspezifische Beschreibung des Fehlers, der in der globalen Variable `errno` gespeichert wurde. Das macht die Fehlersuche viel einfacher als nur eine generische Meldung wie "Fehler beim Ausführen".

---

### 3.3 Aufgabe 3: Prozesshierarchie analysieren

**Ziele**

- Verstehen, was `fork()` wirklich macht.
- Verstehen, was Prozesshierarchien sind.

**Aufgaben**

1. Studieren Sie zuerst Programm `ProcA3.c` und zeichnen Sie die entstehende Prozesshierarchie (Baum) von Hand auf. Starten Sie das Programm und verifizieren Sie ob Ihre Prozesshierarchie stimmt.

✅ **Lösung**:

Prozesshierarchie:

```
Shell(pidS)---ProcA3.e(P0, pidS)
    |-ProcA3.e(P8, P0)
    |-ProcA3.e(P4, P0)
    |   `-ProcA3.e(P12, P4)
    |-ProcA3.e(P2, P0)
    |   |-ProcA3.e(P10, P2)
    |   `-ProcA3.e(P6, P2)
    |       `-ProcA3.e(P14, P6)
    `-ProcA3.e(P1, P0)
        |-ProcA3.e(P9, P1)
        |-ProcA3.e(P5, P1)
        |   `-ProcA3.e(P13, P5)
        `-ProcA3.e(P3, P1)
            |-ProcA3.e(P11, P3)
            `-ProcA3.e(P7, P3)
                `-ProcA3.e(P15, P7)
```

Das Programm `ProcA3.c` ruft viermal hintereinander die Funktion `fork()` auf. Jeder `fork()`-Aufruf verdoppelt die Anzahl der laufenden Prozesse, die den nachfolgenden Code ausführen.

- Start: 1 Prozess
- Nach 1. `fork()`: 2 Prozesse
- Nach 2. `fork()`: 4 Prozesse
- Nach 3. `fork()`: 8 Prozesse
- Nach 4. `fork()`: 16 Prozesse

Alle diese 16 Prozesse führen dann `printf("PID: %d\t PPID: %d\n", getpid(), getppid());` und `sleep(10);` aus. Das `sleep(10)` sorgt dafür, dass alle Prozesse lange genug aktiv bleiben, um die Hierarchie beobachten zu können.

2. Mit dem Befehl `ps f` oder `pstree` können Sie die Prozesshierarchie auf dem Bildschirm ausgeben. Damit die Ausgabe von `pstree` übersichtlich ist, müssen Sie in dem Fenster, wo Sie das Programm `ProcA3.e` starten, zuerst die PID der Shell erfragen, z.B. über `echo $$`. Wenn Sie nun den Befehl `pstree -n -p pid-von-oben` eingeben, wird nur die Prozesshierarchie ausgehend von der Bash Shell angezeigt: `-n` sortiert die Prozesse numerisch, `-p` zeigt für jeden Prozess die PID an.

**Hinweis:** alle erzeugten Prozesse müssen arbeiten (d.h. nicht terminiert sein), damit die Darstellung gelingt. Wie wird das im gegebenen Programm erreicht?

✅ **Lösung**:

(Ausgabe mit `pstree -n 41857`):

```
pstree -p 41857
-+= 00001 root /sbin/launchd
 \-+= 01266 mario /Applications/iTerm.app/Contents/MacOS/iTerm2
   \-+- 01313 mario /Users/mario/Library/Application Support/iTerm2/iTermServer
     \-+= 41856 root login -fp mario
       \-+= 41857 mario -bash
         \-+= 42220 mario ProcA3.e
           |-+- 42221 mario ProcA3.e
           | |-+- 42224 mario ProcA3.e
           | | |-+- 42230 mario ProcA3.e
           | | | \--- 42235 mario ProcA3.e
           | | \--- 42232 mario ProcA3.e
           | |-+- 42226 mario ProcA3.e
           | | \--- 42233 mario ProcA3.e
           | \--- 42229 mario ProcA3.e
           |-+- 42222 mario ProcA3.e
           | |-+- 42227 mario ProcA3.e
           | | \--- 42234 mario ProcA3.e
           | \--- 42231 mario ProcA3.e
           |-+- 42223 mario ProcA3.e
           | \--- 42228 mario ProcA3.e
           \--- 42225 mario ProcA3.e
```

---

### 3.4 Aufgabe 4: Zeitlicher Ablauf von Prozessen

**Ziele**

- Verstehen, wie Kind- und Elternprozesse zeitlich ablaufen.

**Aufgaben**

1. Studieren Sie Programm `ProcA4.c.` Starten Sie nun mehrmals hintereinander das Programm `ProcA4.e` und vergleichen Sie die jeweiligen Outputs (leiten Sie dazu auch die Ausgabe auf verschiedene Dateien um). Was schliessen Sie aus dem Resultat?

**Anmerkung:** Der Funktionsaufruf `selectCPU(0)` erzwingt die Ausführung des Eltern- und Kindprozesses auf CPU 0 (siehe Modul `setCPU.c`). Die Prozedur `justWork(HARD_WORK)` simuliert CPU-Load durch den Prozess (siehe Modul `workerUtils.c`).

✅ **Lösung**:

**Analyse von `ProcA4.c`:**

Das Programm erstellt mit `fork()` einen Kindprozess. Sowohl der Eltern- als auch der Kindprozess versuchen mittels `selectCPU(0)`, sich auf CPU-Kern 0 festzulegen (was, wie wir wissen, auf macOS nicht funktioniert und nur eine Meldung ausgibt). Danach führen beide eine Schleife 20 Mal aus (`ITERATIONS`). In jeder Iteration simulieren sie Arbeit mit `justWork(WORK_HARD)` und geben dann entweder "Mother" oder "Child" zusammen mit dem Schleifenzähler aus. `fflush(stdout)` stellt sicher, dass die Ausgabe sofort erfolgt. Am Ende wartet der Elternprozess auf den Kindprozess.

**Analyse der Ausgaben (`out1.txt` bis `out4.txt`):**

- Die Meldung "Cannot set single CPU on OSX ... continue anyway" erscheint in jeder Datei zweimal, was bestätigt, dass die CPU-Festlegung nicht greift.
- In allen vier Dateien ist die Reihenfolge der Ausgaben **exakt identisch**: Für jeden Zähler `i` von 0 bis 19 erscheint zuerst die Zeile "Mother" und direkt danach die Zeile "Child". Es gibt keinerlei Variation oder Durchmischung der Reihenfolge zwischen den verschiedenen Läufen.

**Schlussfolgerung aus dem Resultat:**

Das Ziel dieser Aufgabe ist es normalerweise, die **Nicht-Determinismus** des Betriebssystem-Schedulers zu beobachten. Wenn zwei Prozesse (Eltern und Kind) um CPU-Zeit konkurrieren – insbesondere wenn sie auf denselben Kern gezwungen werden, wie es `selectCPU(0)` unter Linux tun würde – ist die Reihenfolge, in der sie ihre Arbeitsschritte (hier: `justWork` und `printf`) ausführen, von Lauf zu Lauf typischerweise unterschiedlich. Mal ist der Elternprozess etwas schneller, mal das Kind. Das sollte zu variierenden Ausgabereihenfolgen führen (z.B. mal "Mother 0", "Mother 1", "Child 0", "Child 1", ... oder "Child 0", "Mother 0", "Child 1", "Mother 1", ...).

**Warum siehst du keine Unterschiede auf deinem Mac?**

1.  **`selectCPU(0)` ohne Effekt:** Da die Funktion auf macOS die Prozesse nicht auf einen Kern beschränkt, können Eltern- und Kindprozess vom Scheduler frei auf die verfügbaren Kerne deines M1-Chips verteilt werden.
2.  **Effizientes Scheduling auf Multi-Core:** Moderne Betriebssysteme und Multi-Core-Prozessoren (wie der M1) sind sehr gut darin, zwei solche rechenintensiven, aber einfachen Prozesse parallel oder sehr fein zerlegt (time-slicing) auszuführen. Der macOS-Scheduler scheint hier sehr konsistent zu arbeiten und beiden Prozessen quasi abwechselnd oder parallel genug Zeit zu geben, um eine Iteration (Arbeit + Ausgabe) abzuschliessen, bevor der andere überholt wird.
3.  **Konsequenz:** Die Kombination aus dem nicht funktionierenden `selectCPU(0)` und dem effizienten, konsistenten Scheduling auf deinem System führt dazu, dass die erwartete Variabilität in der Ausgabereihenfolge ausbleibt. Die Ausgabe ist in jedem Lauf gleich (perfektes abwechselndes Muster).

---

### 3.5 Aufgabe 5: Waisenkinder (Orphan Processes)

**Ziele**

- Verstehen, was mit verwaisten Kindern geschieht.

**Aufgaben**

1. Analysieren Sie Programm `ProcA5.c`: was läuft ab und welche Ausgabe erwarten Sie?

✅ **Lösung**:

- **Ablauf:** Das Programm erstellt einen Kindprozess mittels `fork()`.
  - **Kindprozess:** Gibt eine Startmeldung aus und tritt in eine Schleife ein, die 10 Mal durchlaufen wird. In jeder Iteration pausiert es 0.5 Sekunden (`usleep`), holt sich die PID seines aktuellen Elternprozesses (`getppid()`) und gibt diese aus. Nach der Schleife gibt es eine Endmeldung aus.
  - **Elternprozess:** Pausiert für 2 Sekunden (`sleep(2)`) und beendet sich dann sofort mit `exit(0)`, _ohne_ auf das Kind zu warten (`wait()` wird nicht aufgerufen).
  - **Nach dem `switch`:** Beide Prozesse versuchen am Ende, `sprintf(buf, "ps -p %d", id);` gefolgt von `system(buf);` auszuführen. Die Variable `id` soll die zuletzt vom Kind gelesene Eltern-PID enthalten, ist aber im Elternprozess uninitialisiert und wird erst im Kindprozess _in_ der Schleife zugewiesen, was problematisch ist (siehe Compiler-Warnung).
- **Erwartete Ausgabe (Konzept):**
  1.  Das Kind startet und gibt einige Male die PID des ursprünglichen Elternprozesses aus (z.B. "Mein Elternprozess ist 12345").
  2.  Nach 2 Sekunden beendet sich der Elternprozess.
  3.  Das Kind wird dadurch zum **Waisenprozess (Orphan Process)**. Das Betriebssystem (unter macOS typischerweise `launchd` mit PID 1) "adoptiert" das Kind.
  4.  Bei den nächsten Aufrufen von `getppid()` in der Schleife wird das Kind nun die PID des neuen Elternprozesses (wahrscheinlich 1) zurückbekommen und ausgeben (z.B. "Mein Elternprozess ist 1").
  5.  Das Kind beendet seine Schleife und gibt die Endmeldung aus.
  6.  Der `system(buf)`-Aufruf am Ende ist aufgrund der Variablenproblematik fehleranfällig oder führt zu unerwartetem Verhalten.

2. Starten Sie `ProcA5.e`: der Elternprozess terminiert: was geschieht mit dem Kind?

✅ **Lösung**:

Ausgabe:

```bash
... ich bin das Kind 43341
Mein Elternprozess ist 43340
Mein Elternprozess ist 43340
Mein Elternprozess ist 43340
Mein Elternprozess ist 43340

.../work/Prozesse_und_Threads/Aufgabe05 on  main [!?] via  / v17.0.0-clang took 2s
󰥭 Mein Elternprozess ist 1
Mein Elternprozess ist 1
Mein Elternprozess ist 1
Mein Elternprozess ist 1
Mein Elternprozess ist 1
Mein Elternprozess ist 1
... so das wars


*** and here my new parent ****

  PID TTY           TIME CMD
    1 ??        15:12.26 /sbin/launchd
^C
```

- Der Kindprozess startet und gibt zuerst die PID des ursprünglichen Elternprozesses aus (z.B. 43025).
- Nachdem sich der Elternprozess nach 2 Sekunden beendet hat, wird das Kind zum Waisenkind (Orphan Process).
- Das Betriebssystem (macOS) weist dem Kind einen neuen Elternprozess zu, typischerweise `launchd` mit PID 1.
- Die nachfolgenden Ausgaben des Kindes zeigen korrekt die PID des neuen Elternprozesses (`Mein Elternprozess ist 1`).
- Das Kind beendet seine Schleife und führt am Ende `ps -p 1` aus, was die Details des neuen Elternprozesses (`launchd`) anzeigt.
- Dieses Verhalten (Änderung der Eltern-PID zu 1 nach Terminierung des ursprünglichen Elternteils) demonstriert genau, was mit einem Waisenkind geschieht: Es wird vom System adoptiert und läuft weiter, bis es seine Arbeit beendet hat.

3. Was geschieht, wenn der Kindprozess vor dem Elternprozess terminiert? Ändern Sie dazu im `sleep()` Befehl die Zeit von 2 Sekunden auf 12 Sekunden und verfolgen Sie mit top das Verhalten der beiden Prozesse, speziell auch die Spalte S.

✅ **Lösung**:

- Wenn das Kind terminiert, während der Elternprozess noch läuft (in diesem Fall schläft er noch), aber `wait()` noch nicht aufgerufen hat, wird das Kind zu einem **Zombie-Prozess**.
- Ein Zombie-Prozess ist beendet, existiert aber noch als Eintrag in der Prozesstabelle, damit der Elternprozess seinen Exit-Status abfragen kann.
- In `htop` sieht man den Kindprozess wahrscheinlich mit dem Status 'Z' (Zombie) oder als `<defunct>`.
- Der Zombie verbleibt in diesem Zustand, bis der Elternprozess entweder `wait()` aufruft (was in `ProcA5.c` nicht geschieht) oder selbst terminiert. Erst dann wird der Zombie vom System endgültig entfernt.

---

### 3.6 Aufgabe 6: Terminierte, halbtote Prozesse (Zombies)

**Ziele**

- Verstehen, was ein Zombie ist.
- Eine Möglichkeit kennenlernen, um Zombies zu verhindern.

**Aufgaben**

1. Analysieren Sie das Programm `ProcA6.c`.
2. Starten Sie das Script `mtop` bzw. `mtop aaaa.e`. Es stellt das Verhalten der Prozesse dynamisch dar.

**Hinweis:** `<defunct>` = Zombie.

✅ **Lösung**:

Ausgabe von mtop:

```
PID TTY           TIME CMD
41856 ttys001    0:00.03 login -fp mario
41857 ttys001    0:00.34 -bash
43725 ttys001    0:00.05 /bin/bash ./mtop
43881 ttys001    0:00.00 ps -a
39009 ttys005    0:00.07 /opt/homebrew/bin/bash --init-file /Applications/Visual Studio Code.app/Contents/Resources/app/out/vs/w
```

3. Starten Sie `aaaa.e` und verfolgen Sie im `mtop`-Fenster was geschieht. Was beachten Sie?

✅ **Lösung**:

- Es startet `./aaaa.e` (den Elternprozess, PID 44229). Dieser erzeugt schnell drei Kinder (44230, 44231, 44232).
- Das erste Kind (44230) beendet sich nach 2 Sekunden (`sleep(2)`). Da der Elternprozess zu diesem Zeitpunkt noch seine 8 Sekunden schläft und `wait()` noch nicht aufgerufen hat, wird das Kind 44230 zu einem Zombie. `mtop` zeigt dies korrekt als `<defunct>` an.
- Kurz darauf beenden sich auch die anderen Kinder (nach 3 und 4 Sekunden) und werden ebenfalls zu Zombies, da der Elternprozess immer noch schläft. (Deine Momentaufnahme zeigt nur einen Zombie, aber es ist wahrscheinlich, dass kurz danach alle drei `<defunct>` waren).
- Nachdem der Elternprozess seine 8 Sekunden Schlaf beendet hat, führt er die `wait()`-Aufrufe aus. Jeder `wait()`-Aufruf "sammelt" einen der Zombie-Prozesse ein und entfernt ihn aus der Prozesstabelle.
- Sobald alle drei Kinder eingesammelt wurden, verschwinden die `<defunct>`-Einträge aus `mtop`, und kurz darauf beendet sich auch der Elternprozess. Die `mtop`-Anzeige normalisiert sich wieder.

4. In gewissen Fällen will man nicht auf die Terminierung eines Kindes mit `wait()`, bzw. `waitpid()` warten. Überlegen Sie sich, wie Sie in diesem Fall verhindern können, dass ein Kind zum Zombie wird.

✅ **Lösung**:

Wenn man nicht explizit mit `wait()` oder `waitpid()` auf ein Kind warten möchte (z.B. weil der Elternprozess andere Dinge tun soll und nicht blockieren darf), gibt es hauptsächlich diese Möglichkeiten, Zombies zu verhindern:

- **`SIGCHLD` ignorieren:** Der Elternprozess kann dem System mitteilen, dass er nicht am Exit-Status seiner Kinder interessiert ist. Dies geschieht, indem der Signal-Handler für `SIGCHLD` (das Signal, das der Elternteil erhält, wenn ein Kind endet) auf `SIG_IGN` (Ignore) gesetzt wird.

  ```c
  #include <signal.h>
  // Im Elternprozess:
  signal(SIGCHLD, SIG_IGN);
  ```

  Wenn `SIGCHLD` ignoriert wird, räumt der Kernel terminierte Kindprozesse automatisch auf, ohne dass sie zu Zombies werden.

- **Double Fork:** Eine Technik, bei der der ursprüngliche Prozess (P) ein Kind (C1) erzeugt. C1 erzeugt sofort ein weiteres Kind (C2, den eigentlichen "Worker") und beendet sich dann sofort. P wartet auf C1 (was sehr schnell geht). C2 läuft nun weiter, aber sein Elternteil (C1) existiert nicht mehr. C2 wird zum Waisenkind, wird von `init`/`launchd` (PID 1) adoptiert, und `init`/`launchd` kümmert sich korrekt um das Aufräumen von C2, wenn dieser terminiert. Der ursprüngliche Prozess P ist damit nicht mehr für das Warten auf C2 verantwortlich.

- **Nicht-blockierendes `waitpid()`:** Man kann `waitpid()` mit der Option `WNOHANG` aufrufen. `waitpid(-1, &status, WNOHANG)` prüft, ob _irgendein_ Kind beendet wurde, blockiert aber nicht, wenn kein Kind beendet ist (gibt dann 0 zurück). Der Elternprozess kann dies periodisch aufrufen, um beendete Kinder "abzuernten", ohne seine Hauptarbeit zu unterbrechen.

---

### 3.7 Aufgabe 7: Auf Terminieren von Kindprozessen warten

**Vorbemerkung:** Diese Aufgabe verwendet Funktionen welche erst in der Vorlesung über _Inter-Process-Communication (IPC)_ im Detail behandelt werden.

Sie können diese Aufgabe bis dann aufsparen oder die verwendeten Funktionen selber via `man` Pages im benötigten Umfang kennenlernen: `man 2 kill` und `man 7 signal`.

**Ziele**

- Verstehen, wie Informationen zu Kindprozessen abgefragt werden können.
- Die Befehle `wait()` und `waitpid()` verwenden können.

**Aufgaben**

1. Starten Sie das Programm `ProcA7.e` und analysieren Sie wie die Ausgabe im Hauptprogramm zustande kommt und was im Kindprozess `ChildProcA7.c` abläuft.

✅ **Lösung**:

- Im Elternprozess (`ProcA7.c`) wird ein Kindprozess geforkt. Ohne Argument ist `whatToDo = 0`. Der Elternprozess ruft `wait(&status)` auf, um auf das Kind zu warten.
- Der Kindprozess (`ChildProcA7.c`) erhält über `execl` den Wert 0 für `whatToDo`. Im `switch`-Statement führt er `case 0: exit(0);` aus. Er beendet sich also sofort mit dem Exit-Status 0 (normaler Exit).
- Der `wait()`-Aufruf im Elternprozess kehrt zurück. `WIFEXITED(status)` ist wahr, und `WEXITSTATUS(status)` liefert 0. Der Elternprozess gibt daher aus: "Child exits with status 0" (Diese Ausgabe hast du nicht explizit gezeigt, aber sie wäre das Ergebnis dieses Falls).

2. Starten Sie `ProcA7.e` und danach nochmals mit `1` als erstem Argument. Dieser Argument Wert bewirkt, dass im Kindprozess ein ”Segmentation Error” erzeugt wird, also eine Speicherzugriffsverletzung. Welches Signal wird durch die Zugriffsverletzung an das Kind geschickt? Diese Information finden Sie im Manual mit `man 7 signal`.

✅ **Lösung**:

- Der Kindprozess erhält `whatToDo = 1` und führt im `switch` den Code `*a = i;` aus. Da `a` ein Null-Zeiger ist, verursacht dies eine Speicherzugriffsverletzung (Segmentation Fault).
- Eine Speicherzugriffsverletzung sendet normalerweise das Signal **`SIGSEGV`** an den Prozess. Auf den meisten Linux-Systemen hat `SIGSEGV` den numerischen Wert **11**.
- Deine Ausgabe "Child exits on signal 11" bestätigt dies. `WIFSIGNALED(status)` war wahr, und `WTERMSIG(status)` lieferte 11.

Bei einer Zugriffsverletzung kann das System so konfiguriert werden, dass ein core Dump erstellt wird. Ein core Dump ist ein Abbild des Speichers z.B. zum Zeitpunkt, wenn das Programm abstürzt (wie oben mit der Speicher Zugriff Verletzung). Der Dump wird üblicherweise File **core** abgelegt und kann mit dem **gdb** (GNU-Debugger) gelesen werden (siehe auch `README`).
Schalten Sie nun core dump ein (siehe auch `README`) uns analysieren Sie die Ausgabe:

- `ulimit -c unlimited`
- `sudo /bin/bash -c 'echo core > /proc/sys/kernel/core_pattern'`
- `gdb ChildProcA7.e core.*`
- innerhalb von **gdb**
  _ `where`
  _ `list`
  Damit sie den Ort des Absturzes sehen. Mit `quit` verlassen Sie **gdb** wieder.
  Dokumentieren Sie hier Ihre Beobachtungen zu `ProcA7.e 1`:

✅ **Lösung**:

- Ich habe versucht, Core Dumps zu aktivieren (`ulimit -c unlimited`, `/proc/sys/kernel/core_pattern`).
- Die Ausgabe des Elternprozesses "Child exits with core dump 128" deutet darauf hin, dass das System _versucht_ hat oder _konfiguriert_ ist, einen Core Dump zu erstellen (der Wert 128 wird oft durch Addition von 128 zur Signalnummer gebildet, wenn ein Core Dump erzeugt wird, aber das ist nicht standardisiert; wichtiger ist hier, dass `WCOREDUMP(status)` einen Wert ungleich 0 lieferte, was oft, aber nicht immer, bedeutet, dass ein Dump erzeugt wurde).
- Allerdings konnte `gdb` die Datei `core.*` nicht finden (`No such file or directory`). Das bedeutet, dass entweder kein Core Dump geschrieben wurde (trotz `WCOREDUMP` ungleich 0, vielleicht wegen Berechtigungen, Speicherplatz oder anderer Systemkonfigurationen wie `systemd-coredump`) oder er an einem anderen Ort gespeichert wurde.
- **Deine Beobachtung:** Das Kind wurde durch Signal 11 (`SIGSEGV`) beendet. Der Versuch, einen Core Dump zu analysieren, schlug fehl, da die Core-Datei nicht gefunden wurde, obwohl `WCOREDUMP` einen Wert != 0 lieferte. Ohne Core-Datei können `where` und `list` in `gdb` den Absturzort nicht anzeigen.

3. Wenn Sie `ProcA7.e 2` starten, sendet das Kind das Signal 30 an sich selbst. Was geschieht?

✅ **Lösung**:

- Das Kind erhält `whatToDo = 2` und führt `kill(getpid(), 30);` aus, sendet also Signal 30 an sich selbst.
- Die Ausgabe "Child exits on signal 30" und "Child exits with core dump 0" zeigt, dass das Kind durch dieses Signal beendet wurde (`WIFSIGNALED` war wahr, `WTERMSIG` war 30). Es wurde kein Core Dump erzeugt (`WCOREDUMP` war 0).

4. Wenn Sie `ProcA7.e 3` starten, sendet ProcA7.e das Signal SIGABRT (abort) an das Kind: was geschieht in diesem Fall?

✅ **Lösung**:

- Das Kind erhält `whatToDo = 3` und legt sich schlafen (`sleep(5)`).
- Der Elternprozess sendet nach 1 Sekunde das Signal `SIGABRT` an das Kind (`kill(pid, SIGABRT)`). `SIGABRT` hat auf Linux typischerweise den Wert **6**.
- Deine Ausgabe "Child exits on signal 6" und "Child exits with core dump 128" bestätigt, dass das Kind durch `SIGABRT` (Signal 6) beendet wurde. Der Wert 128 für `WCOREDUMP` deutet darauf hin, dass bei Abbruch durch `SIGABRT` ein Core Dump erstellt wurde.

5. Mit `ProcA7.e 4` wird das Kind gestartet und terminiert nach 5 Sekunden. Analysieren Sie wie in ProcA7.e der Lauf- bzw. Exit-Zustand des Kindes abgefragt wird (siehe dazu auch `man 3 exit`).

✅ **Lösung**:

- Das Kind erhält `whatToDo = 4`, schläft 5 Sekunden und beendet sich dann normal mit `exit(222)`.
- Der Elternprozess verwendet eine Schleife mit `while (!waitpid(pid, &status, WNOHANG))`. `WNOHANG` bewirkt, dass `waitpid` nicht blockiert.
  - Solange das Kind läuft (schläft), gibt `waitpid` 0 zurück. Die `while`-Bedingung (`!0`) ist wahr, und der Elternprozess gibt ". . . child is playing" aus und wartet kurz.
  - Sobald das Kind `exit(222)` aufruft, gibt `waitpid` beim nächsten Aufruf die PID des Kindes (>0) zurück. Die `while`-Bedingung (`!PID`) ist falsch, die Schleife endet.
- Der Elternprozess gibt dann "Child has exited with 'exit(222)'" aus, wobei 222 der Wert ist, den `WEXITSTATUS(status)` nach einem normalen Exit des Kindes liefert.
- Deine Ausgabe mit den wiederholten ". . . child is playing"-Meldungen gefolgt von der korrekten Exit-Status-Meldung zeigt exakt dieses Verhalten der nicht-blockierenden Abfrage mittels `waitpid` und `WNOHANG`.

---

### 3.8 Aufgabe 8: Kindprozess als Kopie des Elternprozesses

**Ziele**

- Verstehen, wie Prozessräume vererbt werden.
- Unterschiede zwischen dem Prozessraum von Eltern und Kindern erfahren.

**Aufgaben**

1. Analysieren Sie Programm `ProcA8_1.c`: was gibt das Programm aus?

- Starten Sie `ProcA8_1.e `und überprüfen Sie Ihre Überlegungen.
- Waren Ihre Überlegungen richtig? Falls nicht, was könnten Sie falsch überlegt haben?

✅ **Lösung**:

Ausgabe:

```
Hallo, I am on the way to fork now, ......look: I am the parent

clear ?

Hallo, I am on the way to fork now, ......look: I am the child

clear ?
```

- **Vermutung:**
  Der `printf`-Aufruf vor `fork` wird gepuffert, und beide Prozesse geben den Pufferinhalt später aus, was zu einer Verdoppelung des ersten Teils der Ausgabe führt.
- **Erklärung der Ausgabe:**
  Der erste `printf` ohne `\n` bleibt im `stdout`-Puffer. `fork()` dupliziert den Prozess _inklusive_ dieses Puffers. Beide Prozesse leeren später ihren Puffer (durch `\n` oder `exit`), was dazu führt, dass der gepufferte Text von beiden ausgegeben wird, gefolgt von ihrer jeweiligen spezifischen Meldung und der "clear ?"-Zeile.

2. Analysieren Sie Programm `ProcA8_2.c`: was gibt das Programm aus?

- Starten Sie `ProcA8_2.e` und überprüfen Sie Ihre Überlegungen.
- Waren Ihre Überlegungen richtig? Falls nicht, was könnten Sie falsch gemacht haben?
- Kind und Eltern werden in verschiedener Reihenfolge ausgeführt: ist ein Unterschied ausser der Reihenfolge festzustellen?

✅ **Lösung**:

Ausgabe:

```
- - - - - - - -
- - - - - - - -
- - - - - - - -
- - - - - - - -
- - - - - - - -
- - - - - - - -
- - - - - - - -
- - - - - - - -

Elternarray

p p p p p p p p
p p p p p p p p
p p p p p p p p
p p p p p p p p
- - - - - - - -
- - - - - - - -
- - - - - - - -
- - - - - - - -

Kinderarray

- - - - - - - -
- - - - - - - -
- - - - - - - -
- - - - - - - -
c c c c c c c c
c c c c c c c c
c c c c c c c c
c c c c c c c c
```

- **Vermutung:**
  Das globale Array `GArray` wird von Eltern- und Kindprozess geteilt, die Ausgabe sollte eine Mischung der Änderungen von beiden zeigen.
- **Erklärung der Ausgabe:**
  `fork()` erzeugt separate Adressräume. Eltern- und Kindprozess arbeiten nach dem `fork()` mit **unabhängigen Kopien** des globalen Arrays `GArray`. Änderungen des einen sind für den anderen nicht sichtbar. Deshalb zeigt das "Elternarray" nur die 'p'-Änderungen und das "Kinderarray" nur die 'c'-Änderungen. Die Ausgabe bestätigt separate Speicherbereiche. Die Reihenfolge der Ausgabe ("Elternarray" vs. "Kinderarray") kann variieren, der Inhalt der jeweiligen Arrays aber nicht.

3. Analysieren Sie Programm `ProcA8_3.c` und Überlegen Sie, was in die Datei `AnyOutPut.txt` geschrieben wird, wer schreibt alles in diese Datei (sie wird ja vor `fork()` geöffnet) und wieso ist das so?

- Starten Sie `ProcA8_3.e` und überprüfen Sie Ihre Überlegungen.
- Waren Ihre Überlegungen richtig? Falls nicht, wieso nicht?

✅ **Lösung**:

Ausgabe:

```
We are done
We are done
See file AnyOutPut.txt
```

- **Vermutung:**
  Beide Prozesse schreiben in dieselbe Datei, was zu einer zufällig vermischten Ausgabe von "Mami"- und "Fritzli"-Zeilen führt.
- **Erklärung der Ausgabe:**
  Die Datei wird _vor_ dem `fork()` geöffnet. Beide Prozesse erben Kopien des File Pointers, die aber auf dieselbe _Open File Description_ im Kernel zeigen (inklusive des Datei-Offsets). Daher schreiben beide in dieselbe Datei und beeinflussen denselben Schreib-Offset. Das Resultat in `AnyOutPut.txt` ist eine **interleaved (vermischte)** Ausgabe der Zeilen beider Prozesse. Die genaue Reihenfolge ist nicht-deterministisch, aber alle 30 Zeilen sollten vorhanden sein. (Die Terminalausgabe ist wie erwartet, der Inhalt von `AnyOutPut.txt` müsste zur Bestätigung geprüft werden).

---

### 3.9 Aufgabe 9: Unterschied von Threads gegenüber Prozessen

**Ziele**

- Den Unterschied zwischen Thread und Prozess kennenlernen.
- Problemstellungen um Threads kennenlernen.
- Die `pthread`-Implementation kennen lernen.

**Aufgaben**

1. Studieren Sie Programm `ProcA9.c` und überlegen Sie, wie die Programmausgabe aussieht. Vergleichen Sie Ihre Überlegungen mit denjenigen aus Aufgabe 8.2 b) (`Pro-cA8_2.e`).

- Starten Sie `ProcA9.e` und vergleichen das Resultat mit Ihren Überlegungen.
- Was ist anders als bei `ProcA8_2.e`?

✅ **Lösung**:

- **Vermutung zur Ausgabe von `ProcA9.c`:**
  Ähnlich wie bei Prozessen wird vielleicht auch hier der globale Speicher kopiert, und jeder Thread sieht nur seine eigenen Änderungen? Oder die Threads überschreiben sich gegenseitig, und das Ergebnis ist chaotisch.

- **Erklärung der Ausgabe von `ProcA9.c`:**
  Der wesentliche Unterschied zu `ProcA8_2.c` (mit `fork`) ist, dass `pthread_create` _Threads_ erzeugt, die **innerhalb desselben Prozesses** laufen und sich daher denselben **globalen Speicher teilen**. Das globale Array `GArray` existiert nur einmal. Beide Threads modifizieren _dasselbe_ Array. Thread 'p' schreibt in die untere Hälfte, Thread 'c' in die obere. Wenn ein Thread das Array druckt, nachdem beide ihre Änderungen gemacht haben, sieht er das kombinierte Ergebnis.
  Die Ausgabe des Haupt-Threads nach `pthread_join` (also nachdem garantiert beide Threads fertig sind) zeigt daher das Array mit 'p's unten und 'c's oben.

- **Was ist anders als bei `ProcA8_2.e`?:**
  Der Hauptunterschied ist der **geteilte Speicherbereich** bei Threads im Gegensatz zu den **separaten Speicherbereichen** bei Prozessen nach `fork()`. In `ProcA8_2.e` sahen Eltern- und Kindprozess nur ihre _eigenen_ Änderungen am Array. In `ProcA9.e` sehen beide Threads (und der Hauptthread) das _gemeinsame_, von beiden modifizierte Array. Die "zerrissene" Ausgabe der Threads deutet auf die gleichzeitigen Schreibzugriffe auf `stdout` hin.

2. Setzen Sie in der Thread-Routine vor dem Befehl `pthread_exit()` eine unendliche Schleife ein, z.B. `while(1) { }`; .

   - Starten Sie das Programm und beobachten Sie das Verhalten mit `top`. Was beobachten Sie und was schliessen Sie daraus?

     **Hinweis:** wenn Sie in `top` den Buchstaben H eingeben, werden die Threads einzeln dargestellt.

   - Kommentieren Sie im Hauptprogram die beiden `pthread_join()` Aufrufe aus und starten Sie das Programm. Was geschieht? Erklären Sie das Verhalten.

✅ **Lösung**:

**Endlosschleife in Threads und `top`:**

- **Beobachtung/Schlussfolgerung (Was man in `top`/`htop` sehen sollte):**
  - Der Prozess `ProcA9.e` würde eine hohe CPU-Last zeigen (potenziell nahe 200% auf einem System mit mind. 2 Kernen).
  - Mit der Thread-Ansicht ('H' in `top` oder Standard in `htop`) würde man sehen, dass nicht der Hauptprozess, sondern die **zwei Worker-Threads** die hohe CPU-Last verursachen (jeder lastet einen Kern aus), da sie in der Endlosschleife `while(1){}` feststecken.
  - Der Hauptthread würde in `pthread_join` warten und kaum CPU verbrauchen.
  - Das Programm würde keine Array-Ausgaben mehr machen und nicht terminieren, bis es manuell (z.B. mit Ctrl+C) abgebrochen wird. Dies zeigt, dass Threads als eigenständige Einheiten CPU-Zeit erhalten und eine Endlosschleife einen Thread (und damit potenziell den ganzen Prozess, wenn auf ihn gewartet wird) blockieren kann.

**`pthread_join` auskommentiert:**

- **Was geschieht?:** Der Hauptthread startet die beiden Worker-Threads, **wartet aber nicht** auf deren Beendigung, da `pthread_join` fehlt. Er fährt sofort fort, gibt die Zeile "... nach Threads" und den aktuellen Zustand von `GArray` aus und beendet sich dann (Funktion `main` endet).
- **Erklärung des Verhaltens:** Ohne `pthread_join` gibt es keine Garantie, dass die Worker-Threads ihre Arbeit (das Füllen des Arrays) abgeschlossen haben, bevor der Hauptthread die finale Ausgabe macht und sich beendet. Es ist ein Wettlauf (Race Condition):
  - _Möglicherweise_ sind die Threads sehr schnell und füllen das Array, bevor der Hauptthread zur finalen Ausgabe kommt (wie es in deiner Ausgabe aussieht).
  - _Wahrscheinlicher_ (oder zumindest möglich auf anderen Systemen/unter anderer Last) ist, dass der Hauptthread das Array ausgibt, während es noch unvollständig ist (nur '-', nur 'p', nur 'c', oder teilweise gemischt).
  - Sobald der Hauptthread endet, werden normalerweise auch alle seine zugehörigen Threads beendet, möglicherweise mitten in ihrer Arbeit.
    `pthread_join` ist also notwendig, um die Ausführung zu synchronisieren und sicherzustellen, dass die Ergebnisse der Threads verfügbar sind, bevor der Hauptthread weitermacht oder sich beendet. Das Fehlen führt zu nicht-deterministischem Verhalten bezüglich des finalen Zustands von `GArray` zum Zeitpunkt der Ausgabe durch den Hauptthread.

---

### 3.10 Aufgabe 10 (optional):

#### 3.10.1 Übersicht

Dieser Teil des Praktikums behandelt spezielle Prozesse: die Dämon Prozesse («daemon pro-cesses»). Es ist gedacht als Zusatz zum Basis Praktikum über Prozesse und Threads.

Auch dieser Teil ist ein «Analyse»- und «Experimentier»-Praktikum.

---

##### 3.10.1.1 Nachweis

Dieses Praktikum ist eine leicht abgewandelte Variante des ProcThreads Praktikum des Moduls BSY, angepasst an die Verhältnisse des SNP Moduls. Die Beispiele und Beschreibungen wurden, wo möglich, eins-zu-ein übernommen.

Als Autoren des BSY Praktikums sind genannt: M. Thaler, J. Zeman.

---

#### 3.10.2 Lernziele

In diesem Praktikum werden Sie sich mit Dämon Prozessen beschäftigen.

- Sie können die Problemstellung der Dämon Prozesse erklären
- Sie können einen Dämon Prozess kreieren
- Sie können aus dem Dämon Prozess mit der Umgebung kommunizieren
- ***

#### 3.10.3 Aufgabe: Dämon Prozesse

**Ziele**

- Problemstellungen um Daemons kennenlernen:
  - wie wird ein Prozess zum Daemon?
  - wie erreicht man, dass nur ein Daemon vom gleichen Typ aktiv ist?
  - wie teilt sich ein Daemon seiner Umwelt mit?
  - wo "lebt" ein Daemon?

**Einleitung**

Für diese Aufgabe haben wir einen Daemon implementiert: **MrTimeDaemon** gibt auf Anfrage die Systemzeit Ihres Rechners bekannt. Abfragen können Sie diese Zeit mit dem Programm `WhatsTheTimeMr localhost`. Die Kommunikation zwischen den beiden Prozessen haben wir mit TCP/IP Sockets implementiert. Weitere Infos zum Daemon finden Sie nach den Aufgaben.

Im Abschnitt 4 finden Sie Zusatzinformationen über diese Implementation eines Dämon Prozesses plus weiterführende Informationen.

**Aufgaben**

1. Für die folgende Aufgabe benötigen Sie mindestens zwei Fenster (Kommandozeilen-Konsolen). Übersetzen Sie die Programme mit `make` und starten Sie das Programm **PlapperMaul** in einem der Fenster. Das Programm schreibt (ca.) alle 0.5 Sekunden _Hallo, ich bins.... Pidi_ plus seine Prozess-ID auf den Bildschirm. Mit dem Shell Befehl `ps` können Sie Ihre aktiven Prozesse auflisten, auch **PlapperMaul**. Überlegen Sie sich zuerst, was mit **PlapperMaul** geschieht, wenn Sie das Fenster schliessen: läuft **PlapperMaul** weiter? Was geschieht mit **PlapperMaul** wenn Sie sich ausloggen und wieder einloggen? Testen Sie Ihre Überlegungen, in dem Sie die entsprechenden Aktionen durchführen. Stimmen Ihre Überlegungen?

```

```

2. Starten Sie nun das Programm bzw. den Daemon **MrTimeDaemon**. Stellen Sie die gleichen Überlegungen an wie mit **PlapperMaul** und testen Sie wiederum, ob Ihre Überlegungen stimmen. Ob **MrTimeDaemon** noch läuft können Sie feststellen, indem Sie die Zeit abfragen oder den Befehl `ps ajx | grep MrTimeDaemon` eingeben: was fällt Ihnen am Output auf? Was schliessen Sie aus Ihren Beobachtungen?

```

```

3. Starten Sie **MrTimeDaemon** erneut, was geschieht?

```

```

4. Stoppen Sie nun **MrTimeDaemon** mit `killall MrTimeDaemon`.
5. Starten Sie **MrTimeDaemon** und fragen Sie mit `WhatsTheTimeMr localhost` oder mit `WhatsTheTimeMr 127.0.0.1` die aktuelle Zeit auf Ihrem Rechner ab.

**Optional:**
Fragen Sie die Zeit bei einem Ihrer Kollegen ab. Dazu muss beim Server (dort wo **MrTimeDaemon** läuft) ev. die Firewall angepasst werden. Folgende Befehle müssen dazu mit **root-Privilegien** ausgeführt werden:

```bash
iptables-save > myTables.txt # sichert die aktuelle Firewall
iptables -I INPUT 1 -p tcp --dport 65534 -j ACCEPT
iptables -I OUTPUT 2 -p tcp --sport 65534 -j ACCEPT
```

Nun sollten Sie über die IP-Nummer oder über den Rechner-Namen auf den **TimeServer** mit `WhatsTheTimeMr` zugreifen können.
Die Firewall können Sie mit folgendem Befehl wiederherstellen:

```bash
iptables-restore myTables.txt
```

6. Studieren Sie `MrTimeDaemon.c`, `Daemonizer.c` und `TimeDaemon.c` und analysieren Sie, wie die Daemonisierung abläuft. Entfernen Sie die Kommentare im Macro `Out-PutPIDs` am Anfang des Moduls `Daemonizer.c`. Übersetzen Sie die Programme mit make und starten Sie `MrTimeDaemon` erneut. Analysieren Sie die Ausgabe, was fällt Ihnen auf? Notieren Sie alle für die vollständige Daemonisierung notwendigen Schritte.

   ```


   ```

7. Setzen Sie beim Aufruf von `Daemonizer()` in `MrTimeDaemon.c` anstelle von `lock-FilePath` den Null-Zeiger `NULL` ein. Damit wird keine lock-Datei erzeugt. Übersetzen Sie die Programme und starten Sie erneut `MrTimedaemon`. Was geschieht bzw. wie können Sie feststellen, was geschehen ist?

   **Hinweis:** lesen Sie das log-File: `/tmp/timeDaemon.log.`

   ```


   ```

   Wenn Sie noch Zeit und Lust haben: messen Sie die Zeit, zwischen Start der Zeitanfrage und Eintreffen der Antwort. Dazu müssen Sie die Datei `WhatsTheTimeMr.c` entsprechend anpassen.

---

#### 3.10.4 Zusatzinformationen

---

##### 3.10.4.1 Diese Implementation

Dieser Daemon besteht aus den 3 Komponenten.

**Hauptprogramm: MrTimeDaemon.c**

Hier werden die Pfade für die lock-Datei, die log-Datei und der ”Aufenthaltsort” des Daemons gesetzt. Die lock-Datei wird benötigt um sicherzustellen, dass der Daemon nur einmal gestartet werden kann. In die lock-Datei schreibt der Daemon z.B. seine PID und sperrt sie dann für Schreiben. Wird der Daemon ein zweites Mal gestartet und will seine PID in diese Datei schreiben, erhält er eine Fehlermeldung und terminiert (es soll ja nur ein Daemon arbeiten). Terminiert der Daemon, wird die Datei automatisch freigegeben. Weil Daemonen sämtliche Kontakte mit ihrer Umwelt im Normalfall abbrechen und auch kein Kontrollterminal besitzen, ist es sinnvoll, zumindest die Ausgabe des Daemons in eine log-Datei umzuleiten. Dazu stehen einige Systemfunktionen für Logging zur Verfügung. Der Einfachheit halber haben wir hier eine normale Datei im Verzeichnis `/tmp` gewählt.

> **Anmerkung:** die Wahl des Verzeichnisses `/tmp` für die lock- und log-Datei ist für den normalen Betrieb problematisch, weil der Inhalt dieses Verzeichnisses jederzeit gelöscht werden kann, bzw. darf. Wir haben dieses Verzeichnis gewählt, weil wir die beiden Dateien nur für die kurze Zeit des Praktikums benötigen.

Der Daemon erbt sein Arbeitsverzeichnis vom Elternprozesse, er sollte deshalb in ein festes Verzeichnis des Systems wechseln, um zu verhindern, dass er sich in einem montierten (gemounteten) Verzeichnis aufhält, das dann beim Herunterfahren nicht demontiert werden könnte (wir haben hier wiederum `/tmp` gewählt).

**Daemonizer: Daemonizer.c**

Der Daemonizer macht aus dem aktuellen Prozess einen Daemon. Z.B. sollte er Signale (eine Art Softwareinterrupts) ignorieren: wenn Sie die CTRL-C Taste während dem Ausführen eines Vordergrundprozess drücken, erhält dieser vom Betriebssystem das Signal SIGINT und bricht seine Ausführung ab. Weiter sollte er die Dateierzeugungsmaske auf 0 setzen (Dateizugriffsrechte), damit kann er beim Öffnen von Dateien beliebige Zugriffsrechte verlangen (die Dateierzeugungsmaske erbt er vom Elternprozess). Am Schluss startet der Daemonizer das eigentliche Daemonprogramm: TimeDaemon.e.

**Daemonprogramm: TimeDaemon.c**

Das Daemonprogramm wartet in einer unendlichen Schleife auf Anfragen zur Zeit und schickt die Antwort an den Absender zurück. Die Datenkommunikation ist, wie schon erwähnt, mit Sockets implementiert, auf die wir aber im Rahmen dieses Praktikums nicht weiter eingehen wollen (wir stellen lediglich Hilfsfunktionen zur Verfügung).

---

##### 3.10.4.2 Zusatzinformation zu Dämon Prozessen

Dämonen oder englisch Daemons sind eine spezielle Art von Prozessen, die vollständig unabhängig arbeiten, d.h. ohne direkte Interaktion mit dem Anwender. Dämonen sind Hintergrundprozesse und terminieren i.A. nur, wenn das System heruntergefahren wird oder abstürzt. Dämonen erledigen meist Aufgaben, die periodisch ausgeführt werden müssen, z.B. Überwachung von Systemkomponenten, abfragen, ob neue Mails angekommen sind, etc.

Ein typisches Beispiel unter Unix ist der Printer Daemon `lpd`, der periodisch nachschaut, ob ein Anwender eine Datei zum Ausdrucken hinterlegt hat. Wenn ja, schickt er die Datei auf den Drucker.

Hier wird eine weitere Eigenschaft von Daemons ersichtlich: meist kann nur ein Dämon pro Aufgabe aktiv sein: stellen Sie sich vor, was passiert, wenn zwei Druckerdämonen gleichzeitig arbeiten. Andererseits muss aber auch dafür gesorgt werden, dass ein Dämon wieder gestartet wird, falls er stirbt.

---

---

## 4. Bewertung

Die gegebenenfalls gestellten Theorieaufgaben und der funktionierende Programmcode müssen der Praktikumsbetreuung gezeigt werden. Die Lösungen müssen mündlich erklärt werden.

| Aufgabe | Kriterium                                                  | Punkte |
| :------ | :--------------------------------------------------------- | :----- |
|         | Sie können die gestellten Fragen erklären.                 |        |
| 1       | Prozess mit `fork()` erzeugen                              | 0.5    |
| 2       | Prozess mit `fork()` und `exec()`: Programm Image ersetzen | 0.5    |
| 3       | Prozesshierarchie analysieren                              | 0.5    |
| 4       | Zeitlicher Ablauf von Prozessen                            | 0.5    |
| 5       | Waisenkinder (Orphan Processes)                            | 0.5    |
| 6       | Terminierte, halbtote Prozesse (Zombies)                   | 0.5    |
| 7       | Auf Terminieren von Kindprozessen warten                   | 0.5    |
| 8       | Kindprozess als Kopie des Elternprozesses                  | 0.5    |
| 9       | Unterschied von Threads gegenüber Prozessen                | 0.5    |
| 10      | Dämon Prozesse                                             | (4)    |

---

Version: 11.01.2022
