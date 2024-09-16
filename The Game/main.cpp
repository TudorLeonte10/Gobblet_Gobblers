#include "graphics.h"
#include <iostream>
#include <stdio.h>
#include <Windows.h>

using namespace std;

int strategie = 0, numarPieseMari1, numarPieseMedii1, numarPieseMici1, numarPieseMari2, numarPieseMedii2, numarPieseMici2;
int stadiul = 0, jucatorul = 2, tur = 0;

int player1 = 1, player2 = 1, currentPlayer;

int g1s = 0, g2s = 0, g3s = 0, g4s = 0;

int difLeftLev = 1, difRightLev = 1;

struct globalMove
{
    int fl, fc, ol, oc, type, decision;
    bool change;
} g;

struct mutarea
{
    int tip; ///1=pus piesa, 2=mutat piesa
    int l; ///linie
    int c; ///coloana
    int ci, cf, li, lf;
    int dimensiuneaPiesei;
} mutareaAnterioara;

struct stiva
{
    int player;
    int dimensiune;

};

struct matrice
{
    int jucator;
    int marimePiesa;
    stiva piese[3]; ///trebuie retinuta piesa de sub piesa curenta. poti sa ai mai multe piese una peste alta;

} a[3][3], b[3][3], c[3][3];
///b este matricea auxiliara pentru rotiri

void homeScreen();
void muta_o_piesa(matrice a[3][3], int liniaInitiala, int coloanaInitiala, int liniaFinala, int coloanaFinala);
int verifica_daca_mai_are_un_tip_de_piese(int marime, int jucatorul);
int verifica_daca_se_poate_pune_o_piesa(matrice a[3][3], int linie, int coloana, int jucatorul, int marime);
void pune_o_piesa(matrice a[3][3], int linie, int coloana, int jucatorul, int marime);

void scoreScreen();
void drawScreen(int player);
void afisare(matrice a[3][3]);
void adaugare_min_strat(stiva piese[3], int jucatorul, int marime);
void stergere_min_strat(stiva piese[3]);
int verifica_daca_a_castigat_min_strat(matrice a[3][3]);
void pune_o_piesa_min_strat(matrice a[3][3], int linie, int coloana, int jucatorul, int marime);
int verifica_daca_mai_are_piese_mici_min_strat(int jucator);
int verifica_daca_mai_are_piese_medii_min_strat(int jucator);
int verifica_daca_mai_are_piese_mari_min_strat(int jucator);
int verifica_daca_mai_are_un_tip_de_piese_min_strat(int marime, int jucatorul);
int verifica_daca_se_poate_pune_o_piesa_min_strat(matrice a[3][3], int linie, int coloana, int jucatorul, int marime);
int verifica_daca_piesa_are_marimea_corecta_min_strat(int marime);
void muta_o_piesa_min_strat(matrice a[3][3], int liniaInitiala, int coloanaInitiala, int liniaFinala, int coloanaFinala);
int verifica_daca_vrea_sa_mute_piesa_lui_min_strat(matrice a[3][3], int jucatorul, int linie, int coloana);
void randul_jucatorului_min_strat(matrice a[3][3]);
int verifica_linia_min_strat(matrice a[3][3], int i, int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare);
int verifica_coloana_min_strat(matrice a[3][3], int j, int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare);
int verifica_diagonalele_min_strat(matrice a[3][3], int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare);
void verifica_daca_poate_castiga_min_strat(matrice a[3][3], int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare, int& r);
int verifica_daca_poate_acoperi_piesa_min_strat(int dimensiune, int jucatorul);
int decide_dimensiunea_min_strat();
void mutare1_min_strat(matrice a[3][3], int linia, int coloana);
void mutare2_min_strat(matrice a[3][3], int linia, int coloana);
void mutare3_min_strat(matrice a[3][3], int linia, int coloana, int& rez);
int inMarice_min_strat(int i, int j);
int cauta_pozitia_min_strat(matrice a[3][3], int i, int j, int& linia, int& coloana, int dimensiune);
void randul_calculatorului_min_strat(matrice a[3][3]);


///asta imi trebuie doar ca sa afisez matricea in main sa vad ce mutari face calculatorul

void afisare(matrice a[3][3])
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
            cout << a[i][j].jucator << " ";
        cout << "\n";
    }
}

void adaugare_min_strat(stiva piese[3], int jucatorul, int marime)
{
    piese[2].player = piese[1].player;
    piese[2].dimensiune = piese[1].dimensiune;
    piese[1].player = piese[0].player;
    piese[1].dimensiune = piese[0].dimensiune;

    piese[0].player = jucatorul;
    piese[0].dimensiune = marime;
}

void stergere_min_strat(stiva piese[3])
{

    piese[0].player = piese[1].player;
    piese[0].dimensiune = piese[1].dimensiune;
    piese[1].player = piese[2].player;
    piese[1].dimensiune = piese[2].dimensiune;
    piese[2].player = 0;
    piese[2].dimensiune = 0;
}

int verifica_daca_a_castigat_min_strat(matrice a[3][3])
{
    int i, j;

    for (i = 0; i < 3; ++i)
    {
        if ((a[i][0].jucator == a[i][1].jucator) && (a[i][1].jucator == a[i][2].jucator) && (a[i][0].jucator != 0)) return 1;
    }
    for (j = 0; j < 3; j++)
    {
        if ((a[0][j].jucator == a[1][j].jucator) && (a[1][j].jucator == a[2][j].jucator) && (a[0][j].jucator != 0)) return 1;
    }
    if ((a[0][0].jucator == a[1][1].jucator) && (a[1][1].jucator == a[2][2].jucator) && (a[0][0].jucator != 0)) return 1;
    if ((a[2][0].jucator == a[1][1].jucator) && (a[1][1].jucator == a[0][2].jucator) && (a[2][0].jucator != 0)) return 1;
    return 0;
}

int remiza(matrice a[3][3])
{

     int i, j, nrl=0, nrc=0, nrd1=0, nrd2=0;

     for (i = 0; i < 3; ++i)
    {
        if ((a[i][0].jucator == a[i][1].jucator) && (a[i][1].jucator == a[i][2].jucator) && (a[i][0].jucator != 0))
         nrl++;
        if(nrl == 2)
            return 1;
    }
    for (j = 0; j < 3; j++)
    {
        if ((a[0][j].jucator == a[1][j].jucator) && (a[1][j].jucator == a[2][j].jucator) && (a[0][j].jucator != 0))
            nrc++;
        if(nrc == 2)
            return 1;
    }
    if ((a[0][0].jucator == a[1][1].jucator) && (a[1][1].jucator == a[2][2].jucator) && (a[0][0].jucator != 0)) return 1;
    if ((a[2][0].jucator == a[1][1].jucator) && (a[1][1].jucator == a[0][2].jucator) && (a[2][0].jucator != 0)) return 1;
    return 0;
}




void pune_o_piesa_min_strat(matrice a[3][3], int linie, int coloana, int jucatorul, int marime)
{

    g.fl = linie;
    g.fc = coloana;
    g.decision = 0;
    g.type = marime;
    g.change = true;

    a[linie][coloana].jucator = jucatorul;
    a[linie][coloana].marimePiesa = marime;
    adaugare_min_strat(a[linie][coloana].piese, jucatorul, marime);
    if (jucatorul == 1)
    {
        if (marime == 1) numarPieseMici1++;
        else
        {
            if (marime == 3) numarPieseMari1++;
            else numarPieseMedii1++;
        }
    }
    else
    {
        if (marime == 1) numarPieseMici2++;
        else
        {
            if (marime == 3) numarPieseMari2++;
            else numarPieseMedii2++;
        }
    }
    cout << "\n linia" << " " << linie << " " << coloana << "\n";
}

int verifica_daca_mai_are_piese_mici_min_strat(int jucator)
{
    if (jucator == 1)
    {
        if (numarPieseMici1 == 2) return 0;
        else
            return 1;
    }
    else
    {
        if (numarPieseMici2 == 2) return 0;
        else
            return 1;
    }
}


int verifica_daca_mai_are_piese_medii_min_strat(int jucator)
{
    if (jucator == 1)
    {
        if (numarPieseMedii1 == 2) return 0;
        else
            return 1;
    }
    else
    {
        if (numarPieseMedii2 == 2) return 0;
        else
            return 1;
    }
}

int verifica_daca_mai_are_piese_mari_min_strat(int jucator)
{
    if (jucator == 1)
    {
        if (numarPieseMari1 == 2) return 0;
        else
            return 1;
    }
    else
    {
        if (numarPieseMari2 == 2) return 0;
        else
            return 1;
    }
}

int verifica_daca_mai_are_un_tip_de_piese_min_strat(int marime, int jucatorul)
{
    if (marime == 1)
    {
        if (verifica_daca_mai_are_piese_mici_min_strat(jucatorul) == 0) return 0;
        else
            return 1;
    }
    else
    {
        if (marime == 2)
        {
            if (verifica_daca_mai_are_piese_medii_min_strat(jucatorul) == 0) return 0;
            else
                return 1;
        }

        else
        {
            if (verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 0) return 0;
            else return 1;
        }
    }
}
int verifica_daca_se_poate_pune_o_piesa_min_strat(matrice a[3][3], int linie, int coloana, int jucatorul, int marime)
{
    cout<<"Suntem in cazul cu linia "<<linie<<" si coloana "<<coloana<<endl;
    if (linie < 0 || coloana < 0 || linie > 2 || coloana > 2) { cout<<"You're out!"<<endl; return 0; }
    if (a[linie][coloana].jucator == 0) { cout<<"Pozitia este goala"<<endl; return 1; }
    if (a[linie][coloana].marimePiesa >= marime) { cout<<"Marimea era prea mare"<<endl; return 0; }
    cout<<endl;
    return 1;
}

int verifica_daca_piesa_are_marimea_corecta_min_strat(int marime)
{
    if (marime < 1 || marime>3) return 0;
    return 1;
}

void muta_o_piesa_min_strat(matrice a[3][3], int liniaInitiala, int coloanaInitiala, int liniaFinala, int coloanaFinala)
{
    g.fl = liniaFinala;
    g.fc = coloanaFinala;
    g.ol = liniaInitiala;
    g.oc = coloanaInitiala;
    g.decision = 1;
    g.change = true;
    g.type = a[liniaInitiala][coloanaInitiala].marimePiesa;
    a[liniaFinala][coloanaFinala].jucator = a[liniaInitiala][coloanaInitiala].jucator;
    a[liniaFinala][coloanaFinala].marimePiesa = a[liniaInitiala][coloanaInitiala].marimePiesa;
    adaugare_min_strat(a[liniaFinala][coloanaFinala].piese, a[liniaFinala][coloanaFinala].jucator, a[liniaFinala][coloanaFinala].marimePiesa);
    stergere_min_strat(a[liniaInitiala][coloanaInitiala].piese);
    a[liniaInitiala][coloanaInitiala].jucator = a[liniaInitiala][coloanaInitiala].piese[0].player;
    a[liniaInitiala][coloanaInitiala].marimePiesa = a[liniaInitiala][coloanaInitiala].piese[0].dimensiune;
    cout << "\n" << "linia initiala " << liniaInitiala << " " << coloanaInitiala << "\n";
    cout << "\n" << "linia finala " << liniaFinala << " " << coloanaFinala << "\n";

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            cout<<a[i][j].jucator<<" ";
        }
        cout<<endl;
    }

    cout<<"Dimension matrix"<<endl;
    for(int i = 0; i< 3; i++)
    {
        for(int j = 0; j< 3; j++)
        {
            cout<<a[i][j].marimePiesa<<" ";
        }
        cout<<endl;
    }
}

int verifica_daca_vrea_sa_mute_piesa_lui_min_strat(matrice a[3][3], int jucatorul, int linie, int coloana)
{
    if (a[linie][coloana].jucator != jucatorul) return 0;
    return 1;
}

int verifica_daca_mai_are_piese_min_strat(int jucator)
{
    if (verifica_daca_mai_are_piese_mari_min_strat(jucator) == 1 || verifica_daca_mai_are_piese_medii_min_strat(jucator) == 1 || verifica_daca_mai_are_piese_mici_min_strat(jucator) == 1) return 1;
    return 0;
}

