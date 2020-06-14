Blatt 3
=======

Aufgabe
-------

Implementieren Sie die folgende Scheduler:

* Round-Robin (RR)
* Priority mit Verdrängung (PRIOP)
* Last Come First Served ohne Verdrängung (LCFS)
* Shortest Remaining Time Next mit Verdrängung (SRTNP)
* Highest Response Ratio Next (HRRN)
* Multilevel Feedback (mit 4 Levels (Level $i = 0...3$) mit $\tau_{i} =2^{i}$)

Als Orientierungshilfe ist dafür die Implementierung eines "First Come First Served"-Schedulers in der Datei [FCFS.c](src/FCFS.c) bereits vorgegeben.
Ihre Lösungen sollen in den Dateien [RR.c](src/RR.c), [PRIOP.c](src/PRIOP.c), [LCFS.c](src/LCFS.c), [MLF.c](src/MLF.c), [HRRN.c](src/HRRN.c) und [SRTNP.c](src/SRTNP.c) erfolgen. Entsprechende Stellen sind mit dem Kommentar "//TODO" gekennzeichnet. Sie können natürlich eigene Funktionen auch außerhalb der mit "//TODO" gekennzeichneten Funktionen erstellen. Außerdem können Sie auch die entsprechenden Headerfiles ([RR.h](lib/RR.h), [PRIOP.h](lib/PRIOP.h) usw.) erweitern (siehe Hinweise in entsprechenden Files).

Programmaufruf
--------------

Beim Aufruf des Programms soll als ersten Parameter die Abkürzung eines Namens einer Schedulingstrategie übergeben werden (also RR, PRIOP, FCFS, ...). Danach folgen immer Gruppen von drei Zahlen, die die Eigenschaften der Prozesse bestimmen: Ankunftszeit, Dauer, Priorität. Bei Round-Robin kann zusätzlich als letzen Parameter noch die Länge der Zeitscheibe angegeben werden. Der Standardwert ist ansonsten 2.

### Beispiele 

Wir betrachten folgende drei Prozesse:

  * A: Ankunft 0, Dauer 4, Priorität 2
  * B: Ankunft 3, Dauer 3, Priorität 5
  * C: Ankunft 5, Dauer 1, Priorität 1

#### Beispiel 1
Aufruf dieser drei Prozesse mit "First Come First Served"-Scheduler

    $ ./scheduler FCFS 0 4 2 3 3 5 5 1 1

Dies führt zur Ausgabe
    
    Starting FCFS scheduler
    | A | A | A | A | B | B | B | C |


#### Beispiel 2
Aufruf dieser drei Prozesse mit "Round-Robin"-Scheduler mit Zeitscheibe 3

    $ ./scheduler RR 0 4 2 3 3 5 5 1 1 3

Dies führt zur Ausgabe
    
    Starting RR scheduler
    | A | A | A | B | B | B | A | C |


Programmablauf
--------------

Die Vorgabe verarbeitet bereits die übergeben Parameter, kümmert sich um die Ausgabe der Prozesse und startet ihren Ablauf. Sie müssen nur noch um die Funktionalität der Scheduler kümmern. Dabei sollen sie zu jedem Scheduler die vier Funktionen bearbeiten. Informationen, was diese Funktionen tun sollen, sind jeweils in den header-files vermerkt. Es handelt sich jeweils um äquivalente Funktionen, die aber je nach scheduler unterschiedlich aussehen können.

Zuerst wird immer die Funktion __int xx_startup()__. In dieser können sie alles tun, was einmalig notwendig ist, um die nachfolgenden Funktionen auszuführen, zum Beispiel Variablen initialisieren o.Ä.
Dann werden in einer Schleife die Funktionen __process* xx_new_arrival(process* arriving_process, process* running_process)__ und __process* xx_tick (process* running_process)__ aufgerufen.

