#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Functie care da lungimea unui numar
int nrlen(int n)
{
    int cnt = 0;
    while(n)
    {
        cnt++;
        n /= 10;
    }
    return cnt;
}

// Functie care verifica daca jocul a fost castigat
int castig(int tabla[6][6])
{
    int i, j;
    for(i = 1; i <= 4; i++)
    {
        for(j = 1; j <= 4; j++)
        {
            if(tabla[i][j] == 2048)
                return 1;
        }
    }
    return 0;
}

// Functie care actualizeaza valorile din vpoz in functie de tabla
void actualizare_vpoz(int vpoz[6][6], int tabla[6][6])
{
    int i, j;
    for(i = 1; i <= 4; i++)
    {
        for(j = 1; j <= 4; j++)
        {
            if(tabla[i][j] == 0)
                vpoz[i][j] = 0;
            else
                vpoz[i][j] = 1;
        }
    }
}

// Functie care actualizeaza vizual tabla
void update_tabla(WINDOW *cas[4][4], WINDOW *tScor, int tabla[6][6], int *scor, int lxcas)
{
    // Actualizarea scorului
    mvwprintw(tScor, 1, 2, "%d", *scor);
    wrefresh(tScor);

    // Actualizarea tabla
    int i, j;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            werase(cas[i][j]);
            if(tabla[i+1][j+1] > 0)
            {
                mvwprintw(cas[i][j], 3, (lxcas - nrlen(tabla[i+1][j+1])) / 2 + 1, "%d", tabla[i+1][j+1]);
            }
                wrefresh(cas[i][j]);
        }
    }
}

int meniu_principal(int yMax, int xMax)
{
    char optiuni[4][12] = {"", "NEW GAME", "RESUME", "QUIT"};
    int optiune, highlight = 1;
    int i=1, xMen = xMax/2.5, yMen = yMax/2-6;

    WINDOW *meniu = newwin(7, 20, yMen, xMen);
    box(meniu, 0, 0);
    mvwprintw(meniu, 1, 1, " ");
    refresh();
    wrefresh(meniu);
    keypad(meniu, true);
    
    while(1)
    {
        for(i = 1; i <= 3; i++)
        {
            if(i == highlight)
                wattron(meniu, A_REVERSE);
            mvwprintw(meniu, i+1, 10-strlen(optiuni[i])/2, "%s", optiuni[i]);
            wattroff(meniu, A_REVERSE);
        }
        optiune = wgetch(meniu);

        switch (optiune)
        {
            case KEY_UP:
                highlight--;
                if(highlight == 0)
                    highlight = 1;
                break;
            
            case KEY_DOWN:
                if(highlight == 3)
                    highlight = 2;
                highlight++;
                break;
            
            default:
                break;
        }
        if(optiune == 10 || optiune == 32)
        {
            delwin(meniu);
            return highlight;
        }
    }
}

// Functie care, la apel, generaza 2 sau 4 pe o pozitie aleatoare
void nr_nou(int tabla[6][6], int vpoz[6][6])
{
    // Aleg 2 sau 4 la intamplare
    int nr, aux;
    aux = rand() % 2;
    nr = (aux == 0) ? 2 : 4;

    // Aleg o pozitie libera la intamplare
    int i, j;
    i = rand() % 4;
    j = rand() % 4;
    while(vpoz[i+1][j+1] == 1)
    {
        i = rand() % 4;
        j = rand() % 4;
    }

    // Introduc numarul in matrice
    vpoz[i+1][j+1] = 1;
    tabla[i+1][j+1] = nr;
}

// Functie care verifica daca tabla este plina
int plin(int v[6][6])
{
    int i, j;
    for(i = 1; i <= 4; i++)
        for(j = 1; j <= 4; j++)
        if(v[i][j] == 0)
           return 0;
    return 1;
}