int verifica_linia_min_strat(matrice a[3][3], int i, int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare)
{
    if (a[i][0].jucator == jucatorul)
    {
        if (a[i][1].jucator == jucatorul && a[i][2].marimePiesa != 3 && !(a[i][2].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
        {
            liniaCastigatoare = i;
            coloanaCastigatoare = 2;
            return 1;
        }
        if (a[i][2].jucator == jucatorul && a[i][1].marimePiesa != 3 && !(a[i][1].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
        {
            liniaCastigatoare = i;
            coloanaCastigatoare = 1;
            return 1;
        }
    }
    else
    {
        if (a[i][1].jucator == jucatorul)
        {
            if (a[i][2].jucator == jucatorul && a[i][0].marimePiesa != 3 && !(a[i][0].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
            {
                liniaCastigatoare = i;
                coloanaCastigatoare = 0;
                return 1;
            }
        }
    }
    return 0;
}

int verifica_coloana_min_strat(matrice a[3][3], int j, int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare)
{
    if (a[0][j].jucator == jucatorul)
    {
        if (a[1][j].jucator == jucatorul && a[2][j].marimePiesa != 3 && !(a[2][j].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
        {
            liniaCastigatoare = 2;
            coloanaCastigatoare = j;
            return 1;
        }
        if (a[2][j].jucator == jucatorul && a[1][j].marimePiesa != 3 && !(a[1][j].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
        {
            liniaCastigatoare = 1;
            coloanaCastigatoare = j;
            return 1;
        }
    }
    else
    {
        if (a[1][j].jucator == jucatorul)
        {
            if (a[2][j].jucator == jucatorul && a[0][j].marimePiesa != 3 && !(a[0][j].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
            {
                liniaCastigatoare = 0;
                coloanaCastigatoare = j;
                return 1;
            }
        }
    }
    return 0;

}

int verifica_diagonalele_min_strat(matrice a[3][3], int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare)
{
    if (a[0][0].jucator == jucatorul)
    {
        if (a[1][1].jucator == jucatorul && a[2][2].marimePiesa != 3 && !(a[2][2].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
        {
            liniaCastigatoare = 2;
            coloanaCastigatoare = 2;
            return 1;
        }
        if (a[2][2].jucator == jucatorul && a[1][1].marimePiesa != 3 && !(a[1][1].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
        {
            liniaCastigatoare = 1;
            coloanaCastigatoare = 1;
            return 1;
        }
    }
    else
    {
        if (a[0][2].jucator == jucatorul)
        {
            if (a[1][1].jucator == jucatorul && a[2][0].marimePiesa != 3 && !(a[2][0].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
            {
                liniaCastigatoare = 2;
                coloanaCastigatoare = 0;
                return 1;
            }
            if (a[2][0].jucator == jucatorul && a[1][1].marimePiesa != 3 && !(a[1][1].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
            {
                liniaCastigatoare = 1;
                coloanaCastigatoare = 1;
                return 1;
            }
        }
        else
        {
            if (a[1][1].jucator == jucatorul)
            {
                if (a[2][0].jucator == jucatorul && a[2][2].marimePiesa != 3 && !(a[2][2].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
                {
                    liniaCastigatoare = 0;
                    coloanaCastigatoare = 2;
                    return 1;
                }
                if (a[2][2].jucator == jucatorul && a[2][0].marimePiesa != 3 && !(a[2][0].marimePiesa == 2 && verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1))
                {
                    liniaCastigatoare = 0;
                    coloanaCastigatoare = 0;
                    return 1;
                }
            }
        }
    }
    return 0;
}

void verifica_daca_poate_castiga_min_strat(matrice a[3][3], int jucatorul, int& liniaCastigatoare, int& coloanaCastigatoare, int& r)
{
    ///verificam liniile
    r = 0;
    int i, j;
    for (i = 0; i < 3 && r != 1; ++i)
        r = verifica_linia_min_strat(a, i, jucatorul, liniaCastigatoare, coloanaCastigatoare);
    if (r != 1)
    {
        ///verificam coloanele
        for (j = 0; j < 3 && r != 1; j++) r = verifica_coloana_min_strat(a, j, jucatorul, liniaCastigatoare, coloanaCastigatoare);
        if (r != 1)
        {
            ///verificam diagonalele
            r = verifica_diagonalele_min_strat(a, jucatorul, liniaCastigatoare, coloanaCastigatoare);
        }
    }
}

int verifica_daca_poate_acoperi_piesa_min_strat(int dimensiune, int jucatorul)
{
    int marime = dimensiune + 1;
    if (verifica_daca_mai_are_un_tip_de_piese_min_strat(marime, jucatorul) == 1) return 1;
    if (marime != 3)
    {
        if (verifica_daca_mai_are_piese_mari_min_strat(jucatorul) == 1) return 1;
    }
    return 0;
}
int decide_dimensiunea_min_strat()
{
    int ok = 0, calculator = 2;
    ok = verifica_daca_mai_are_piese_mari_min_strat(calculator);
    if (ok == 1) return 3;
    ok = verifica_daca_mai_are_piese_medii_min_strat(calculator);
    if (ok == 1) return 2;
    ok = verifica_daca_mai_are_piese_mici_min_strat(calculator);
    if (ok == 1) return 1;
    return -1;
}
void mutare1_min_strat(matrice a[3][3], int linia, int coloana)
{
    int liniaInitiala, coloanaInitiala;

    int l1, l2, c1, c2, ok1 = 0;

    ///verificam daca castigam pe coloana
    if (linia == 1)
    {
        if (a[linia - 1][coloana].jucator == 2 && a[linia + 1][coloana].jucator == 2) c1 = c2 = coloana;
        l1 = linia - 1;
        l2 = linia + 1;
        ok1 = 1;
    }
    if (linia == 0)
    {
        if (a[linia + 1][coloana].jucator == 2 && a[linia + 2][coloana].jucator == 2) c1 = c2 = coloana;
        l1 = linia + 1;
        l2 = linia + 2;
        ok1 = 1;
    }
    if (linia == 2)
    {
        if (a[linia - 1][coloana].jucator == 2 && a[linia - 2][coloana].jucator == 2) c1 = c2 = coloana;
        l1 = linia - 2;
        l2 = linia - 1;
        ok1 = 1;
    }

    if (ok1 == 0)
    {
        ///verificam daca castigam pe linie
        if (coloana == 0)
        {
            if (a[linia][coloana + 1].jucator == 2 && a[linia][coloana + 2].jucator == 2) l1 = l2 = linia;
            c1 = coloana + 1;
            c2 = coloana + 2;
            ok1 = 1;
        }
        if (coloana == 1)
        {
            if (a[linia][coloana - 1].jucator == 2 && a[linia][coloana + 1].jucator == 2) l1 = l2 = linia;
            c1 = coloana - 1;
            c2 = coloana + 1;
            ok1 = 1;
        }
        if (coloana == 2)
        {
            if (a[linia][coloana - 1].jucator == 2 && a[linia][coloana - 2].jucator == 2) l1 = l2 = linia;
            c1 = coloana - 1;
            c2 = coloana - 2;
            ok1 = 1;
        }
    }
    if (ok1 == 0)
    {
        ///verificam daca castiga pe diagonala
        if (linia == 0 && coloana == 0)
        {
            if (a[1][1].jucator == 2 && a[2][2].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 2;
                c2 = 2;
            }
        }
        if (linia == 0 && coloana == 2)
        {
            if (a[1][1].jucator == 2 && a[2][0].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 2;
                c2 = 0;
            }
        }
        if (linia == 2 && coloana == 0)
        {
            if (a[1][1].jucator == 2 && a[0][2].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 0;
                c2 = 2;
            }
        }
        if (linia == 2 && coloana == 2)
        {
            if (a[1][1].jucator == 2 && a[0][0].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 0;
                c2 = 0;
            }
        }
        if (linia == 1 && coloana == 1)
        {
            if (a[0][0].jucator == 2 && a[2][2].jucator == 2)
            {
                l1 = 0;
                c1 = 0;
                l2 = 2;
                c2 = 2;
            }
            else
            {
                if (a[2][0].jucator == 2 && a[0][2].jucator == 2)
                {
                    l1 = 0;
                    c1 = 2;
                    l2 = 2;
                    c2 = 0;
                }
            }
        }
    }

    int i, j, ok = 0;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
        {
            if ((a[i][j].jucator == 2) && (i != l1) && (i != l2) && (j != c1) && (j != c2))
            {
                ok = 1;
                liniaInitiala = i;
                coloanaInitiala = j;
            }
        }
    if (ok == 1) muta_o_piesa_min_strat(a, liniaInitiala, coloanaInitiala, linia, coloana);
}

void mutare2_min_strat(matrice a[3][3], int linia, int coloana)
{
    int c1, l1, c2, l2, ok1 = 0, liniaInitiala, coloanaInitiala;
    cout<<linia<<" "<<coloana<<endl;
    ///verificam daca castigam pe coloana
    if (linia == 1)
    {
        if (a[linia - 1][coloana].jucator == 2 && a[linia + 1][coloana].jucator == 2)
        {
            c1 = c2 = coloana;
            l1 = linia - 1;
            l2 = linia + 1;
            ok1 = 1;
        }
    }
    if (linia == 0)
    {
        if (a[linia + 1][coloana].jucator == 2 && a[linia + 2][coloana].jucator == 2)
        {
            c1 = c2 = coloana;
            l1 = linia + 1;
            l2 = linia + 2;
            ok1 = 1;
            cout<<"Suntem in cazul cu linia 0 si coloana 1 adica al doilea if de la prima verif";
        }
    }
    if (linia == 2)
    {
        if (a[linia - 1][coloana].jucator == 2 && a[linia - 2][coloana].jucator == 2)
        {
            c1 = c2 = coloana;
            l1 = linia - 2;
            l2 = linia - 1;
            ok1 = 1;
        }
    }

    if (ok1 == 0)
    {
        cout<<"Verif pe linie";
        ///verificam daca castigam pe linie
        if (coloana == 0)
        {
            if (a[linia][coloana + 1].jucator == 2 && a[linia][coloana + 2].jucator == 2)
            {
                l1 = l2 = linia;
                c1 = coloana + 1;
                c2 = coloana + 2;
                ok1 = 1;
            }
        }
        if (coloana == 1)
        {
            if (a[linia][coloana - 1].jucator == 2 && a[linia][coloana + 1].jucator == 2)
            {
                l1 = l2 = linia;
                c1 = coloana - 1;
                c2 = coloana + 1;
                ok1 = 1;
                cout<< "Pe un caz obosit";
            }
        }
        if (coloana == 2)
        {
            if (a[linia][coloana - 1].jucator == 2 && a[linia][coloana - 2].jucator == 2)
            {
                l1 = l2 = linia;
                c1 = coloana - 1;
                c2 = coloana - 2;
                ok1 = 1;
            }
        }
    }
    if (ok1 == 0)
    {
        cout<<"Verificam pe diagonala"<<endl;
        ///verificam daca castiga pe diagonala
        if (linia == 0 && coloana == 0)
        {
            if (a[1][1].jucator == 2 && a[2][2].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 2;
                c2 = 2;
            }
        }
        if (linia == 0 && coloana == 2)
        {
            if (a[1][1].jucator == 2 && a[2][0].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 2;
                c2 = 0;
                cout<<"Suntem in cazul cu linia 0 si coloana 2";
            }
        }
        if (linia == 2 && coloana == 0)
        {
            if (a[1][1].jucator == 2 && a[0][2].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 0;
                c2 = 2;
            }
        }
        if (linia == 2 && coloana == 2)
        {
            if (a[1][1].jucator == 2 && a[0][0].jucator == 2)
            {
                l1 = 1;
                c1 = 1;
                l2 = 0;
                c2 = 0;
            }
        }
        if (linia == 1 && coloana == 1)
        {
            if (a[0][0].jucator == 2 && a[2][2].jucator == 2)
            {
                l1 = 0;
                c1 = 0;
                l2 = 2;
                c2 = 2;
            }
            else
            {
                if (a[2][0].jucator == 2 && a[0][2].jucator == 2)
                {
                    l1 = 0;
                    c1 = 2;
                    l2 = 2;
                    c2 = 0;
                }
            }
        }
    }

    cout<<"l1="<<l1<<" c1="<<c1<<" l2="<<l2<<" c2="<<c2<<endl;
    int i, j, ok = 0;
    int marime = a[linia][coloana].marimePiesa;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
        {
            if ((a[i][j].jucator == 2) && !(((i == l1) && (j == c1)) || ((i == l2) && (j == c2))) && (a[i][j].marimePiesa > marime))
            {
                ok = 1;
                liniaInitiala = i;
                coloanaInitiala = j;
            }
        }
    cout<<"linia initiala="<<liniaInitiala<<"coloana initiala="<<coloanaInitiala;
    if (ok == 1) muta_o_piesa_min_strat(a, liniaInitiala, coloanaInitiala, linia, coloana);
    cout<<"S-a mutat o piesa "<<ok<<endl;
}

void mutare3_min_strat(matrice a[3][3], int linia, int coloana, int& rez)
{
    int c1, l1, c2, l2, calculator = 2, ok1 = 0;
    ///verificam daca castiga pe coloana
    if (linia == 1)
    {
        if (a[linia - 1][coloana].jucator == 1 && a[linia + 1][coloana].jucator == 1) c1 = c2 = coloana;
        l1 = linia - 1;
        l2 = linia + 1;
        ok1 = 1;
    }
    if (linia == 0)
    {
        if (a[linia + 1][coloana].jucator == 1 && a[linia + 2][coloana].jucator == 1) c1 = c2 = coloana;
        l1 = linia + 1;
        l2 = linia + 2;
        ok1 = 1;
    }
    if (linia == 2)
    {
        if (a[linia - 1][coloana].jucator == 1 && a[linia - 2][coloana].jucator == 1) c1 = c2 = coloana;
        l1 = linia - 2;
        l2 = linia - 1;
        ok1 = 1;
    }

    if (ok1 == 0)
    {
        ///verificam daca castigam pe linie
        if (coloana == 0)
        {
            if (a[linia][coloana + 1].jucator == 1 && a[linia][coloana + 2].jucator == 1) l1 = l2 = linia;
            c1 = coloana + 1;
            c2 = coloana + 2;
            ok1 = 1;
        }
        if (coloana == 1)
        {
            if (a[linia][coloana - 1].jucator == 1 && a[linia][coloana + 1].jucator == 1) l1 = l2 = linia;
            c1 = coloana - 1;
            c2 = coloana + 1;
            ok1 = 1;
        }
        if (coloana == 2)
        {
            if (a[linia][coloana - 1].jucator == 1 && a[linia][coloana - 2].jucator == 1) l1 = l2 = linia;
            c1 = coloana - 1;
            c2 = coloana - 2;
            ok1 = 1;
        }
    }
    if (ok1 == 0)
    {
        ///verificam daca castiga pe diagonala
        if (linia == 0 && coloana == 0)
        {
            if (a[1][1].jucator == 1 && a[2][2].jucator == 1)
            {
                l1 = 1;
                c1 = 1;
                l2 = 2;
                c2 = 2;
            }
        }
        if (linia == 0 && coloana == 2)
        {
            if (a[1][1].jucator == 1 && a[2][0].jucator == 1)
            {
                l1 = 1;
                c1 = 1;
                l2 = 2;
                c2 = 0;
            }
        }
        if (linia == 2 && coloana == 0)
        {
            if (a[1][1].jucator == 1 && a[0][2].jucator == 1)
            {
                l1 = 1;
                c1 = 1;
                l2 = 0;
                c2 = 2;
            }
        }
        if (linia == 2 && coloana == 2)
        {
            if (a[1][1].jucator == 1 && a[0][0].jucator == 1)
            {
                l1 = 1;
                c1 = 1;
                l2 = 0;
                c2 = 0;
            }
        }
        if (linia == 1 && coloana == 1)
        {
            if (a[0][0].jucator == 1 && a[2][2].jucator == 1)
            {
                l1 = 0;
                c1 = 0;
                l2 = 2;
                c2 = 2;
            }
            else
            {
                if (a[2][0].jucator == 1 && a[0][2].jucator == 1)
                {
                    l1 = 0;
                    c1 = 2;
                    l2 = 2;
                    c2 = 0;
                }
            }
        }
    }
    int poate = 1;
    if (a[l1][c1].marimePiesa == 3 && a[l2][c2].marimePiesa == 3)
        poate = 0;

    if (!poate)
    {
        if (verifica_daca_mai_are_piese_mari_min_strat(calculator) == 1)
        {
            pune_o_piesa_min_strat(a, linia, coloana, calculator, 3);
            rez = 1;
        }
        else
        {
            int i, j, ok = 0;
            for (i = 0; i < 3 && !ok; ++i)
                for (j = 0; j < 3 && !ok; ++j)
                {
                    if (a[i][j].jucator == 2 && a[i][j].marimePiesa == 3)
                    {
                        muta_o_piesa_min_strat(a, i, j, linia, coloana);
                        ok = 1;
                        rez = 1;
                    }
                }
        }
    }

    if (poate || rez == 0)
    {
        if (verifica_daca_mai_are_piese_min_strat(calculator) == 1)
        {
            int dimensiune = decide_dimensiunea_min_strat();
            if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, l1, c1, calculator, dimensiune) == 1)
            {
                pune_o_piesa_min_strat(a, l1, c1, calculator, dimensiune);
                rez = 1;
            }
            else if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, l2, c2, calculator, dimensiune) == 1)
            {
                pune_o_piesa_min_strat(a, l2, c2, calculator, dimensiune);
                rez = 1;
            }
        }
        if (verifica_daca_mai_are_piese_min_strat(calculator) == 0 || rez == 0)
        {
            int ok = 0, i, j;
            for (i = 0; i < 3 && !ok; ++i)
                for (j = 0; j < 3 && !ok; ++j)
                {
                    if (a[i][j].jucator == 2)
                    {
                        int dimensiune = a[i][j].marimePiesa;
                        if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, l1, c1, calculator, dimensiune) == 1)
                        {
                            ok = 1;
                            muta_o_piesa_min_strat(a, i, j, l1, c1);
                            rez = 1;
                        }
                        else
                        {
                            if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, l2, c2, calculator, dimensiune) == 1)
                            {
                                ok = 1;
                                muta_o_piesa_min_strat(a, i, j, l2, c2);
                                rez = 1;
                            }
                        }
                    }
                }
        }

    }

}
int inMarice_min_strat(int i, int j)
{
    if (i < 0) return 0;
    if (j < 0) return 0;
    if (i > 2) return 0;
    if (j > 2) return 0;

    return 1;
}

int cauta_pozitia_min_strat(matrice a[3][3], int i, int j, int& linia, int& coloana, int dimensiune)
{
    int calculator = 2;
    cout<<"Se cauta pozitia";
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i, j + 1, calculator, dimensiune) == 1 && inMarice_min_strat(i, j + 1) == 1)
    {
        linia = i;
        coloana = j + 1;
        return 1;
    }
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i, j - 1, calculator, dimensiune) == 1 && inMarice_min_strat(i, j - 1) == 1)
    {
        linia = i;
        coloana = j - 1;
        cout<<"suntem pe cazul i si j-1 si afisez linia "<<linia<< "si coloana "<<coloana;
        return 1;
    }
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i + 1, j, calculator, dimensiune) == 1 && inMarice_min_strat(i + 1, j) == 1)
    {
        linia = i + 1;
        coloana = j;
        return 1;
    }
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i - 1, j, calculator, dimensiune) == 1 && inMarice_min_strat(i - 1, j) == 1)
    {
        linia = i - 1;
        coloana = j;
        return 1;
    }
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i + 1, j + 1, calculator, dimensiune) == 1 && inMarice_min_strat(i + 1, j + 1) == 1)
    {
        linia = i + 1;
        coloana = j + 1;
        return 1;
    }
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i + 1, j - 1, calculator, dimensiune) == 1 && inMarice_min_strat(i + 1, j - 1) == 1)
    {
        linia = i + 1;
        coloana = j - 1;
        return 1;
    }
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i - 1, j + 1, calculator, dimensiune) == 1 && inMarice_min_strat(i - 1, j + 1) == 1)
    {
        linia = i - 1;
        coloana = j + 1;
        return 1;
    }
    if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i - 1, j - 1, calculator, dimensiune) == 1 && inMarice_min_strat(i - 1, j - 1) == 1)
    {
        linia = i - 1;
        coloana = j - 1;
        return 1;
    }
    return 0;
}

