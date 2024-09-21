# PCLP - Tema 2 - 2048

## Cerinte rezolvate

Toate, cu exceptia ca nu am afisat data si ora in timp real.

## Mod de interactionare cu aplicatia

La pornire se observa meniul principal. Se navigheaza cu ajutorul sagetilor
sus/jos si se selecteaza optiunea cu tasta ENTER. Pentru a incepe jocul se
selecteaza optiunea NEW GAME. Daca anterior nu a fost inceput un joc nou,
optiunea RESUME nu va functiona. In jocul prorpiu-zis, se folosesc
sagetile de directie sus/jos/stanga/dreapta pentru mutari si tasta Q pentru
a reveni la meniul principal.

## Implementarea fiecarei cerinte

### Cerinta 1

Meniul principal este initializat cu ajutorul functiei `meniu_principal()`.
Aceasta este pusa intr-un `while()` care se incheie doar in cazul selectarii
optiunii **QUIT**. In finctia `meniu_principal()`, am creat o matrice in care memorez
optiunile din meniu si fereastra pentru acesta. Urmeaza un `while()` constant
in care este realizata marcarea optiunii curente si este facuta posibila
navigarea in meniu cu ajutorul sagetilor sus/jos. Cand programul detecteaza ca
a fost apasata tasta **ENTER**, meniul este inchis si este returnata valoarea
variabilei `highlight`, aceasta reprezentand numarul de ordine al optiunii
selectate (1, 2, 3). In functie de optiunea selectata, se intampla urmatoarele:

* **NEW GAME** - Incepe un joc nou
* **RESUME** - Se reintoarce la jocul anterior inceput sau afiseaza `---Eroare---`;
daca nu a fost inceput un joc;
* **QUIT** - Opreste aplicatia.
Pentru a determina daca a fost inceput un joc nou sau nu, am folosit
variabila `ng` (new game).

### Cerinta 2

Jocul propriu-zis se intampla, in principal, in functia `joc()`. La fiecare
apel al acesteia, se construiesc ferestrele ce definesc layout-ul jocului.
In jumatatea stanga a ecranului apare tabla de joc, ce contine toate elementele
acesteia. In jumatatea dreapta a ecranului apar ferestrele unde este actualizat
scorul, respectiv unde este afisata legenda comenzilor valide.

### Cerinta 3

Odata ce au fost afisate toate ferestrele, programul verifica daca a fost
selectat **NEW GAME** sau **RESUME**. Daca vorbim de un joc nou `(ng == 1)`, este
apelata functia `init_tabla()`, care realizeaza forma initiala a tablei de joc.
Tabla este formata dintr-o matrice de numere intregi, bordata cu valorile
`-1`. Interiorul, care este luat in calcul pentru joc, are pe toate pozitiile
valoarea `0`. In componenta tablei intra si matricea `vpoz`, care este initializata
similar cu cea anterioara. Aceasta va contine in interior doar valori de `0` sau
`1`, reprezentand existenta elementelor din matricea de mai devreme. Odata ce
aceste matrici au fost initializate, se apeleaza de doua ori functia `nr_nou()`.
Aceasta functie are rolul de a introduce un numar nou, aleator (2 sau 4), pe o
pozitie aleatoare in tabla. Dupa initializarea tablei de joc, se intra intr-un
`while()` constant care se incheie fie la apasarea tastei **Q**, fie la incheierea
jocului. La inceputul acestui `while()`, se actualizeaza vizual continutul tablei si
scorul. In continuare, aplicatia asteapta apasarea unei taste. *(Cerinta 4)* Daca
nu a fost apasata nicio tasta, programul executa automat o mutare (detalii
aditionale mai jos). In cazul in care a fost apasata o sageata directionala,
se apeleaza functia `mutare()`. In cazul in care mutarea a fost valida, functia
returneaza o valoare: `0` pentru cazul in care tabla este plina, `1` daca se mai pot
face miscari, `2` daca exista o caseta cu valoarea **2048**. Daca valoarea returnata
este `0` sau `2`, jocul se inchide automat. Dupa o mutare valida, se actualizeaza
vizual tabla si scorul.

### Cerinta 4

Daca trec 60 de secunde (un minut) fara ca jucatorul sa apese o tasta, se apeleaza
functia `mut_auto()`, care determina directia in care sa fie facuta mutarea, si
functia de mutare, cu directia determinata anterior. In rest, jocul functioneaza
normal.

### Cerinta 5

Daca `mutare()` returneaza `0` (tabla plina) sau `2` (joc castigat), jocul se incheie
si revine la meniul principal.

## Rularea programului

* Construirea executabilului

```bash
make build
```

* Rularea programului

```bash
make run
```

* Curatare

```bash
make clean
```