int mutare(int dir, int tabla[6][6], int vpoz[6][6], int *scor)
{
    int i, j, alip, k, valid;
    if(dir == KEY_LEFT)
    {
        valid = 0;
        for(i = 1; i <= 4; i++)
        {
            alip = 1;
            for(j = 2; j <= 4; j++)
                if(!(tabla[i][j-1] == tabla[i][j] && tabla[i][j-1] != 0))
                    alip = 0;
            if(alip == 0)
                for(j = 2; j <= 4; j++)
                {
                    k = j;
                    if(tabla[i][k] != 0)
                    {
                        while(k > 1 && tabla[i][k-1] == 0)
                        {
                            valid = 1;
                            tabla[i][k-1] = tabla[i][k];
                            tabla[i][k] = 0;
                            k--;
                        }
                        if(k > 1 && tabla[i][k-1] == tabla[i][k] && alip == 0)
                        {
                            alip = 1;
                            valid = 1;
                            *scor += tabla[i][k];
                            tabla[i][k-1] *= 2;
                            tabla[i][k] = 0;
                        }
                    }
                }
            else
            {
                *scor += tabla[i][1]*2;
                valid = 1;
                tabla[i][1] *= 2;
                tabla[i][2] *= 2;
                tabla[i][3] = 0;
                tabla[i][4] = 0;
            }
        }
    }
    if(dir == KEY_RIGHT)
    {
        valid = 0;
        for(i = 1; i <= 4; i++)
        {
            alip = 1;
            for(j = 2; j <= 4; j++)
                if(!(tabla[i][j-1] == tabla[i][j] && tabla[i][j-1] != 0))
                    alip = 0;
            if(alip == 0)
                for(j = 3; j >= 1; j--)
                {
                    k = j;
                    if(tabla[i][k] != 0)
                    {
                        while(k < 4 && tabla[i][k+1] == 0)
                        {
                            valid = 1;
                            tabla[i][k+1] = tabla[i][k];
                            tabla[i][k] = 0;
                            k++;
                        }
                        if(k < 4 && tabla[i][k+1] == tabla[i][k] && alip == 0)
                        {
                            alip = 1;
                            valid = 1;
                            *scor += tabla[i][k];
                            tabla[i][k+1] *= 2;
                            tabla[i][k] = 0;
                        }
                    }
                }
            else
            {
                *scor += tabla[i][4]*2;
                valid = 1;
                tabla[i][4] *= 2;
                tabla[i][3] *= 2;
                tabla[i][2] = 0;
                tabla[i][1] = 0;
            }
        }
    }
    if(dir == KEY_UP)
    {
        valid = 0;
        for (j = 1; j <= 4; j++)
        {
            alip = 1;
            for(i = 2; i <= 4; i++)
                if(!(tabla[i-1][j] == tabla[i][j] && tabla[i-1][j] != 0))
                    alip = 0;
            if(alip == 0)
                for (i = 2; i <= 4; i++)
                {
                    if (tabla[i][j] != 0)
                    {
                        k = i;
                        while (k > 1 && tabla[k-1][j] == 0)
                        {
                            valid = 1;
                            tabla[k-1][j] = tabla[k][j];
                            tabla[k][j] = 0;
                            k--;
                        }
                        if (k > 1 && tabla[k-1][j] == tabla[k][j] && alip == 0)
                        {
                            alip = 1;
                            valid = 1;
                            *scor += tabla[k][j];
                            tabla[k-1][j] *= 2;
                            tabla[k][j] = 0;
                        }
                    }
                }
            else
            {
                valid = 1;
                *scor += tabla[1][j]*2;
                tabla[1][j] *= 2;
                tabla[2][j] *= 2;
                tabla[3][j] = 0;
                tabla[4][j] = 0;
            }
        }
    }
    if(dir == KEY_DOWN)
    {
        valid = 0;
        for (j = 1; j <= 4; j++)
        {
            alip = 1;
            for(i = 2; i <= 4; i++)
                if(!(tabla[i-1][j] == tabla[i][j] && tabla[i-1][j] != 0))
                    alip = 0;
            if(alip == 0)
                for (i = 3; i >= 1; i--)
                {
                    if (tabla[i][j] != 0)
                    {
                        k = i;
                        while (k < 4 && tabla[k+1][j] == 0)
                        {
                            valid = 1;
                            tabla[k+1][j] = tabla[k][j];
                            tabla[k][j] = 0;
                            k++;
                        }
                        if (k < 4 && tabla[k+1][j] == tabla[k][j] && alip == 0)
                        {
                            alip = 1;
                            valid = 1;
                            *scor += tabla[k][j];
                            tabla[k+1][j] *= 2;
                            tabla[k][j] = 0;
                        }
                    }
                }
            else
            {
                valid = 1;
                *scor += tabla[4][j]*2;
                tabla[4][j] *= 2;
                tabla[3][j] *= 2;
                tabla[2][j] = 0;
                tabla[1][j] = 0;
            }
        }
    }
    actualizare_vpoz(vpoz, tabla);
    if(valid)
    {
        if(!plin(vpoz))
        {
            nr_nou(tabla, vpoz);
        }
    }
    if(castig(tabla))
        return 2;
    if(plin(vpoz))
        return 0;
    return 1;
}