void randul_calculatorului_min_strat(matrice a[3][3])
{
    int r = 0, linia = -1, coloana = -1, calculator = 2, dimensiune, jucatorul = 1, amMutat = 0;
    verifica_daca_poate_castiga_min_strat(a, calculator, linia, coloana, r);
    cout<<"Linia: "<<linia<<" Coloana: "<<coloana;
    if (r == 1) ///putem noi castiga
    {
        if (a[linia][coloana].jucator == 0)
        {
            if (verifica_daca_mai_are_piese_min_strat(calculator) == 1)
            {
                int dimensiune = decide_dimensiunea_min_strat();
                pune_o_piesa_min_strat(a, linia, coloana, calculator, dimensiune);
                if (a[linia][coloana].jucator == 2) amMutat = 1;
            }
            else
            {
                mutare1_min_strat(a, linia, coloana);
                if (a[linia][coloana].jucator == 2) amMutat = 1;
            }
        }
        else
        {
            if (a[linia][coloana].marimePiesa != 3)
            {
                if (verifica_daca_mai_are_piese_min_strat(calculator) == 1)
                {
                    dimensiune = decide_dimensiunea_min_strat();
                    cout<<"Dimensiunea piesei pentru a castiga "<<dimensiune<<endl;
                    if (dimensiune > a[linia][coloana].marimePiesa)
                    {
                        cout<<"Marimea piesei de inlocuit "<<a[linia][coloana].marimePiesa<<endl;
                        pune_o_piesa_min_strat(a, linia, coloana, calculator, dimensiune);
                        amMutat = 1;
                    }
                }
                if (verifica_daca_mai_are_piese_min_strat(calculator) == 0 || amMutat == 0)
                {
                    cout<<"Suntem in cazul in care trebuie sa mutam o piesa"<<endl;
                    mutare2_min_strat(a, linia, coloana);
                    if (a[linia][coloana].jucator == 2) amMutat = 1;
                }
            }
        }
    }

    if (amMutat == 0)
    {
        verifica_daca_poate_castiga_min_strat(a, jucatorul, linia, coloana, r);
        if (r == 1) ///poate castiga
        {
            if (a[linia][coloana].jucator == 0)
            {
                dimensiune = decide_dimensiunea_min_strat();
                if (dimensiune != -1)
                {
                    pune_o_piesa_min_strat(a, linia, coloana, calculator, dimensiune);
                    amMutat = 1;
                }
                else
                {
                    int liniaInitiala, coloanaInitiala;

                    int i, j, ok = 0, dimensiune = 0;
                    for (i = 0; i < 3 && !ok; ++i)
                        for (j = 0; j < 3 && !ok; ++j)
                        {
                            if (a[i][j].jucator == 2 && a[i][j].marimePiesa > dimensiune)
                            {
                                ok = 1;
                                liniaInitiala = i;
                                coloanaInitiala = j;
                            }
                        }
                    if (ok == 1)
                    {
                        muta_o_piesa_min_strat(a, liniaInitiala, coloanaInitiala, linia, coloana);
                        amMutat = 1;
                    }
                }
            }
            else ///unde poate el castiga este o piesa de-a noastra

            {
                if (a[linia][coloana].marimePiesa != 3) ///daca piesa noastra nu e de 3 poate sa o acopere
                {
                    int areRost = verifica_daca_poate_acoperi_piesa_min_strat(a[linia][coloana].marimePiesa, jucatorul);
                    if (areRost) ///poate sa puna o piesa peste a noastra
                    {
                        mutare3_min_strat(a, linia, coloana, amMutat);
                    }
                }
            }
        }

    }

    if (amMutat == 0)
    {
        ///nu castiga nimeni
        if (numarPieseMari2 == 0  && numarPieseMici2 == 0  && numarPieseMedii2 == 0 )
        {
            int l, c, d;
            l = 1;
            c = 1;
            d = decide_dimensiunea_min_strat();
            if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, l, c, calculator, d) == 1)
            {
                pune_o_piesa_min_strat(a, l, c, calculator, d);
                amMutat = 1;
            }
            else
            {
                int i, j, ok = 0;
                for (i = 0; i < 3 && !ok; ++i)
                    for (j = 0; j < 3 && !ok; ++j)
                    {
                        if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i, j, calculator, d) == 1)
                        {
                            pune_o_piesa_min_strat(a, i, j, calculator, d);
                            ok = 1;
                            amMutat = 1;
                        }
                    }
            }
        }

        else
        {
            if (verifica_daca_mai_are_piese_min_strat(calculator) == 1)
            {
                cout<<"Suntem in cazul in care trebuie sa se puna o piesa mica"<<endl;
                dimensiune = decide_dimensiunea_min_strat();
                int i, j, ok = 0, lf, cf;
                for (i = 0; i <= 2 && !ok; ++i)
                    for (j = 0; j <= 2 && !ok; ++j)
                    {
                        if (a[i][j].jucator == 2)
                        {
                            cout<<"Calculatorul are piesa la linia "<<i<<" "<<"si coloana "<<j<<endl;
                            if (cauta_pozitia_min_strat(a, i, j, lf, cf, dimensiune) == 1)
                            {
                                cout<<"lf="<<lf<<" "<<"cf="<<cf;
                                ok = 1;
                                pune_o_piesa_min_strat(a, lf, cf, calculator, dimensiune);
                                amMutat = 1;
                            }
                        }
                    }
            }
            else
            {
                int i, j, li, lf, ci, cf, d = 0, ok = 0;
                for (i = 0; i <= 2; ++i)
                    for (j = 0; j <= 2; ++j)
                    {
                        if (a[i][j].jucator == 2 && a[i][j].marimePiesa > d)
                        {
                            d = a[i][j].marimePiesa;
                            li = i;
                            ci = j;
                        }
                    }
                for (i = 0; i <= 2 && !ok; ++i)
                    for (j = 0; j <= 2 && !ok; ++j)
                    {
                        if (verifica_daca_se_poate_pune_o_piesa_min_strat(a, i, j, calculator, d) == 1)
                        {
                            lf = i;
                            cf = j;
                            ok = 1;
                        }
                    }
                muta_o_piesa_min_strat(a, li, ci, lf, cf);
                amMutat = 1;
            }
        }

    }
}