Wenn zu einem bestimmten Zeitpunkt ein neuer Prozess ankommt, dann wird ein Pointer auf ein process-Objekt (siehe [process.h](./lib/process.h) für weitere Informationen) in __xx_new_arrival__ als Parameter *arriving_process* übergeben. Andernfalls ist dieser Parameter *NULL*. Der Pointer auf *running_process*, der in diesen beiden Funktionen übergeben wird ist jeweils der zuletzt aktive Prozess (oder NULL, falls kein Prozess aktiv war). Die Funktion __xx_new_arrival__ soll, falls es einen neu ankommenden Prozess gibt, diesen verarbeiten (zum Beispiel einer Queue hinzufügen). Der Rückgabewert soll wieder ein Pointer auf den jetzt aktuellen zu bearbeitenden Prozess sein (falls dieser getauscht wurde) oder auf den alten *running_process*. 

Danach wird die Funktion __xx_tick__ aufgerufen. In dieser soll geprüft werden, ob der aktuelle Prozess gewechselt werden muss (zum Beispiel, weil seine Bearbeitungsdauer abgeschlossen ist).
Es soll ein Pointer zum nun aktuell zu bearbeitenden Prozess zurückgegeben werden. Dies ist dann der Prozess, der auch in der Ausgabe auf dem Bildschirm erscheint. 

Nachdem alle Prozesse bearbeitet wurden, wird einmalig die Funktion __void xx_finish()__ aufgerufen. Hier kann alles erledeigt werden, was es noch zu erledigen gibt (Speicher freigeben etc.).

Hilfen/Hinweise
---------------
### Textausgabe auf der Konsole
Achten Sie darauf, keine zusätzlichen Zeichen auf der Standardausgabe im fertigen Programm auszugeben, da dies das automatische Testen erschwert. Zusätzliche Ausgaben werden dementsprechend mit Punktabzug bewertet.

### Queue
Es steht ihnen eine Implementierung einer Queue zur Verfügung. Die nutzbaren Funktionen dieser Queue sind in [queue.h](lib/queue.h) beschrieben. Diese Queue wird auch in der vorgegeben implementierung des FCFS-Schedulers verwendet. 

### Kompiliertes Programm
Die Vorgabe enthält das Programm __scheduler_vorgabe__. In diesem sind bereits alle Scheduler einprogrammiert. Sie können dieses Programm nutzen um die Ausgaben mit Ihrem Programm zu vergleichen. Falls sich das Programm nicht ausführen lässt, müssen sie es möglicherweise vorher ausführbar machen. Geben Sie dazu folgendes in die Konsole ein:

    $ chmod +x ./scheduler_vorgabe

### Valgrind
Achten Sie darauf, dass dynamisch allokierter Speicher wieder freigegeben wird. Der allokierte Speicher für die *process*-Objekte werden schon von der Vorgabe freigegeben. Geben sie also nur selbst allokierten Speicher frei.
Um zu prüfen, ob der Speicher komplett freigegeben wurde kann das Programm *valgrind* genutzt werden. Beispielaufruf:

    $ valgrind ./scheduler RR 0 4 2 3 3 5 5 1 1 3 

### Makefile
In der Vorgabe wird ein makefile mitgeliefert. Um das Projekt zu kompilieren rufen sie einfach im Ordner, in dem sich das makefile befindet

    $ make

auf. Sie müssen diese Datei nicht verändern. Sollten Sie allerdings weitere .c oder .h Files einbinden wollen, dann passen Sie das makefile entsprechend an. Es ist allerdings nicht nötig weitere Datein hinzuzufügen. *Ändern Sie nicht die Files __main.c__, __colors.c__ und __colors.h__*

### Farbausgbae
Die bunte Ausgabe soll Ihnen helfen, schneller einen Überblick über die Ausgabe zu bekommen. Falls Sie jedoch Probleme mit der Farbausgabe auf Ihrem Terminal haben, dann können Sie das Flag -c mitübergeben, um die farbige Ausgabe zu unterbinden:

    $ ./scheduler FCFS 0 4 2 3 4 2 -c

Ihr Projekt sollte ohne Warnungen und Fehlern auf den Uni-Rechnern mit Ubuntu 18 kompilieren und ausführbar sein. Testen Sie die Funktionalität, indem Sie per SSH auf einen Unirechner zugreifen und Ihr Programm dort testen. Informationen zur Nutzung von SSH finden Sie auf der Webseite der [Freitagsrunde](https://wiki.freitagsrunde.org/SSH).