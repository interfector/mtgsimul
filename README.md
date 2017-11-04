# mtgsimul

Programma per simulare partite di Magic: The Gathering (in goldfish) per l'analisi statistica di alcuni mazzi Combo facilmente pilotabili.

Mazzi per ora testati:
- Oops All Spells!
- Belcher Budget

Per ogni mazzo devono essere programmate strategie di Mulligan e strategie di calcolo del mana, da questi fattori (e altri) dipende l'analisi statistica.
Le funzioni su cui lavorare sono dunque 'mulliganFilter' e 'evaluateGame'. In entrambi i mazzi (Belcher e Oops All Spells!) la 'evaluateGame' si basa sul mero
calcolo del mana accessibile e non ha strategie nei turni conseguenti al 1° (gioca solo Gitaxian Probe e Street Wraith se può), nel caso si voglia implementare
un mazzo con Combo più complesse (che si estendono su più turni o usano effetti tipo Ponder) allora la 'evaluateGame' sarà necessariamente più complicata.

TODO :
- Implementare un piccolo simulatore interno che cerchi di giocare ogni carta che ha in mano (con strategie di vittoria)


TODO Mazzi:
-