///asta imi trebuie doar ca sa afisez matricea in main sa vad ce mutari face calculatorul

void adaugare(stiva piese[3], int jucatorul, int marime)
{
    piese[2].player = piese[1].player;
    piese[2].dimensiune = piese[1].dimensiune;
    piese[1].player = piese[0].player;
    piese[1].dimensiune = piese[0].dimensiune;

    piese[0].player = jucatorul;
    piese[0].dimensiune = marime;
}

int verifica_daca_mai_are_piese_mici(int jucator)
{
    if (jucator == 1)
    {
        if (numarPieseMici1 == 2) return 0;
        return 1;
    }
    else
    {
        if (numarPieseMici2 == 2) return 0;
        return 1;
    }
}

int verifica_daca_mai_are_piese_medii(int jucator)
{
    if (jucator == 1)
    {
        if (numarPieseMedii1 == 2) return 0;
        return 1;
    }
    else
    {
        if (numarPieseMedii2 == 2) return 0;
        return 1;
    }
}

int verifica_daca_mai_are_piese_mari(int jucator)
{
    if (jucator == 1)
    {
        if (numarPieseMari1 == 2) return 0;
        return 1;
    }
    else
    {
        if (numarPieseMari2 == 2) return 0;
        return 1;
    }
}


int verifica_daca_mai_are_piese(int jucator)
{
    if (verifica_daca_mai_are_piese_mari(jucator) == 1 || verifica_daca_mai_are_piese_medii(jucator) == 1 || verifica_daca_mai_are_piese_mici(jucator) == 1) return 1;
    return 0;
}


int mutarea_calculatorului(matrice a[3][3])
{
    int calculator = 2, decizie = 0;
    if (numarPieseMari2 == 0 && numarPieseMedii2 == 0 && numarPieseMici2 == 0) decizie = 0;
    else
    {
        if (verifica_daca_mai_are_piese(calculator) == 1) decizie = rand() % 2;
        else decizie = 1; ///sa mute o piesa
    }
    return decizie;
}

void muta_o_piesa_pentru_calculator(matrice a[3][3], int& type, int& line, int& col, int& origy, int& origx)
{
    int i, j, ok = 0, liniaFinala, coloanaFinala, calculator = 2, liniaInitiala, coloanaInitiala, dimensiune = 0;
    for (i = 0; i <= 2; ++i)
        for (j = 0; j <= 2; ++j)
        {
            if (a[i][j].jucator == 0 || a[i][j].marimePiesa > dimensiune)
            {
                dimensiune = a[i][j].marimePiesa;
                liniaInitiala = i;
                coloanaInitiala = j;
            }
        }
    ok = 0;
    do
    {
        liniaFinala = rand() % 3;
        coloanaFinala = rand() % 3;
        ok = verifica_daca_se_poate_pune_o_piesa(a, liniaFinala, coloanaFinala, calculator, dimensiune);
    }
    while (ok == 0);

    type = a[liniaInitiala][coloanaInitiala].marimePiesa;
    line = liniaFinala;
    col = coloanaFinala;
    origy = liniaInitiala;
    origx = coloanaInitiala;
    muta_o_piesa(a, liniaInitiala, coloanaInitiala, liniaFinala, coloanaFinala);
}


void adauga_o_piesa_pentru_calculator(matrice a[3][3], int& type, int& line, int& col)
{
    int dimensiune = 0, calculator = 2, ok = 0;
    int linie = 0, coloana = 0, ok2 = 0;
    do
    {
        dimensiune = rand() % 3 + 1;
        type = dimensiune;
        ok2 = verifica_daca_mai_are_un_tip_de_piese(dimensiune, calculator);
    }
    while (ok2 == 0);

    do
    {
        linie = rand() % 3;
        line = linie;
        coloana = rand() % 3;
        col = coloana;
        ok = verifica_daca_se_poate_pune_o_piesa(a, linie, coloana, calculator, dimensiune);
    }
    while (ok == 0);
    pune_o_piesa(a, linie, coloana, calculator, dimensiune);
}

void stergere(stiva piese[3])
{

    piese[0].player = piese[1].player;
    piese[0].dimensiune = piese[1].dimensiune;
    piese[1].player = piese[2].player;
    piese[1].dimensiune = piese[2].dimensiune;
    piese[2].player = 0;
    piese[2].dimensiune = 0;
}

int verifica_daca_a_castigat(matrice a[3][3])
{
    int i, j;

    for (i = 0; i <= 2; ++i)
    {
        if ((a[i][0].jucator == a[i][1].jucator) && (a[i][1].jucator == a[i][2].jucator) && (a[i][0].jucator != 0)) return a[i][1].jucator;
    }
    for (j = 0; j <= 2; j++)
    {
        if ((a[0][j].jucator == a[1][j].jucator) && (a[1][j].jucator == a[2][j].jucator) && (a[0][j].jucator != 0)) return a[2][j].jucator;
    }
    if ((a[0][0].jucator == a[1][1].jucator) && (a[1][1].jucator == a[2][2].jucator) && (a[0][0].jucator != 0)) return a[0][0].jucator;
    if ((a[2][0].jucator == a[1][1].jucator) && (a[1][1].jucator == a[0][2].jucator) && (a[2][0].jucator != 0)) return a[0][2].jucator;
    return 0;
}

void pune_o_piesa(matrice a[3][3], int linie, int coloana, int jucatorul, int marime)
{
    a[linie][coloana].jucator = jucatorul;
    a[linie][coloana].marimePiesa = marime;
    adaugare(a[linie][coloana].piese, jucatorul, marime);
    if (jucatorul == 1)
    {
        if (marime == 1) numarPieseMici1++;
        else
        {
            if (marime == 3) numarPieseMari1++;
            else numarPieseMedii1++;
        }
    }
    else
    {
        if (marime == 1) numarPieseMici2++;
        else
        {
            if (marime == 3) numarPieseMari2++;
            else numarPieseMedii2++;
        }
    }
}

int verifica_daca_se_poate_pune_o_piesa(matrice a[3][3], int linie, int coloana, int jucatorul, int marime)
{

    if (a[linie][coloana].jucator == 0) return 1;
    if (a[linie][coloana].marimePiesa >= marime) return 0;
    return 1;
}

int verifica_daca_piesa_are_marimea_corecta(int marime)
{
    if (marime < 1 || marime>3) return 0;
    return 1;
}

int verifica_daca_mai_are_un_tip_de_piese(int marime, int jucatorul)
{
    if (marime == 1)
    {
        if (verifica_daca_mai_are_piese_mici(jucatorul) == 0) return 0;
        return 1;
    }
    else
    {
        if (marime == 2)
        {
            if (verifica_daca_mai_are_piese_medii(jucatorul) == 0) return 0;
            return 1;
        }

        else
        {
            if (verifica_daca_mai_are_piese_mari(jucatorul) == 0) return 0;
            else return 1;
        }
    }
}



void muta_o_piesa(matrice a[3][3], int liniaInitiala, int coloanaInitiala, int liniaFinala, int coloanaFinala)
{
    a[liniaFinala][coloanaFinala].jucator = a[liniaInitiala][coloanaInitiala].jucator;
    a[liniaFinala][coloanaFinala].marimePiesa = a[liniaInitiala][coloanaInitiala].marimePiesa;
    adaugare(a[liniaFinala][coloanaFinala].piese, a[liniaFinala][coloanaFinala].jucator, a[liniaFinala][coloanaFinala].marimePiesa);
    stergere(a[liniaInitiala][coloanaInitiala].piese);
    a[liniaInitiala][coloanaInitiala].jucator = a[liniaInitiala][coloanaInitiala].piese[0].player;
    a[liniaInitiala][coloanaInitiala].marimePiesa = a[liniaInitiala][coloanaInitiala].piese[0].dimensiune;

}

void randul_calculatorului(matrice a[3][3], int& decision, int& type, int& line, int& col, int& origy, int& origx)
{
    int decizie;
    decizie = mutarea_calculatorului(a);
    decision = decizie;
    if (decizie == 0)
    {
        adauga_o_piesa_pentru_calculator(a, type, line, col);
    }
    else muta_o_piesa_pentru_calculator(a, type, line, col, origy, origx);
}


void randul_jucatorului(matrice a[3][3], int originy, int originx, int posy, int posx, int jucatorul, int dimensiune)
{

    if (originy == -1)
    {
        mutareaAnterioara.tip = 1;
        mutareaAnterioara.dimensiuneaPiesei = dimensiune;
        mutareaAnterioara.l = posy;
        mutareaAnterioara.c = posx;
        pune_o_piesa(a, posy, posx, jucatorul, dimensiune);
    }
    else
    {

        mutareaAnterioara.tip = 2;
        mutareaAnterioara.li = originy;
        mutareaAnterioara.ci = originx;
        mutareaAnterioara.lf = posy;
        mutareaAnterioara.cf = posx;
        muta_o_piesa(a, originy, originx, posy, posx);
    }

}


int w = GetSystemMetrics(SM_CXSCREEN);
int h = GetSystemMetrics(SM_CYSCREEN);
int smol = 25, medium = 45, large = 65;
void winScreen(int player);

void infoScreen()
{
    cleardevice();
    readimagefile("info.jpg", 0, 0, w, h);
    readimagefile("close.jpg", 25, 25, 75, 75);

    bool infoScreen = true;

    setmousequeuestatus(WM_LBUTTONUP, true);

    int mousex = 0, mousey = 0, newx = 0, newy = 0;

    while (infoScreen)
    {
        getmouseclick(WM_LBUTTONUP, newx, newy);
        if (newx != mousex || newy != mousey)
        {
            mousex = newx;
            mousey = newy;
            if (mousex >= 25 && mousex <= 75 && mousey >= 25 && mousey <= 75)
            {
                infoScreen = false;
                homeScreen();
            }
        }
    }
}

struct piece
{
    int type, team;
    bool onboard;
};

struct boardState
{
    int leftx, lefty, stackL = 0;
    piece stack[3];
} board[3][3];

struct pieces
{
    int leftx, lefty, boardx, boardy;
    piece piece;
} p1[3][2], p2[3][2];

//imi trebuie pentru ales culoarea pe care o vreau la piese
char p1gobb[7] = "g1.gif", p2gobb[7] = "g2.gif";