// Functie care initializeaza o tabla noua de joc
void init_tabla(int tabla[6][6], int vpoz[6][6])
{
    int i, j;
    for(i = 0; i < 6; i++)
        for(j = 0; j < 6; j++)
        {
            tabla[i][j] = -1;
            vpoz[i][j] = -1;
        }
    for(i = 1; i <= 4; i++)
        for(j = 1; j <= 4; j++)
        {
            tabla[i][j] = 0;
            vpoz[i][j] = 0;
        }
    nr_nou(tabla, vpoz);
    nr_nou(tabla, vpoz);
}

// Functie care numara cate casete libere sunt in tabla
int cas_l(int vpoz[6][6])
{
    int i, j, cnt = 0;
    for(i = 1; i <= 4; i++)
    {
        for(j = 1; j <= 4; j++)
        {
            if(vpoz[i][j] == 1)
                cnt++;
        }
    }
    return cnt;
}

int mut_auto(int tabla[6][6])
{
    // Initializez patru copii ale tablei
    int tabla_aux[4][6][6], i, j, k, vpoz_aux[4][6][6];
    for(k = 0; k < 4; k++)
    {
        for(i = 0; i < 6; i++)
        {
            for(j = 0; j < 6; j++)
            {
                tabla_aux[k][i][j] = tabla[i][j];
            }
        }
        actualizare_vpoz(vpoz_aux[k], tabla_aux[k]);
    }

    // Caut pentru care directie se elibereaza mai multe casete; scorul este irelevant
    int cnt, cntm = 0, dir = KEY_LEFT, scor = 0;
    mutare(KEY_LEFT, tabla_aux[0], vpoz_aux[0], &scor);
    cnt = cas_l(vpoz_aux[0]);
    if(cntm < cnt)
    {
        cntm = cnt;
        dir = KEY_LEFT;
    }
    mutare(KEY_RIGHT, tabla_aux[1], vpoz_aux[1], &scor);
    cnt = cas_l(vpoz_aux[1]);
    if(cntm < cnt)
    {
        cntm = cnt;
        dir = KEY_RIGHT;
    }
    mutare(KEY_UP, tabla_aux[2], vpoz_aux[2], &scor);
    cnt = cas_l(vpoz_aux[2]);
    if(cntm < cnt)
    {
        cntm = cnt;
        dir = KEY_UP;
    }
    mutare(KEY_DOWN, tabla_aux[3], vpoz_aux[3], &scor);
    cnt = cas_l(vpoz_aux[3]);
    if(cntm < cnt)
    {
        cntm = cnt;
        dir = KEY_DOWN;
    }
    return dir;
}