bool addPiece(int i, int j, piece piece)
{

    cout << endl << "Adding piece to: " << j << " " << i << endl;
    cout<<"Echipa piesei de mutat: "<<piece.team<<endl;
    cout<<"Tipul piesei de mutat: "<<piece.type<<endl;

    bool valid = true;

    for (int x = 0; x < board[j][i].stackL; x++)
    {
        if (board[j][i].stack[x].type >= piece.type)
        {
            valid = false;
            cout<<"Se pare ca valid e fals pentru ca tipul piesei nu e bun"<<endl;
        }
    }
    if (board[j][i].stackL == 3) valid = false;

    char path[7];

    if (piece.team == 1) strcpy(path, p1gobb);
    else strcpy(path, p2gobb);
    setlinestyle(SOLID_LINE, 1, 3);

    if (valid)
    {
        board[j][i].stack[board[j][i].stackL] = piece;
        board[j][i].stack[board[j][i].stackL].onboard = true;
        board[j][i].stackL++;
        if (piece.type == 1)
        {
            readimagefile(path, board[i][j].leftx + 86, board[i][j].lefty + 75, board[i][j].leftx + 114, board[i][j].lefty + 125);
        }
        else if (piece.type == 2)
        {
            readimagefile(path, board[i][j].leftx + 71, board[i][j].lefty + 50, board[i][j].leftx + 128, board[i][j].lefty + 150);
        }
        else
        {
            readimagefile(path, board[i][j].leftx + 57, board[i][j].lefty + 25, board[i][j].leftx + 143, board[i][j].lefty + 175);
        }
    }

    cout<<"Aici este matricea interfetei cu echipe: "<<endl;
    for(int m = 0; m<3; m++){
        for(int n = 0; n<3; n++){
            cout<<board[m][n].stack[board[m][n].stackL - 1].team<<" ";
        }
        cout<<endl;
    }

    cout<<"Aici este matricea interfetei cu tipuri: "<<endl;
    for(int m = 0; m<3; m++){
        for(int n = 0; n<3; n++){
            cout<<board[m][n].stack[board[m][n].stackL - 1].type<<" ";
        }
        cout<<endl;
    }

    cout<<endl;
    return valid;

}

void removePieceFromSide(int x, int y, int team)
{
    setcolor(COLOR(255, 255, 255));
    setbkcolor(COLOR(255, 255, 255));
    setfillstyle(SOLID_FILL, COLOR(255, 255, 255));

    if (team == 1)
    {
        bar(p1[x][y].leftx, p1[x][y].lefty, p1[x][y].leftx + 150, p1[x][y].lefty + 150);
    }
    else
    {
        bar(p2[x][y].leftx, p2[x][y].lefty, p2[x][y].leftx + 150, p2[x][y].lefty + 150);
    }
}

void removePieceFromBoard(int x, int y)
{
    cout<<"Intram in stergerea piesei"<<endl;
    setcolor(COLOR(255, 255, 255));
    setbkcolor(COLOR(255, 255, 255));
    setfillstyle(SOLID_FILL, COLOR(255, 255, 255));
    board[y][x].stackL = board[y][x].stackL - 1;
    if (board[y][x].stackL > 0)
    {
        int type = board[y][x].stack[board[y][x].stackL - 1].type;
        int team = board[y][x].stack[board[y][x].stackL - 1].team;
        char path[7];
        if (team == 1) strcpy(path, p1gobb);
        else strcpy(path, p2gobb);
        setfillstyle(SOLID_FILL, COLOR(255, 255, 255));
        bar(board[x][y].leftx + 10, board[x][y].lefty + 10, board[x][y].leftx + 180, board[x][y].lefty + 180);
        if (type == 1)
        {
            readimagefile(path, board[x][y].leftx + 86, board[x][y].lefty + 75, board[x][y].leftx + 114, board[x][y].lefty + 125);
        }
        else if (type == 2)
        {
            readimagefile(path, board[x][y].leftx + 71, board[x][y].lefty + 50, board[x][y].leftx + 128, board[x][y].lefty + 150);
        }
        else
        {
            readimagefile(path, board[x][y].leftx + 57, board[x][y].lefty + 25, board[x][y].leftx + 143, board[x][y].lefty + 175);
        }
    }
    else
    {
        setfillstyle(SOLID_FILL, COLOR(255, 255, 255));
        bar(board[x][y].leftx + 10, board[x][y].lefty + 10, board[x][y].leftx + 180, board[x][y].lefty + 180);
    }
}

void putPieceBack(int x, int y, piece piece, int originx, int originy)
{

    if (piece.team == 1) setcolor(RED);
    else setcolor(BLUE);

    if (piece.onboard)
    {
        addPiece(originx, originy, piece);
    }
    else
    {
        if (piece.team == 1)
        {
            if (p1[x][y].piece.type == 1) readimagefile(p1gobb, p1[x][y].leftx + 61, p1[x][y].lefty + 50, p1[x][y].leftx + 89, p1[x][y].lefty + 100);
            else if (p1[x][y].piece.type == 2) readimagefile(p1gobb, p1[x][y].leftx + 49, p1[x][y].lefty + 30, p1[x][y].leftx + 100, p1[x][y].lefty + 120);
            else readimagefile(p1gobb, p1[x][y].leftx + 38, p1[x][y].lefty + 10, p1[x][y].leftx + 111, p1[x][y].lefty + 140);
        }
        else
        {
            if (p2[x][y].piece.type == 1) readimagefile(p2gobb, p2[x][y].leftx + 61, p2[x][y].lefty + 50, p2[x][y].leftx + 89, p2[x][y].lefty + 100);
            else if (p2[x][y].piece.type == 2) readimagefile(p2gobb, p2[x][y].leftx + 49, p2[x][y].lefty + 30, p2[x][y].leftx + 100, p2[x][y].lefty + 120);
            else readimagefile(p2gobb, p2[x][y].leftx + 38, p2[x][y].lefty + 10, p2[x][y].leftx + 111, p2[x][y].lefty + 140);
        }
    }
}

void whichPiece(int& x, int& y, pieces a[3][2], int mousex, int mousey)
{
    int i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 2; j++)
            if (mousex >= a[i][j].leftx && mousex <= a[i][j].leftx + 1503
                    && mousey >= a[i][j].lefty && mousey <= a[i][j].lefty + 150)
            {
                x = i;
                y = j;
            }
}

void printSides();

piece findOfSize(int type, int p)
{
    int i, j;
    piece x;
    if (p == 1)
    {
        int found = 0;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 2; j++)
                if (p1[i][j].piece.type == type && p1[i][j].piece.onboard == false && found == 0)
                {
                    x = p1[i][j].piece;
                    p1[i][j].piece.onboard = true;
                    removePieceFromSide(i, j, 1);
                    found = 1;
                }
    }
    else
    {
        int found = 0;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 2; j++)
                if (p2[i][j].piece.type == type && p2[i][j].piece.onboard == false && found == 0)
                {
                    x = p2[i][j].piece;
                    p2[i][j].piece.onboard = true;
                    removePieceFromSide(i, j, 2);
                    found = 1;
                }
    }

    printSides();

    return x;
}

void printSides()
{
    int i, j;

    cout << endl << "SIDE 1: ";
    cout << endl;
    for (int i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++) cout << p1[i][j].piece.onboard;
        cout << endl;
    }
    cout << endl << "SIDE 2: ";
    cout << endl;
    for (int i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++) cout << p2[i][j].piece.onboard;
        cout << endl;
    }

}


void gameScreen()
{

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) board[i][j].stackL = 0;

    if (player1 == 2) currentPlayer = 1;
    else if (player2 == 2) currentPlayer = 2;
    else currentPlayer = (rand() % 2) + 1;

    readimagefile("bg.jpg", 0, 0, w, h);
    setlinestyle(SOLID_LINE, 1, 7);
    setfillstyle(SOLID_FILL, COLOR(255, 255, 255));
    setcolor(BLACK);

    int squarex = (w - 600) / 2, squarey = (h - 600) / 2;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j].leftx = squarex + 200 * i;
            board[i][j].lefty = squarey + 200 * j;
            bar(board[i][j].leftx, board[i][j].lefty, board[i][j].leftx + 200, board[i][j].lefty + 200);
        }
    }

    line(squarex, squarey, squarex, squarey + 600);
    line(squarex, squarey, squarex + 600, squarey);
    line(squarex + 600, squarey, squarex + 600, squarey + 600);
    line(squarex, squarey + 600, squarex + 600, squarey + 600);

    for (int i = 1; i <= 2; i++)
    {
        line(squarex + 200 * i, squarey, squarex + 200 * i, squarey + 600);
        line(squarex, squarey + 200 * i, squarex + 600, squarey + 200 * i);
    }

    setmousequeuestatus(WM_LBUTTONUP, true);

    int mousex = 0, mousey = 0, newx = 0, newy = 0;

    int fPieceX = (squarex - 300) / 2, fPieceY = (h - 450) / 2,
        sPieceX = (squarex + 600) + (w - (squarex + 600) - 300) / 2, sPieceY = (h - 450) / 2;

    setlinestyle(DOTTED_LINE, 1, 4);

    putpixel(sPieceX, sPieceY, 0);

    p1[0][0].leftx = fPieceX;
    p1[0][0].lefty = fPieceY;
    p2[0][0].leftx = sPieceX;
    p2[0][0].lefty = sPieceY;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++)
        {
            if (i == 0)
            {
                p1[i][j].piece.type = 1;
                p2[i][j].piece.type = 1;
            }
            else if (i == 1)
            {
                p1[i][j].piece.type = 2;
                p2[i][j].piece.type = 2;
            }
            else
            {
                p1[i][j].piece.type = 3;
                p2[i][j].piece.type = 3;
            }
            p1[i][j].piece.team = 1;
            p2[i][j].piece.team = 2;
            p1[i][j].piece.onboard = false;
            p2[i][j].piece.onboard = false;
        }

    p1[0][1].leftx = p1[0][0].leftx + 150;
    p1[0][1].lefty = p1[0][0].lefty;
    p2[0][1].leftx = p2[0][0].leftx + 150;
    p2[0][1].lefty = p2[0][0].lefty;

    for (int i = 1; i < 3; i++)
        for (int j = 0; j < 2; j++)
        {
            if (j == 0)
            {
                p1[i][j].leftx = p1[i - 1][j].leftx;
                p1[i][j].lefty = p1[i - 1][j].lefty + 150;
                p2[i][j].leftx = p2[i - 1][j].leftx;
                p2[i][j].lefty = p2[i - 1][j].lefty + 150;
            }
            else
            {
                p1[i][j].leftx = p1[i][j - 1].leftx + 150;
                p1[i][j].lefty = p1[i][j - 1].lefty;
                p2[i][j].leftx = p2[i][j - 1].leftx + 150;
                p2[i][j].lefty = p2[i][j - 1].lefty;
            }
        }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bar(p1[i][j].leftx, p1[i][j].lefty, p1[i][j].leftx + 150, p1[i][j].lefty + 150);
            bar(p2[i][j].leftx, p2[i][j].lefty, p2[i][j].leftx + 150, p2[i][j].lefty + 150);
            if (p1[i][j].piece.type == 1)
            {
                setcolor(RED);
                readimagefile(p1gobb, p1[i][j].leftx + 61, p1[i][j].lefty + 50, p1[i][j].leftx + 89, p1[i][j].lefty + 100);
                setcolor(BLUE);
                readimagefile(p2gobb, p2[i][j].leftx + 61, p2[i][j].lefty + 50, p2[i][j].leftx + 89, p2[i][j].lefty + 100);
            }
            else if (p1[i][j].piece.type == 2)
            {
                setcolor(RED);
                readimagefile(p1gobb, p1[i][j].leftx + 49, p1[i][j].lefty + 30, p1[i][j].leftx + 100, p1[i][j].lefty + 120);
                setcolor(BLUE);
                readimagefile(p2gobb, p2[i][j].leftx + 49, p2[i][j].lefty + 30, p2[i][j].leftx + 100, p2[i][j].lefty + 120);
            }
            else
            {
                setcolor(RED);
                readimagefile(p1gobb, p1[i][j].leftx + 38, p1[i][j].lefty + 10, p1[i][j].leftx + 111, p1[i][j].lefty + 140);
                setcolor(BLUE);
                readimagefile(p2gobb, p2[i][j].leftx + 38, p2[i][j].lefty + 10, p2[i][j].leftx + 111, p2[i][j].lefty + 140);
            }
        }
    }

    setlinestyle(SOLID_LINE, 1, 5);

    line(fPieceX, fPieceY, fPieceX + 300, fPieceY);
    line(fPieceX, fPieceY, fPieceX, fPieceY + 450);
    line(fPieceX, fPieceY + 450, fPieceX + 300, fPieceY + 450);
    line(fPieceX + 300, fPieceY, fPieceX + 300, fPieceY + 450);
    line(sPieceX, sPieceY, sPieceX + 300, sPieceY);
    line(sPieceX, sPieceY, sPieceX, sPieceY + 450);
    line(sPieceX, sPieceY + 450, sPieceX + 300, sPieceY + 450);
    line(sPieceX + 300, sPieceY, sPieceX + 300, sPieceY + 450);

    int next1 = textwidth("Player 1 now has to move"),
        next2 = textwidth("Player 2 now has to move");

    bool picked = false;
    int pickx, picky, originx = 1, originy = 1;
    piece pickedPiece;
    piece pieceToAdd;

    setbkcolor(COLOR(0, 0, 0));
    setcolor(WHITE);

    if (currentPlayer == 1)
    {
        outtextxy((w - next1) / 2, h - 100, "Player 1 now has to move");
    }
    else
    {
        outtextxy((w - next1) / 2, h - 100, "Player 2 now has to move");
    }

    int tur = 0;

    int win;

    while (!stadiul)
    {
        if (currentPlayer == 1 && player1 == 2 && difLeftLev == 1 || currentPlayer == 2 && player2 == 2 && difRightLev == 1)
        {
            int wp;
            if (currentPlayer == 1) wp = 1;
            else wp = 2;
            int decision, type, line, col, origy, origx;
            randul_calculatorului(a, decision, type, line, col, origy, origx);
            if (decision == 0)
            {
                piece x = findOfSize(type, wp);
                addPiece(col, line, x);
                afisare(a);
            }
            else
            {
                removePieceFromBoard(origx, origy);
                addPiece(col, line, board[origx][origy].stack[board[origx][origy].stackL]);
                afisare(a);
            }
            setbkcolor(COLOR(0, 0, 0));
            setcolor(WHITE);
            if (currentPlayer == 1)
            {
                currentPlayer = 2;
                outtextxy((w - next1) / 2, h - 100, "Player 2 now has to move");
            }
            else
            {
                currentPlayer = 1;
                outtextxy((w - next1) / 2, h - 100, "Player 1 now has to move");
            }
            win = verifica_daca_a_castigat(a);
            if (win)
            {
                stadiul = 1;
                if(currentPlayer == 1)
                {
                    if(strcmp(p1gobb, "g1.gif")==0) g1s++;
                    else if(strcmp(p1gobb, "g2.gif")==0) g2s++;
                    else if(strcmp(p1gobb, "g3.gif")==0) g3s++;
                    else if(strcmp(p1gobb, "g4.gif")==0) g4s++;
                }
                else
                {
                    if(strcmp(p2gobb, "g1.gif")==0) g1s++;
                    else if(strcmp(p2gobb, "g2.gif")==0) g2s++;
                    else if(strcmp(p2gobb, "g3.gif")==0) g3s++;
                    else if(strcmp(p2gobb, "g4.gif")==0) g4s++;
                }
                winScreen(win);
            }
        }
        else if (currentPlayer == 1 && player1 == 2 && difLeftLev == 2 || currentPlayer == 2 && player2 == 2 && difRightLev == 2)
        {
            randul_calculatorului_min_strat(a);
            if (g.change)
            {
                if (g.decision == 0)
                {
                    piece x;
                    if (currentPlayer == 1) x = findOfSize(g.type, 2);
                    else x = findOfSize(g.type, 2);
                    addPiece(g.fc, g.fl, x);
                }
                else
                {
                    cout<<"Suntem in cazul mutarii piesei cu piece to add"<<endl;
                    cout<<"Old line "<<g.ol<<" Old column "<<g.oc;
                    pieceToAdd = board[g.ol][g.oc].stack[board[g.ol][g.oc].stackL - 1];
                    cout<<"Team piesa "<<pieceToAdd.team<<endl;
                    cout<<"Type piesa "<<pieceToAdd.type<<endl;
                    removePieceFromBoard(g.oc, g.ol);
                    addPiece(g.fc, g.fl, pieceToAdd);
                }
                g.change = false;
            }
            afisare(a);
            setbkcolor(COLOR(0, 0, 0));
            setcolor(WHITE);
            if (currentPlayer == 1)
            {
                currentPlayer = 2;
                outtextxy((w - next1) / 2, h - 100, "Player 2 now has to move");
            }
            else
            {
                currentPlayer = 1;
                outtextxy((w - next1) / 2, h - 100, "Player 1 now has to move");
            }
            win = verifica_daca_a_castigat(a);
            if (win)
            {
                stadiul = 1;
                if(currentPlayer == 2)
                {
                    if(strcmp(p1gobb, "g1.gif")==0) g1s++;
                    else if(strcmp(p1gobb, "g2.gif")==0) g2s++;
                    else if(strcmp(p1gobb, "g3.gif")==0) g3s++;
                    else if(strcmp(p1gobb, "g4.gif")==0) g4s++;
                }
                else
                {
                    if(strcmp(p2gobb, "g1.gif")==0) g1s++;
                    else if(strcmp(p2gobb, "g2.gif")==0) g2s++;
                    else if(strcmp(p2gobb, "g3.gif")==0) g3s++;
                    else if(strcmp(p2gobb, "g4.gif")==0) g4s++;
                }
                winScreen(win);
            }
        }
        else if (currentPlayer == 1 && player1 == 2 && difLeftLev == 3 || currentPlayer == 2 && player2 == 2 && difRightLev == 3)
        {
            afisare(a);
            if (g.change)
            {
                if (g.decision == 0)
                {
                    piece x;
                    if (currentPlayer == 1) x = findOfSize(g.type, 2);
                    else x = findOfSize(g.type, 2);
                    addPiece(g.fl, g.fc, x);
                }
                else
                {
                    removePieceFromBoard(g.ol, g.oc);
                    addPiece(g.fl, g.fc, board[g.ol][g.oc].stack[board[g.ol][g.oc].stackL]);
                }
                g.change = false;
            }
            cout << endl << "FINAL DISPLAY" << endl;
            afisare(a);
            setbkcolor(COLOR(0, 0, 0));
            setcolor(WHITE);
            if (currentPlayer == 1)
            {
                currentPlayer = 2;
                outtextxy((w - next1) / 2, h - 100, "Player 2 now has to move");
            }
            else
            {
                currentPlayer = 1;
                outtextxy((w - next1) / 2, h - 100, "Player 1 now has to move");
            }
            win = verifica_daca_a_castigat(a);
            if (win)
            {
                stadiul = 1;
                if(currentPlayer == 1)
                {
                    if(strcmp(p1gobb, "g1.gif")==0) g1s++;
                    else if(strcmp(p1gobb, "g2.gif")==0) g2s++;
                    else if(strcmp(p1gobb, "g3.gif")==0) g3s++;
                    else if(strcmp(p1gobb, "g4.gif")==0) g4s++;
                }
                else
                {
                    if(strcmp(p2gobb, "g1.gif")==0) g1s++;
                    else if(strcmp(p2gobb, "g2.gif")==0) g2s++;
                    else if(strcmp(p2gobb, "g3.gif")==0) g3s++;
                    else if(strcmp(p2gobb, "g4.gif")==0) g4s++;
                }
                winScreen(win);
            }
        }

        else
        {
            getmouseclick(WM_LBUTTONUP, newx, newy);
            if (newx != mousex || newy != mousey)
            {
                mousex = newx;
                mousey = newy;
                if (mousex >= squarex && mousex <= squarex + 600 && mousey >= squarey && mousey <= squarey + 600)
                {
                    int posx, posy;
                    if (mousex > squarex&& mousex < squarex + 200) posx = 0;
                    if (mousex > squarex + 200 && mousex < squarex + 200 * 2) posx = 1;
                    if (mousex > squarex + 200 * 2 && mousex < squarex + 200 * 3) posx = 2;
                    if (mousey > squarey&& mousey < squarey + 200) posy = 0;
                    if (mousey > squarey + 200 && mousey < squarey + 200 * 2) posy = 1;
                    if (mousey > squarey + 200 * 2 && mousey < squarey + 200 * 3) posy = 2;
                    if (picked)
                    {
                        mutareaAnterioara.tip = 0;
                        mutareaAnterioara.l = 0;
                        mutareaAnterioara.c = 0;
                        mutareaAnterioara.li = 0;
                        mutareaAnterioara.ci = 0;
                        mutareaAnterioara.lf = 0;
                        mutareaAnterioara.cf = 0;
                        bool valid;
                        int win = 0, draw=0;
                        if (pickedPiece.team == 1)
                        {
                            cout<<"Cunoastem echipa piesei pe care trebuie sa o mutam";
                            valid = addPiece(posx, posy, pickedPiece);
                            if (!valid)
                            {
                                putPieceBack(pickx, picky, pickedPiece, originx, originy);
                            }
                            else
                            {
                                if (pickedPiece.onboard)
                                {
                                    randul_jucatorului(a, originy, originx, posy, posx, pickedPiece.team, pickedPiece.type);
                                }
                                else
                                {
                                    mutareaAnterioara.tip = 1;
                                    randul_jucatorului(a, -1, -1, posy, posx, pickedPiece.team, pickedPiece.type);
                                    p1[pickx][picky].piece.onboard = true;
                                }
                                win = verifica_daca_a_castigat(a);
                                draw = remiza(a);
                                if(!draw){
                                if (win)
                                {
                                    stadiul = 1;
                                    if(currentPlayer == 1)
                                    {
                                        if(strcmp(p1gobb, "g1.gif")==0) g1s++;
                                        else if(strcmp(p1gobb, "g2.gif")==0) g2s++;
                                        else if(strcmp(p1gobb, "g3.gif")==0) g3s++;
                                        else if(strcmp(p1gobb, "g4.gif")==0) g4s++;
                                    }
                                    else
                                    {
                                        if(strcmp(p2gobb, "g1.gif")==0) g1s++;
                                        else if(strcmp(p2gobb, "g2.gif")==0) g2s++;
                                        else if(strcmp(p2gobb, "g3.gif")==0) g3s++;
                                        else if(strcmp(p2gobb, "g4.gif")==0) g4s++;
                                    }
                                    winScreen(win);
                                }
                                }
                                else
                                 drawScreen(draw);
                            }
                            picked = false;
                            jucatorul = 2;
                        }
                        else
                        {
                            cout<<"Suntem in cazul unei mutari fara sa cunoastem echipa piesei"<<endl;
                            valid = addPiece(posx, posy, pickedPiece);
                            if (!valid)
                            {
                                putPieceBack(pickx, picky, pickedPiece, originx, originy);
                            }
                            else
                            {
                                if (pickedPiece.onboard)
                                {
                                    randul_jucatorului(a, originy, originx, posy, posx, pickedPiece.team, pickedPiece.type);
                                }
                                else
                                {
                                    mutareaAnterioara.tip = 1;
                                    randul_jucatorului(a, -1, -1, posy, posx, pickedPiece.team, pickedPiece.type);
                                    p2[pickx][picky].piece.onboard = true;
                                }
                                win = verifica_daca_a_castigat(a);
                                if (win)
                                {
                                    stadiul = 1;
                                    if(currentPlayer == 1)
                                    {
                                        if(strcmp(p1gobb, "g1.gif")==0) g1s++;
                                        else if(strcmp(p1gobb, "g2.gif")==0) g2s++;
                                        else if(strcmp(p1gobb, "g3.gif")==0) g3s++;
                                        else if(strcmp(p1gobb, "g4.gif")==0) g4s++;
                                    }
                                    else
                                    {
                                        if(strcmp(p2gobb, "g1.gif")==0) g1s++;
                                        else if(strcmp(p2gobb, "g2.gif")==0) g2s++;
                                        else if(strcmp(p2gobb, "g3.gif")==0) g3s++;
                                        else if(strcmp(p2gobb, "g4.gif")==0) g4s++;
                                    }
                                    winScreen(win);
                                }
                            }
                            picked = false;
                        }
                        if (!win)
                        {
                            setbkcolor(COLOR(0, 0, 0));
                            setcolor(WHITE);
                            if (valid)
                            {
                                if (currentPlayer == 1)
                                {
                                    currentPlayer = 2;
                                    outtextxy((w - next1) / 2, h - 100, "Player 2 now has to move");
                                }
                                else
                                {
                                    currentPlayer = 1;
                                    outtextxy((w - next1) / 2, h - 100, "Player 1 now has to move");
                                }
                            }
                        }
                        afisare(a);
                    }
                    else
                    {
                        if (board[posx][posy].stackL > 0)
                        {
                            if (board[posx][posy].stack[board[posx][posy].stackL - 1].team == currentPlayer)
                            {
                                removePieceFromBoard(posx, posy);
                                picked = true;
                                originx = posx;
                                originy = posy;
                                pickedPiece = board[posx][posy].stack[board[posx][posy].stackL];
                            }
                        }
                    }
                }
                else if (mousex >= fPieceX && mousex <= fPieceX + 300
                         && mousey >= fPieceY && mousey <= fPieceY + 450 && !picked && currentPlayer == 1)
                {
                    int x, y;
                    whichPiece(x, y, p1, mousex, mousey);
                    if (!p1[x][y].piece.onboard)
                    {
                        picked = true;
                        pickx = x;
                        picky = y;
                        pickedPiece = p1[pickx][picky].piece;
                        removePieceFromSide(x, y, 1);
                    }
                }
                else if (mousex >= sPieceX && mousex <= sPieceX + 300
                         && mousey >= sPieceY && mousey <= sPieceY + 450 && !picked && currentPlayer == 2)
                {
                    int x, y;
                    whichPiece(x, y, p2, mousex, mousey);
                    if (!p2[x][y].piece.onboard)
                    {
                        picked = true;
                        pickx = x;
                        picky = y;
                        pickedPiece = p2[pickx][picky].piece;
                        removePieceFromSide(x, y, 2);
                    }
                }
            }
        }
    }
}