// Joc nou
int joc(int tabla[6][6], int vpoz[6][6], int *scor, int *ng)
{
    int yMax, xMax, fin;
    getmaxyx(stdscr, yMax, xMax);
    yMax += 0;

    // Initializarea tablei de joc
    int lyJoc = xMax/5+1, lxJoc = xMax/2-6;
    WINDOW *tJoc = newwin(lyJoc, lxJoc, 1, 2);
    box(tJoc, 0, 0);
    wrefresh(tJoc);

    // Initializarea continutului tablei de joc
    WINDOW *patr[4][4], *cas[4][4];
    int i, j;
    int lxpatr = lxJoc/4-2, lypatr = lyJoc/4;
    int lxcas = lxpatr/2+5, lycas = lypatr-2;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            patr[i][j] = newwin(lypatr, lxpatr, 2+4*i*2, 4+4*j*4.5);
            box(patr[i][j], 0, 0);
            wrefresh(patr[i][j]);
        }
    }
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            cas[i][j] = newwin(lycas, lxcas, 3+4*i*2, 6+4*j*4.5);
            wrefresh(cas[i][j]);
        }
    }

    // Initializare panou de informatii
    WINDOW *tScor = newwin(3, 20, 4, xMax/2);
    box(tScor, 0, 0);
    mvwprintw(tScor, 0, 1, "Scor");
    mvwprintw(tScor, 1, 2, "%d", *scor);
    wrefresh(tScor);

    WINDOW *tLegenda = newwin(7, 80, 10, xMax/2);
    box(tLegenda, 0, 0);
    mvwprintw(tLegenda, 0, 1, "Comenzi-valabile");
    mvwprintw(tLegenda, 1, 2, "SAGEATA SUS - Mutare in sus");
    mvwprintw(tLegenda, 2, 2, "SAGEATA JOS - Mutare in jos");
    mvwprintw(tLegenda, 3, 2, "SAGEATA STANGA - Mutare in stanga");
    mvwprintw(tLegenda, 4, 2, "SAGEATA DREAPTA - Mutare in dreapta");
    mvwprintw(tLegenda, 5, 2, "Q - Reintoarcere la meniul principal");
    wrefresh(tLegenda);

    // In cazul unui joc nou (NEW GAME), se initializeaza tabla
    if(*ng)
    {
        init_tabla(tabla, vpoz);
        *scor = 0;
        *ng = 0;
    }

    // Inceperea efectiva a jocului
    int optiune, dir;
    update_tabla(cas, tScor, tabla, scor, lxcas);
    while(1)
    {
        timeout(300000);
        optiune = getch();
        if(optiune == ERR)
        {
            dir = mut_auto(tabla);
            fin = mutare(dir, tabla, vpoz, scor);
        }
        else
        {
            if(optiune == 'q' || optiune == 'Q')
            {
                delwin(tJoc);
                delwin(tScor);
                delwin(tLegenda);
                for(i = 0; i < 4; i++)
                {
                    for(j = 0; j < 4; j++)
                    {
                        delwin(cas[i][j]);
                        delwin(patr[i][j]);
                    }
                }
                clear();
                refresh();
                return optiune;
            }
            else
            {
                fin = mutare(optiune, tabla, vpoz, scor);
            }
        }
        update_tabla(cas, tScor, tabla, scor, lxcas);
        if(fin == 0 || fin == 2)
        {
            delwin(tJoc);
            delwin(tScor);
            delwin(tLegenda);
            for(i = 0; i < 4; i++)
            {
                for(j = 0; j < 4; j++)
                {
                    delwin(cas[i][j]);
                    delwin(patr[i][j]);
                }
            }
            clear();
            refresh();
            return fin;
        }
        napms(60000);
    }
}

int main(void)
{
    int ng = 1, tabla[6][6], vpoz[6][6], scor, fin;
    srand(time(NULL));
    // START NCURSES
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, true);

    int yMax, xMax, optiune;
    getmaxyx(stdscr, yMax, xMax);

    while(1)
    {
        optiune = meniu_principal(yMax, xMax);
        switch (optiune)
        {
            case 1:
                clear();
                refresh();
                ng = 1;
                fin = joc(tabla, vpoz, &scor, &ng);
                break;
            
            case 2:
                if(ng)
                {
                    clear();
                    printw("---EROARE---\n");
                    refresh();
                    getch();
                    clear();
                    fin = 1;
                }
                else
                {
                    clear();
                    refresh();
                    fin = joc(tabla, vpoz, &scor, &ng);
                }
                break;

            case 3:
                clear();
                fin = 3;
                break;

            default:
                clear();
                printw("--- EROARE ---\n");
                refresh();
                getch();
                break;
        }
        if(fin == 0)
        {
            printw("Ai pierdut. Joc incheiat.");
            getch();
        }
        if(fin == 2)
        {
            printw("Felicitari! Ai ajuns la 2048. Ai castigat!");
            getch();
        }
        if(fin == 3)
        {
            break;
        }
    }
    endwin();
    return 0;
}