void resetMatrix();

void drawScreen(int player){

 Sleep(500);
    readimagefile("bg.jpg", 0, 0, w, h);
    settextstyle(TRIPLEX_SCR_FONT, HORIZ_DIR, 5);
    setcolor(BLACK);
    setbkcolor(WHITE);
    int p1w = textwidth("It's a draw!"),
        p1h = textheight("It's a draw!"),
        p2h = textheight("It's a draw!"),
        p2w = textwidth("It's a draw!");

    if (player == 1)
    {
        outtextxy((w - p1w) / 2, (h - p1h) / 2, "It's a draw!");
    }
    else
    {
        outtextxy((w - p2w) / 2, (h - p2h) / 2, "It's a draw!");
    }

    Sleep(5000);
    resetMatrix();
    scoreScreen();
}

void winScreen(int player)
{
    Sleep(500);
    readimagefile("bg.jpg", 0, 0, w, h);
    settextstyle(TRIPLEX_SCR_FONT, HORIZ_DIR, 5);
    setcolor(BLACK);
    setbkcolor(WHITE);
    int p1w = textwidth("Player 1 has won!"),
        p1h = textheight("Player 1 has won!"),
        p2h = textheight("Player 2 has won!"),
        p2w = textwidth("Player 2 has won!");

    if (player == 1)
    {
        outtextxy((w - p1w) / 2, (h - p1h) / 2, "Player 1 has won!");
    }
    else
    {
        outtextxy((w - p2w) / 2, (h - p2h) / 2, "Player 2 has won!");
    }

    Sleep(5000);
    resetMatrix();
    scoreScreen();
}

int lgobb = 1, rgobb = 2;

void chooseGobblet(int lgobb, int rgobb, int rsqx, int lsqx, int lsqy)
{
    setfillstyle(SOLID_FILL, WHITE);
    bar(lsqx + 75, lsqy + 280, lsqx + 225, lsqy + 430); //left sq
    bar(rsqx - 225 , lsqy + 285, rsqx - 75, lsqy + 430); //right sq

    setcolor(BLACK);
    setbkcolor(WHITE);

    if(lgobb == 1)
    {
        readimagefile("g1.gif", lsqx + 110, lsqy + 315, lsqx + 190, lsqy + 420 );
        outtextxy (lsqx + (300 - textwidth("< Genny >"))/2, lsqy + 295, "< Genny >");
    }
    if(lgobb == 2)
    {
        readimagefile("g2.gif", lsqx + 110, lsqy + 315, lsqx + 190, lsqy + 420  );
        outtextxy (lsqx + (300 - textwidth("< Geffy >"))/2, lsqy + 295, "< Geffy >");
    }
    if(lgobb == 3)
    {
        readimagefile("g3.gif", lsqx + 110, lsqy + 315, lsqx + 190, lsqy + 420 );
        outtextxy (lsqx + (300 - textwidth("< Gerry >"))/2, lsqy + 295, "< Gerry >");
    }
    if(lgobb == 4)
    {
        readimagefile("g4.gif", lsqx + 110, lsqy + 315, lsqx + 190, lsqy + 420  );
        outtextxy (lsqx + (300 - textwidth("< Gabby >"))/2, lsqy + 295, "< Gabby >");
    }

    if(rgobb == 1)
    {
        readimagefile("g1.gif", rsqx - 190 , lsqy + 315, rsqx - 110, lsqy + 420 );
        outtextxy (rsqx -300 + (300 - textwidth("< Genny >"))/2, lsqy + 295, "< Genny >");
    }
    if(rgobb == 2)
    {
        readimagefile("g2.gif", rsqx - 190 , lsqy + 315, rsqx - 110, lsqy + 420);
        outtextxy (rsqx - 300 + (300 - textwidth("< Geffy >"))/2, lsqy + 295, "< Geffy >");
    }
    if(rgobb == 3)
    {
        readimagefile("g3.gif", rsqx - 190 , lsqy + 315, rsqx - 110, lsqy + 420 );
        outtextxy (rsqx -300 + (300 - textwidth("< Gerry >"))/2, lsqy + 295, "< Gerry >");
    }
    if(rgobb == 4)
    {
        readimagefile("g4.gif", rsqx - 190 , lsqy + 315, rsqx - 110, lsqy + 420 );
        outtextxy (rsqx -300 + (300 - textwidth("< Gabby >"))/2, lsqy + 295, "< Gabby >");
    }

}

void paintDifficulty(bool remove, int level, int side, int rsqx, int lsqx, int lsqy, int& difrxpos, int& difrypos, int& diflxpos, int& diflypos)
{

    int difw = textwidth("Difficulty"),
        difh = textheight("Difficulty"),
        l1w = textwidth("< Level 1 >"),
        l1h = textheight("< Level 1 >"),
        l2w = textwidth("< Level 2 >"),
        l2h = textheight("< Level 2 >");


    setcolor(WHITE);

    if (remove)
    {
        setbkcolor(COLOR(50, 50, 50));
    }
    else
    {
        setbkcolor(COLOR(50, 50, 50));
        //setcolor(WHITE);
    }
    setcolor(WHITE);

    if (level == 1)
    {
        if (side == 0 || side == 2)
        {
            outtextxy(lsqx + (300 - difw) / 2, lsqy + 200, "Difficulty");
            outtextxy(lsqx + (300 - l1w) / 2, lsqy + 250, "< Level 1 >");
            diflxpos = lsqx + (300 - l1w) / 2;
            diflypos = lsqy + 250;
        }
        if (side == 1 || side == 2)
        {
            outtextxy(rsqx - 300 + (300 - difw) / 2, lsqy + 200, "Difficulty");
            outtextxy(rsqx - 300 + (300 - l1w) / 2, lsqy + 250, "< Level 1 >");
            difrxpos = rsqx - 300 + (300 - l1w) / 2;
            difrypos = lsqy + 250;
        }
    }
    else if (level == 2)
    {
        if (side == 0 || side == 2)
        {
            outtextxy(lsqx + (300 - difw) / 2, lsqy + 200, "Difficulty");
            outtextxy(lsqx + (300 - l2w) / 2, lsqy + 250, "< Level 2 >");
            diflxpos = lsqx + (300 - l2w) / 2;
            diflypos = lsqy + 250;
        }
        if (side == 1 || side == 2)
        {
            outtextxy(rsqx - 300 + (300 - difw) / 2, lsqy + 200, "Difficulty");
            outtextxy(rsqx - 300 + (300 - l2w) / 2, lsqy + 250, "< Level 2 >");
            difrxpos = rsqx - 300 + (300 - l2w) / 2;
            difrypos = lsqy + 250;
        }
    }
    chooseGobblet( lgobb, rgobb, rsqx, lsqx, lsqy);
    //setbkcolor(COLOR(0, 0, 0));

}

void resetMatrix()
{
    int i, j;

    stadiul = 0;

    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
        {
            a[i][j].jucator = 0;
            a[i][j].piese[0].player = 0;
            a[i][j].piese[1].player = 0;
            a[i][j].piese[2].player = 0;
            a[i][j].piese[0].dimensiune = 0;
            a[i][j].piese[1].dimensiune = 0;
            a[i][j].piese[2].dimensiune = 0;
            a[i][j].marimePiesa = 100;
        }

}

void pickUsers()
{
    clearviewport();

    setlinestyle(SOLID_LINE, 1, 5);
    settextstyle(8, HORIZ_DIR, 3);

    bool users = true;

    readimagefile("bg.jpg", 0, 0, w, h);
    /*setbkcolor(COLOR(255, 150, 5));*/

    int entertw = textwidth("Click here to Start Game"),
        enterh = textheight("Click here to Start Game");

    outtextxy((w - entertw) / 2, h - 150, "Click here to Start Game");

    // set up left user square
    int lsqx = 300, lsqy = (h - 400) / 2;

    line(lsqx, lsqy, lsqx + 300, lsqy);
    line(lsqx, lsqy, lsqx, lsqy + 450);
    line(lsqx + 300, lsqy, lsqx + 300, lsqy + 450);
    line(lsqx, lsqy + 450, lsqx + 300, lsqy + 450);

    // set up right user square
    int rsqx = w - 300, rsqy = (h - 400) / 2;

    line(rsqx, rsqy, rsqx - 300, rsqy);
    line(rsqx, rsqy, rsqx, rsqy + 450);
    line(rsqx - 300, rsqy, rsqx - 300, rsqy + 450);
    line(rsqx, rsqy + 450, rsqx - 300, rsqy + 450);

    // text setup
    int pone = textwidth("Player #1"), ptwo = textwidth("Player #2");

    outtextxy(lsqx + (300 - pone) / 2, lsqy + 50, "Player #1");
    outtextxy(rsqx - 300 + (300 - ptwo) / 2, lsqy + 50, "Player #2");

    int ptype = textwidth("<  Human  >"),
        btype = textwidth("< Machine >"),
        ptypeh = textheight("<  Human  >"),
        btypeh = textheight("< Machine >"),
        difw = textwidth("Difficulty"),
        difh = textheight("Difficulty"),
        l1w = textwidth("< Level 1 >"),
        l1h = textheight("< Level 1 >"),
        l2w = textwidth("< Level 2 >"),
        l2h = textheight("< Level 2 >");


    outtextxy(lsqx + (300 - ptype) / 2, lsqy + 100, "<  Human  >");
    outtextxy(rsqx - 300 + (300 - ptype) / 2, lsqy + 100, "<  Human  >");

    int difrxpos, difrypos, diflxpos, diflypos, difLeftWidth = l1w, difRightWidth = l1w;

    paintDifficulty(true, 1, 2, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
    chooseGobblet(lgobb, rgobb, rsqx, lsqx, lsqy);


    setmousequeuestatus(WM_LBUTTONUP, true);
    int mousex = 0, mousey = 0, newx = 0, newy = 0;
    bool lbot = false, rbot = false;

    while (users)
    {
        getmouseclick(WM_LBUTTONUP, newx, newy);
        if (newx != mousex || newy != mousey)
        {
            mousex = newx;
            mousey = newy;
            if (mousex >= (w - entertw) / 2
                    && mousex <= (w - entertw) / 2 + entertw
                    && mousey >= (h - 150)
                    && mousey <= (h - 150 + enterh))
            {
                if (lbot) player1 = 2;
                if (rbot) player2 = 2;
                gameScreen();
            }
            if (lbot)
            {
                if (mousex >= lsqx + (300 - btype) / 2
                        && mousex <= (lsqx + (300 - btype) / 2) + btype
                        && mousey >= lsqy + 100
                        && mousey <= (lsqy + 100) + btypeh)
                {
                    outtextxy(lsqx + (300 - ptype) / 2, lsqy + 100, "<   Human   >");
                    paintDifficulty(true, 1, 0, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                    difLeftLev = 0;
                    lbot = false;
                }
                else if (mousex >= diflxpos
                         && mousey >= diflypos
                         && mousex <= diflxpos + difLeftWidth
                         && mousey <= diflypos + l1h)
                {
                    if (difLeftLev == 1)
                    {
                        paintDifficulty(false, 2, 0, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                        difLeftLev = 2;
                        difLeftWidth = l2w;
                    }
                    else if (difLeftLev == 2)
                    {
                        paintDifficulty(false, 3, 0, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                        // difLeftLev = 3;
                        difLeftWidth = l1w;
                    }

                }
            }
            else
            {
                if (mousex >= lsqx + (300 - ptype) / 2
                        && mousex <= (lsqx + (300 - ptype) / 2) + ptype
                        && mousey >= lsqy + 100
                        && mousey <= (lsqy + 100) + ptypeh)
                {
                    outtextxy(lsqx + (300 - ptype) / 2, lsqy + 100, "<  Machine  >");
                    paintDifficulty(false, 1, 0, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                    lbot = true;
                }
            }
            if (rbot)
            {
                if (mousex >= (rsqx - 300 + (300 - ptype) / 2)
                        && mousex <= (rsqx - 300 + (300 - ptype) / 2) + ptype
                        && mousey >= lsqy + 100
                        && mousey <= (lsqy + 100) + ptypeh)
                {
                    outtextxy(rsqx - 300 + (300 - ptype) / 2, lsqy + 100, "<   Human   >");
                    paintDifficulty(true, 1, 1, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                    rbot = false;
                }
                else if (mousex >= difrxpos
                         && mousey >= difrypos
                         && mousex <= difrxpos + difRightWidth
                         && mousey <= difrypos + l1h)
                {
                    if (difRightLev == 1)
                    {
                        paintDifficulty(false, 2, 1, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                        difRightLev = 2;
                        difRightWidth = l2w;
                    }
                    else if (difRightLev == 2)
                    {
                        paintDifficulty(false, 1, 1, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                        difRightLev = 1;
                        difRightWidth = l1w;
                    }
                }
            }
            else
            {
                setcolor(WHITE);
                setbkcolor(BLACK);

                if (mousex >= (rsqx - 300 + (300 - ptype) / 2)
                        && mousex <= (rsqx - 300 + (300 - ptype) / 2) + ptype
                        && mousey >= lsqy + 100
                        && mousey <= (lsqy + 100) + ptypeh)
                {
                    outtextxy(rsqx - 300 + (300 - ptype) / 2, lsqy + 100, "<  Machine  >");
                    paintDifficulty(false, 1, 1, rsqx, lsqx, lsqy, difrxpos, difrypos, diflxpos, diflypos);
                    rbot = true;
                }
            }
            if(mousex >= lsqx + 75 && mousex <= lsqx + 225
                    && mousey >= lsqy + 280 && mousey <= lsqy + 430 )  // lsqx + 75, lsqy + 280, lsqx + 225, lsqy + 430
            {
                    if(lgobb == 1)
                    {
                        lgobb = 2;
                        strcpy (p1gobb, "g2.gif");

                        if(rgobb == 2)
                        {
                            rgobb = 3;
                            strcpy (p2gobb, "g3.gif");
                        }
                    }
                    else
                        if(lgobb == 2)
                        {
                            lgobb = 3;
                            strcpy (p1gobb, "g3.gif");

                            if(rgobb == 3)
                            {
                                rgobb = 4;
                                strcpy (p2gobb, "g4.gif");
                            }
                        }
                        else if(lgobb == 3)
                            {
                                lgobb = 4;
                                strcpy (p1gobb, "g4.gif");

                                if(rgobb == 4)
                                {
                                    rgobb = 1;
                                    strcpy (p2gobb, "g1.gif");
                                }
                            }
                            else if(lgobb == 4)
                                {
                                    lgobb = 1;
                                    strcpy (p1gobb, "g1.gif");

                                    if(rgobb == 1)
                                    {
                                        rgobb = 2;
                                        strcpy (p2gobb, "g2.gif");
                                    }
                                }
                    chooseGobblet( lgobb, rgobb, rsqx, lsqx, lsqy);
            }
            else
                if(mousex >= rsqx - 225 && mousex <= rsqx - 75
                        && mousey >= lsqy + 285 && mousey <= lsqy + 430 )
                    {
                            if(rgobb == 1)
                            {
                                rgobb = 2;
                                strcpy (p2gobb, "g2.gif");

                                if(lgobb == 2)
                                {
                                    rgobb = 3;
                                    strcpy (p2gobb, "g3.gif");
                                }
                            }
                            else
                                if(rgobb == 2)
                                {
                                    rgobb = 3;
                                    strcpy (p2gobb, "g3.gif");

                                    if(lgobb == 3)
                                    {
                                        rgobb = 4;
                                        strcpy (p2gobb, "g4.gif");
                                    }
                                }
                                else if(rgobb == 3)
                                    {
                                        rgobb = 4;
                                        strcpy (p2gobb, "g4.gif");

                                        if(lgobb == 4)
                                        {
                                            rgobb = 1;
                                            strcpy (p2gobb, "g1.gif");
                                        }
                                    }
                                    else if(rgobb == 4)
                                        {
                                            rgobb = 1;
                                            strcpy (p2gobb, "g1.gif");

                                            if(lgobb == 1)
                                            {
                                                rgobb = 2;
                                                strcpy (p2gobb, "g2.gif");
                                            }
                                        }
                    chooseGobblet( lgobb, rgobb, rsqx, lsqx, lsqy);
                }
        }
    }
}

void homeListenForClick()
{
    bool game = true;
    int textw = textwidth("Start Game"), texth = textheight("Start Game"),
        infow = textwidth("Tutorial"), infoh = textheight("Tutorial"),
        scorew = textwidth("Score"), scoreh = textheight("Score");;

    setmousequeuestatus(WM_LBUTTONUP, true);

    int mousex = 0, mousey = 0, newx = 0, newy = 0;
    while (game)
    {
        getmouseclick(WM_LBUTTONUP, newx, newy);
        if (newx != mousex || newy != mousey)
        {
            mousex = newx;
            mousey = newy;
            if (mousex >= (w - textw) / 2 && mousex <= (w - textw) / 2 + textw && mousey >= 400 && mousey <= 400 + texth)
            {
                game = false;
                pickUsers();
            }
            else if (mousex >= (w - infow) / 2 && mousex <= (w - infow) / 2 + infow && mousey >= 470 && mousey <= 470 + infoh)
            {
                game = false;
                infoScreen();
            }
            else if (mousex >= (w - scorew) / 2 && mousex <= (w - scorew) / 2 + scorew && mousey >= 540 && mousey <= 540 + scoreh)
            {
                game = false;
                scoreScreen();
            }
        }
    }
}

void scoreScreen()
{
    readimagefile("bg.jpg", 0, 0, 1920, 1080);
    readimagefile("close.jpg", 25, 25, 75, 75);

    settextstyle(8, HORIZ_DIR, 5);
    setcolor(WHITE);
    setbkcolor(BLACK);
    int titlew = textwidth("Score");
    outtextxy((w - titlew)/2, 50, "Score");

    cout<<endl<<g1s<<" "<<g2s<<" "<<g3s<<" "<<g4s<<endl;
    char gobb1[20], gobb2[20], gobb3[20], gobb4[20];
    itoa(g1s, gobb1, 10);
    itoa(g2s, gobb2, 10);
    itoa(g3s, gobb3, 10);
    itoa(g4s, gobb4, 10); //Genny - 1, Geffy - 2, Gerry - 3, Gabby - 4

    readimagefile("g1.gif", 90, 250, w/4 - 90 , 500);
    readimagefile("g2.gif", w/4 + 90, 250, w/2 - 90 , 500);
    readimagefile("g3.gif", w/2 + 90, 250, w/2 + w/4 - 90 , 500);
    readimagefile("g4.gif", w/2 + w/4 + 90, 250, w - 90 , 500);

    outtextxy ( w/8 - textwidth("Genny")/2, 200 ,"Genny");
    outtextxy ( w/4 + w/8 - textwidth("Geffy")/2, 200 ,"Geffy");
    outtextxy ( w/2 + w/8 - textwidth("Gerry")/2, 200 ,"Gerry");
    outtextxy ( w - w/8 - textwidth("Gabby")/2, 200 ,"Gabby");

    settextstyle(8, HORIZ_DIR, 7);
    outtextxy ( w/8 - textwidth(gobb1)/2, 590 ,gobb1);
    outtextxy ( w/4 + w/8 - textwidth(gobb2)/2, 590 ,gobb2);
    outtextxy ( w/2 + w/8 - textwidth(gobb3)/2, 590 ,gobb3);
    outtextxy ( w - w/8 - textwidth(gobb4)/2, 590 ,gobb4);

  ////////RESET SCORE Buttons - design///////
    setcolor(BLACK);
    settextstyle(8, HORIZ_DIR, 4);
    setbkcolor(LIGHTGRAY);
    outtextxy ( w/8 - textwidth("Reset")/2, 700 ,"Reset");
    outtextxy ( w/4 + w/8 - textwidth("Reset")/2, 700 ,"Reset");
    outtextxy ( w/2 + w/8 - textwidth("Reset")/2, 700 ,"Reset");
    outtextxy ( w - w/8 - textwidth("Reset")/2, 700 ,"Reset");

    //////////  EXIT button  ///////////
        bool infoScreen = true;

        setmousequeuestatus(WM_LBUTTONUP, true);

        int mousex = 0, mousey = 0, newx = 0, newy = 0;

        while (infoScreen) {
            getmouseclick(WM_LBUTTONUP, newx, newy);
            if (newx != mousex || newy != mousey) {
                mousex = newx;
                mousey = newy;
                if (mousex >= 25 && mousex <= 75 && mousey >= 25 && mousey <= 75) {
                    infoScreen = false;
                    homeScreen();
                }
                else
                {
                    setcolor(WHITE);
                    setbkcolor(BLACK);
                    settextstyle(8, HORIZ_DIR, 7);

                if ( mousex >= w/8 - textwidth("Reset")/2 && mousex <= w/8 + textwidth("Reset")/2
                        && mousey >= 700 && mousey <= 700 + textheight("Reset") )
                            {
                                g1s = 0;
                                itoa(g1s, gobb1, 10);
                                outtextxy ( w/8 - textwidth(gobb1)/2, 590 ,gobb1);
                            }
                else
                if ( mousex >= w/4 + w/8 - textwidth("Reset")/2 && mousex <= w/4 + w/8 + textwidth("Reset")/2
                        && mousey >= 700 && mousey <= 700 + textheight("Reset") )
                            {
                                g2s = 0;
                                itoa(g2s, gobb2, 10);
                                outtextxy ( w/4 + w/8 - textwidth(gobb2)/2, 590 ,gobb2);
                            }
                else
                if ( mousex >= w/2 + w/8 - textwidth("Reset")/2 && mousex <= w/2 + w/8 + textwidth("Reset")/2
                        && mousey >= 700 && mousey <= 700 + textheight("Reset") )
                            {
                                g3s = 0;
                                itoa(g3s, gobb3, 10);
                                outtextxy ( w/2 + w/8 - textwidth(gobb3)/2, 590 ,gobb3);
                            }
                else
                if ( mousex >= w - w/8 - textwidth("Reset")/2 && mousex <= w - w/8 + textwidth("Reset")/2
                        && mousey >= 700 && mousey <= 700 + textheight("Reset") )
                            {
                                g4s = 0;
                                itoa(g4s, gobb4, 10);
                                outtextxy ( w - w/8 - textwidth(gobb4)/2, 590 ,gobb4);
                            }
                }
            }
        }
}

void homeScreen()
{
    settextstyle(8, HORIZ_DIR, 5);
    int startw = textwidth("Start Game"), infow = textwidth("Tutorial"), scorew = textwidth("Score");
    readimagefile("bg.jpg", 0, 0, 1920, 1080);
    readimagefile("gobblets.jpg", (w - 600) / 2, 50, (w - 600) / 2 + 600, 350);
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy((w - startw) / 2, 400, "Start Game");
    outtextxy((w - infow) / 2, 470, "Tutorial");
    outtextxy((w - scorew) / 2, 540, "Score");
    homeListenForClick();
}

int main()
{
    bool Gameee = true;

    initwindow(w, h, "", -3, -3);
    settextstyle(8, HORIZ_DIR, 5);

    while (Gameee)
    {
        homeScreen();
        homeListenForClick();
    }
    getch();

    return 0;
}
