// !WAZNE! w.aktywny_gracz w calym programie jest liczony od 1 a nie od 0
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
using std::cout;
using std::cin;
using std::endl;
enum Kolor
{
    green,
    blue,
    red,
    violet,
    yellow,
    white,
    black,
    b_koloru
};
struct Karty
{
    Kolor kolor = green;
    int wartosc = 0;
};
struct Wymiary
{
    int kociolki = 6, ile_zielonych = 9, wartosc_zielonych = 10, ile_kart = 20, liczba_graczy = 6, eksplozja{}, liczba_kart{}, aktywny_gracz = 1;  //ile_kart = liczba kart kazdego koloru innego niz zielony , kociolki = liczba kolorow kart z wylaczeniem zielonego, eksplozja = maksymalna suma wartosci kart na stosie, liczba_kart=liczba wszystkich kart
    int wartosci_kart[100]{}; //tablica wartosci kart w kazdym kolorze oprocz zielonego
};
struct Gracze
{
    Karty* reka;
    Karty* stol;
};
struct Kociolki
{
    Karty stos[500];
};
void wczytaj_wymiary(Wymiary& w); //wczytuje wymiary gry
void stworz_talie(Karty t[], Wymiary w); // Tworzy talie na podstawie danych ze struktury Wymiary
void wypisz_talie(const Karty t[], Wymiary w); //wypisuje wartosci i kolory wszystkich kart w talii po kolei
void rozdaj(const Karty t[], Gracze g[], Wymiary w); //rozdaje karty graczom po kolei tak jak sa ulozone w talii
void wypisz_stan_gry(const Gracze g[], const Kociolki k[], Wymiary w); //wypisuje/wczytuje do pliku stan gry (liczbe kart u kazdego z graczy i w kociolkach)
void wczytaj_stan_gry(Wymiary& w, Gracze g[], Kociolki k[], Karty t[]); //wczytuje stan gry i ustawia wszystkie struktury na poprawne wartosci
void zliczaj(Wymiary w, const Gracze g[], const Kociolki k[]); //zlicza i wypisuje ile kart ma ka�dy z graczy i kociolki
int mniejszy(const void* a, const void* b); //funkcja pomocnicza do sortowania rosnacego
void posortuj_karty(Karty t[], Wymiary w); //sortuje karty kolorami i wartosciami
void zielone(Wymiary& w, const Karty t[]); //wypisuje stan zielonych kart
int ile_kart_jednego_koloru(const Karty t[], Kolor k, Wymiary w); //zwraca liczbe kart danego koloru w calej talii
int ile_kart_jednego_koloru(const Gracze g[], int nr_gracza, Kolor k); //zwraca liczbe kart jedengo koloru na stole danego gracza
void wypisz_liczbe_kart(const Karty t[], Wymiary& w); //wypisuje liczbe kart w kazdym kolorze
bool czy_tyle_samo_kart(const Karty t[], Wymiary w); //sprawdza czy w kazdym kolorze jest tyle samo kart
Kolor ustaw_kolor(char s[]); //zamienia char[] na Kolor
const char* zamien_kolor_na_napis(Kolor k); //zamienia zmienna kolor na odpowiadajacy napis char[]
Wymiary wartosci_kart(const Karty t[], Kolor k, const Wymiary& w); //zwraca tablice wartosci kart danego koloru
bool czy_takie_same_wartosci(const Karty t[], Wymiary w); //sprawdza czy wartosci kart wszystkich kolorow ss takie same
void wypisz_wartosci_kart(const Karty t[], Wymiary& w); //wypisuje wartosci kart w kazdym kolorze
void wypisz_kolor(const Karty t[], Kolor k, const Wymiary& w); //wypisuje wartosci kart w danym kolorze
int indeks_koloru(const Karty t[], Kolor k, Wymiary w); //zwraca indeks pierwszej karty danego koloru w talii
int zliczaj_karty_na_rece(const Gracze g[], int nr_gracza); //zwraca liczbe kart na rece danego gracza razem z pustymi
bool czy_kolory_w_kociolku(const Kociolki k[], int nr_kociolka); //sprawdza czy dany kociolek jest poprawny pod wzgledem koloru
int suma_wartosci_w_kociolku(const Kociolki k[], int nr_kociolka); //zwraca sume wartosci kart w danym kociolku
void stan_gry(const Gracze g[], const Kociolki k[], Wymiary w, int karty[], bool pierwszy); //sprawdza czy stan gry jest poprawny i wypisuje odpowiednie komunikaty
Kolor jaki_kolor_w_kociolku(const Kociolki k[], int nr_kociolka); //zwraca kolor jaki znajduje sie w danym kociolku lub green gdy nie ma zadnego innego
int ile_kart_w_kociolku(const Kociolki k[], int nr_kociolka); //zwraca liczbe kart w danym kociolku
void proste_posuniecie(Gracze g[], Kociolki k[], Wymiary& w); //przerzuca pierwsza karte aktywnego gracza na pierwszy mozliwy kociolek
void przerzuc_podana_karte_na_koniec(Gracze g[], int nr_gracza, int nr_karty); //przerzuca wyrzucona karte na rece danego gracza na koniec (uzywana w posunieciu)
void posuniecie(Gracze g[], int nr_karty, Kociolki k[], int nr_kociolka, Wymiary& w); //funkcja pomocnicza do posuniec przerzuca karte gracza na kociolek i obsluguje ekzplozje kociolka
int ile_kart_na_stole(const Gracze g[], int nr_gracza); //zwraca liczbe kart na stole przed danym graczem
bool odporny(const Gracze g[], int nr_gracza, Kolor k, Wymiary w); //sprawdza czy dany gracz jest odporny na dany kolor
void wypisz_wyniki(Wymiary w, const Gracze g[]); //wypisuje stan odpornosci na kolory i koncowe wyniki kazdego gracza
int wynik(const Gracze g[], int nr_gracza, const Wymiary& w); //zwraca koncowy wynik danego gracza
void potasuj_karty(Karty t[], Wymiary w); //tasuje talie
void wypisz_komunikaty_o_stanie_gry(Karty t[], Gracze g[], Kociolki k[], Wymiary& w, int karty[], bool pierwszy); //wypisuje wszystkie potrzebne komunikatu o stanie gry (zlozenie innych funkcji w  calosc)
bool koniec_rundy(const Gracze g[], Wymiary w); //Jesli true to runda sie skonczyla
void posuniecie_najmniejsza_karta(Gracze g[], Kociolki k[], Wymiary& w); //wybiera najmniejsza karte na rece i odklada na stos o najmniejszej sumie wartosci w srodku
void posuniecie_najwieksza_karta(Gracze g[], Kociolki k[], Wymiary& w);  //wybiera najwieksza karte na rece i odklada na stos o najmniejszej sumie wartosci w srodku
void posuniecie_najmniejsza_i_najwieksza(Gracze g[], Kociolki k[], Wymiary& w); //posuniecie najmniejsza a gdy jest eksplozja to najwieksza
void posuniecie_optymalna_karta(Gracze g[], Kociolki k[], Wymiary& w); //wybiera najwieksza ktora nie spowoduje eksplozji a jesli sie nie da to posuniecie_najwieksza_karta
void posortuj_malejaco_karty_na_rece(Gracze& g, int ile, Wymiary w); //sortuje malejaco karty na rece danego gracza (uzywana w posunieciu optymalna karta) ile = liczba kart do posortowania
int wiekszy(const void* a, const void* b); //funkcja pomocnicza do sortowania malejacego
int ile_kart_na_rece(const Gracze g[], int nr_gracza); //zwraca liczbe kart na rece danego gracza (bez pustych)
void posuniecie_maksymalizacja_koloru(Gracze g[], Kociolki k[], Wymiary& w); //to samo co optymalna karta ale w razie eksplozji maksymalizuje ktorys kolor



int main(/*int argc, char* argv[]*/)
{
    Wymiary w;
    wczytaj_wymiary(w);

    int* wyniki = new int[w.liczba_graczy]{};
    for (int i = 0; i < 50; i++)
    {
        Karty* talia = new Karty[w.liczba_kart];
        Gracze* gracz = new Gracze[w.liczba_graczy];
        for (int i = 0; i < w.liczba_graczy; i++) //dynamiczna alokacja tablic rak i stolow graczy
        {
            gracz[i].reka = new Karty[w.liczba_kart];
            gracz[i].stol = new Karty[w.liczba_kart];
        }
        Kociolki* kociolek = new Kociolki[w.kociolki];
        int* karty_rece = new int[w.liczba_graczy]; //pomocnicza tablica przechowujaca liczbe kart na rece kazdego z graczy zeby sprawdzic czy w danej rundzie liczba sie zgadza

        stworz_talie(talia, w);
        potasuj_karty(talia, w);
        rozdaj(talia, gracz, w);
        w.aktywny_gracz = 1;
        wypisz_stan_gry(gracz, kociolek, w);
        wczytaj_stan_gry(w, gracz, kociolek, talia);
        //      cout << endl << endl;
    //    wypisz_komunikaty_o_stanie_gry(talia, gracz, kociolek, w, karty_rece, true);
        while (koniec_rundy(gracz, w) == false)
        {
            wczytaj_stan_gry(w, gracz, kociolek, talia);
            //         cout << endl << endl;
                 //    cout << "PRZED RUCHEM:\n";
          //        wypisz_stan_gry(gracz, kociolek, w);
                  //   system("pause");
            switch (w.aktywny_gracz) //który gracz ma się ruszyć
            {
            case 1: posuniecie_maksymalizacja_koloru(gracz, kociolek, w); break;
            case 2: posuniecie_optymalna_karta(gracz, kociolek, w); break;
            case 3: posuniecie_najmniejsza_karta(gracz, kociolek, w); break;
            case 4: posuniecie_najwieksza_karta(gracz, kociolek, w); break;
            case 5: posuniecie_najmniejsza_i_najwieksza(gracz, kociolek, w); break;
            default: proste_posuniecie(gracz, kociolek, w);
            }
            //      cout << "PO RUCHU:\n";
            wypisz_stan_gry(gracz, kociolek, w);
            //        wypisz_komunikaty_o_stanie_gry(talia, gracz, kociolek, w, karty_rece, false);
                          //   system("pause");
        }
        for (int j = 0; j < w.liczba_graczy; j++)
        {
            wyniki[j] += wynik(gracz, j, w);
        }
        //      wypisz_wyniki(w, gracz);

        //zwolnienie miejsca w pamieci
        for (int j = 0; j < w.liczba_graczy; j++)
        {
            delete[] gracz[j].reka;
            delete[] gracz[j].stol;
        }
        delete[] talia;
        delete[] gracz;
        delete[] kociolek;
        delete[] karty_rece;
    }

    cout << endl;
    for (int j = 0; j < w.liczba_graczy; j++)
    {
        cout << "Wynik " << j + 1 << ": " << wyniki[j] << endl;
    }
    delete[] wyniki;

    return 0;
}



void wczytaj_wymiary(Wymiary& w)
{
    cout << "Wczytaj liczbe graczy: "; cin >> w.liczba_graczy;
    cout << "Wczytaj liczbe kociolkow: "; cin >> w.kociolki;
    cout << "Wczytaj liczbe zielonych kart: "; cin >> w.ile_zielonych;
    cout << "Wczytaj wartosc zielonych kart: "; cin >> w.wartosc_zielonych;
    cout << "Wczytaj maksymalna wartosc na kociolku: "; cin >> w.eksplozja;
    cout << "Wczytaj liczbe kart innych kolorow: "; cin >> w.ile_kart;

    int wartosc;
    for (int i = 0; i < w.ile_kart; i++)
    {
        cout << "Wczytaj wartosc " << i + 1 << " karty: ";
        cin >> wartosc;
        w.wartosci_kart[i] = wartosc;
    }

    w.liczba_kart = w.ile_zielonych + w.kociolki * w.ile_kart;
}


void stworz_talie(Karty t[], Wymiary w)
{
    //Przypisanie kolorow i wartosci zielonym kartom
    for (int i = 0; i < w.ile_zielonych; i++)
    {
        t[i].kolor = green;
        t[i].wartosc = w.wartosc_zielonych;
    }
    //Przypisanie kolorow i wartosci pozostalym kartom
    for (int i = 1; i <= w.kociolki; i++)
    {
        int start = w.ile_zielonych + (i - 1) * w.ile_kart;
        for (int j = start; j < start + w.ile_kart; j++)
        {
            switch (i)
            {
            case 1: t[j].kolor = blue; t[j].wartosc = w.wartosci_kart[j - start]; break;
            case 2: t[j].kolor = red; t[j].wartosc = w.wartosci_kart[j - start]; break;
            case 3: t[j].kolor = violet; t[j].wartosc = w.wartosci_kart[j - start]; break;
            case 4: t[j].kolor = yellow; t[j].wartosc = w.wartosci_kart[j - start]; break;
            case 5: t[j].kolor = white; t[j].wartosc = w.wartosci_kart[j - start]; break;
            case 6: t[j].kolor = black; t[j].wartosc = w.wartosci_kart[j - start]; break;
            }
        }

    }
}


void wypisz_talie(const Karty t[], Wymiary w)
{
    char k[10];
    for (int i = 0; i < w.liczba_kart; i++)
    {
        strcpy_s(k, 10, zamien_kolor_na_napis(t[i].kolor));
        cout << t[i].wartosc << " " << k << " " << endl;
    }
}


void rozdaj(const Karty t[], Gracze g[], Wymiary w)
{
    int j = 0; //numer gracza
    int nr_reka = 0; //numer karty na rece
    for (int i = 0; i < w.liczba_kart; i++)
    {
        g[j].reka[nr_reka] = t[i];
        if (j >= w.liczba_graczy - 1)
        {
            j = 0;
            nr_reka++;
        }
        else
        {
            j++;
        }
    }
}


void wypisz_stan_gry(const Gracze g[], const Kociolki k[], Wymiary w)
{
    FILE* plik;
    plik = fopen("stan_gry.txt", "w"); //otwarcie/utowrzenie pliku ze stanem gry i sprawdzenie czy sie to powiodlo
    if (plik == NULL)
    {
        cout << "Otwarcie pliku nie powiodlo sie!\n";
        exit(1);
    }
    fprintf(plik, "active player = %d\n", w.aktywny_gracz);
    //   cout << "active player = " << w.aktywny_gracz << "\n";
    fprintf(plik, "players number = %d\n", w.liczba_graczy);
    //   cout << "players number = " << w.liczba_graczy << "\n";
    fprintf(plik, "explosion threshold = %d\n", w.eksplozja);
    //   cout << "explosion threshold = " << w.eksplozja << "\n";
    for (int i = 0; i < w.liczba_graczy; i++)
    {
        fprintf(plik, "%d player hand cards: ", i + 1);
        //      cout << i + 1 << " player hand cards: ";
        int j = 0;
        while (g[i].reka[j].wartosc != 0)  //pokaz wszystkie karty na rece danego gracza
        {
            fprintf(plik, "%d %s ", g[i].reka[j].wartosc, zamien_kolor_na_napis(g[i].reka[j].kolor));
            //          cout << g[i].reka[j].wartosc << " " << zamien_kolor_na_napis(g[i].reka[j].kolor) << " ";
            j++;
        }
        fprintf(plik, "\n%d player deck cards: ", i + 1);
        //      cout << endl;
        //      cout << i + 1 << " player deck cards: ";
        j = 0;
        while (g[i].stol[j].wartosc != 0)  //pokaz wszystkie karty na stole danego gracza
        {
            fprintf(plik, "%d %s ", g[i].stol[j].wartosc, zamien_kolor_na_napis(g[i].stol[j].kolor));
            //         cout << g[i].stol[j].wartosc << " " << zamien_kolor_na_napis(g[i].stol[j].kolor) << " ";
            j++;
        }
        fprintf(plik, "\n");
        //      cout << endl;
    }

    for (int i = 0; i < w.kociolki; i++)
    {
        fprintf(plik, "%d pile cards: ", i + 1);
        //      cout << i + 1 << " pile cards: ";
        int j = 0;
        while (k[i].stos[j].wartosc != 0)  //pokaz wszystkie karty w kociolku
        {
            fprintf(plik, "%d %s ", k[i].stos[j].wartosc, zamien_kolor_na_napis(k[i].stos[j].kolor));
            //         cout << k[i].stos[j].wartosc << " " << zamien_kolor_na_napis(k[i].stos[j].kolor) << " ";
            j++;
        }
        fprintf(plik, "\n");
        //      cout << endl;
    }
    fclose(plik);
}


void wczytaj_stan_gry(Wymiary& w, Gracze g[], Kociolki k[], Karty t[])
{
    FILE* plik;
    plik = fopen("stan_gry.txt", "r"); //otwarcie/utowrzenie pliku ze stanem gry i sprawdzenie czy sie to powiodlo
    if (plik == NULL)
    {
        cout << "Otwarcie pliku nie powiodlo sie!\n";
        exit(1);
    }
    char s[10];
    int l{};
    fscanf(plik, "%s %s %s", s, s, s);
    // cin >> s >> s >> s;
    fscanf(plik, "%d", &(w.aktywny_gracz));
    //cin >> w.aktywny_gracz;
    fscanf(plik, "%s %s %s", s, s, s);
    //cin >> s >> s >> s;
    fscanf(plik, "%d", &(w.liczba_graczy));
    //cin >> w.liczba_graczy;
    fscanf(plik, "%s %s %s", s, s, s);
    //cin >> s >> s >> s;
    fscanf(plik, "%d", &(w.eksplozja));
    // cin >> w.eksplozja;
    fscanf(plik, "%s %s", s, s);
    //cin >> s >> s;
    int nr_karty = 0; //nr_karty w talii

    for (int i = 0; i < w.liczba_graczy; i++)
    {
        fscanf(plik, "%s %s", s, s);
        //cin >> s >> s;
        int j = 0; //numer karty
        //while (cin >> l >> s)  //wczytuje karty na rece i-tego gracza
        while (fscanf(plik, "%d %s", &l, s))
        {
            if (!strcmp(s, "player")) break;
            else
            {
                g[i].reka[j].wartosc = l;
                g[i].reka[j].kolor = ustaw_kolor(s);
                j++;
                t[nr_karty].wartosc = l;
                t[nr_karty].kolor = ustaw_kolor(s);
                nr_karty++;
            }
        }
        fscanf(plik, "%s %s", s, s);
        //cin >> s >> s;
        j = 0;
        // while (cin >> l >> s) //wczytuje karty na stole i-tego gracza
        while (fscanf(plik, "%d %s", &l, s))
        {
            if (!strcmp(s, "player") || !strcmp(s, "pile")) break;
            else
            {

                g[i].stol[j].wartosc = l;
                g[i].stol[j].kolor = ustaw_kolor(s);
                j++;
                t[nr_karty].wartosc = l;
                t[nr_karty].kolor = ustaw_kolor(s);
                nr_karty++;
            }
        }
    }

    int i = 0;
    //while (cin >> s) //wczytuje karty w kociolkach
    while (fscanf(plik, "%s", s) != EOF)
    {
        //cout << s << endl;
        int j = 0;
        //while (cin >> l >> s)
        while (fscanf(plik, "%d %s", &l, s) != EOF)
        {
            if (!strcmp(s, "pile")) break;
            else
            {
                k[i].stos[j].wartosc = l;
                k[i].stos[j].kolor = ustaw_kolor(s);
                j++;
                t[nr_karty].wartosc = l;
                t[nr_karty].kolor = ustaw_kolor(s);
                nr_karty++;
            }
        }
        i++;
    }
    w.kociolki = i;
    w.liczba_kart = nr_karty;
    fclose(plik);
}


int zliczaj_karty_na_rece(const Gracze g[], int nr_gracza)
{
    int i = 0;
    while ((g[nr_gracza].reka[i].wartosc != 0) || g[nr_gracza].reka[i].kolor == b_koloru)
    {
        i++;
    }

    return i;
}

void zliczaj(Wymiary w, const Gracze g[], const Kociolki k[])
{
    for (int i = 0; i < w.liczba_graczy; i++)
    {
        int j = 0;
        while (g[i].reka[j].wartosc != 0)
        {
            j++;
        }
        cout << i + 1 << " player has " << j << " cards on hand" << endl;

        j = 0;
        while (g[i].stol[j].wartosc != 0)
        {
            j++;
        }
        cout << i + 1 << " player has " << j << " cards in front of him" << endl;
    }

    for (int i = 0; i < w.kociolki; i++)
    {
        int j = 0;
        while (k[i].stos[j].wartosc != 0)
        {
            j++;
        }
        cout << "there are " << j << " cards on " << i + 1 << " pile" << endl;
    }
}


void posortuj_karty(Karty t[], Wymiary w)
{
    qsort(t, w.liczba_kart, sizeof(Karty), mniejszy);
}


int mniejszy(const void* a, const void* b)
{
    Karty* karta_a = (Karty*)a;
    Karty* karta_b = (Karty*)b;
    if (karta_a->kolor == karta_b->kolor)
    {
        if (karta_a->wartosc < karta_b->wartosc) return -1;
        return 1;
    }
    else
    {
        if (karta_a->kolor < karta_b->kolor) return -1;
        return 1;
    }
}


void zielone(Wymiary& w, const Karty t[])
{
    bool czy_zielone = true;
    int licznik_zielonych = 0, wartosc_zielonych = 0, poprzedni_zielony = 0;
    while (t[licznik_zielonych].kolor == green)
    {
        if (poprzedni_zielony == 0)
        {
            poprzedni_zielony = t[licznik_zielonych].wartosc;
            wartosc_zielonych = t[licznik_zielonych].wartosc;
        }
        else
        {
            if (t[licznik_zielonych].wartosc != poprzedni_zielony)
            {
                czy_zielone = false;
            }
            int pom = wartosc_zielonych;
            wartosc_zielonych = t[licznik_zielonych].wartosc;
            poprzedni_zielony = pom;
        }
        licznik_zielonych++;
    }
    w.ile_zielonych = licznik_zielonych;
    w.wartosc_zielonych = wartosc_zielonych;

    if (czy_zielone)
    {
        if (w.ile_zielonych > 0)
        {
            cout << "Found " << w.ile_zielonych << " green cards, all with " << w.wartosc_zielonych << " value" << endl;
        }
        else
        {
            cout << "Green cards does not exist" << endl;
        }
    }
    else
    {
        cout << "Different green cards values occurred" << endl;
    }
}


int ile_kart_jednego_koloru(const Karty t[], Kolor k, Wymiary w)
{
    int licznik = 0;
    for (int i = 0; i < w.liczba_kart; i++)
    {
        if (t[i].kolor == k)
        {
            licznik++;
        }
    }
    return licznik;
}


void wypisz_liczbe_kart(const Karty t[], Wymiary& w)
{
    if (czy_tyle_samo_kart(t, w))
    {
        w.ile_kart = ile_kart_jednego_koloru(t, blue, w);
        cout << "The number cards of all colors is equal: " << w.ile_kart << endl;
    }
    else
    {
        cout << "At least two colors with a different number of cards were found:" << endl;
        if (ile_kart_jednego_koloru(t, blue, w))
        {
            cout << "blue cards are " << ile_kart_jednego_koloru(t, blue, w) << endl;
        }
        if (ile_kart_jednego_koloru(t, red, w))
        {
            cout << "red cards are " << ile_kart_jednego_koloru(t, red, w) << endl;
        }
        if (ile_kart_jednego_koloru(t, violet, w))
        {
            cout << "violet cards are " << ile_kart_jednego_koloru(t, violet, w) << endl;
        }
        if (ile_kart_jednego_koloru(t, yellow, w))
        {
            cout << "yellow cards are " << ile_kart_jednego_koloru(t, yellow, w) << endl;
        }
        if (ile_kart_jednego_koloru(t, white, w))
        {
            cout << "white cards are " << ile_kart_jednego_koloru(t, white, w) << endl;
        }
        if (ile_kart_jednego_koloru(t, black, w))
        {
            cout << "black cards are " << ile_kart_jednego_koloru(t, black, w) << endl;
        }
    }
}


bool czy_tyle_samo_kart(const Karty t[], Wymiary w)
{
    int kolor[6];
    kolor[0] = ile_kart_jednego_koloru(t, blue, w);
    kolor[1] = ile_kart_jednego_koloru(t, red, w);
    kolor[2] = ile_kart_jednego_koloru(t, violet, w);
    kolor[3] = ile_kart_jednego_koloru(t, yellow, w);
    kolor[4] = ile_kart_jednego_koloru(t, white, w);
    kolor[5] = ile_kart_jednego_koloru(t, black, w);
    for (int i = 1; i < w.kociolki; i++)
    {
        if (kolor[i] != kolor[i - 1]) return false;
    }
    return true;
}


Kolor ustaw_kolor(char s[])
{
    if (!strcmp(s, "green"))  return green;
    else if (!strcmp(s, "blue")) return blue;
    else if (!strcmp(s, "red")) return red;
    else if (!strcmp(s, "violet")) return violet;
    else if (!strcmp(s, "yellow")) return yellow;
    else if (!strcmp(s, "white")) return white;
    else if (!strcmp(s, "black")) return black;

    else return b_koloru;
}


const char* zamien_kolor_na_napis(Kolor k)
{
    switch (k)
    {
    case green: return "green";
    case blue: return "blue";
    case red: return "red";
    case violet: return "violet";
    case yellow: return "yellow";
    case white: return "white";
    case black: return "black";
    default: return "b_koloru";
    }
}


Wymiary wartosci_kart(const Karty t[], Kolor k, const Wymiary& w)
{
    int ile_kart = ile_kart_jednego_koloru(t, k, w);
    int start = indeks_koloru(t, k, w); //od ktorej karty w talii zaczac w zaleznosci od koloru
    int j = 0;
    Wymiary wynik;
    for (int i = start; i < start + ile_kart; i++)
    {
        wynik.wartosci_kart[j] = t[i].wartosc;
        j++;
    }
    return wynik;
}


bool czy_takie_same_wartosci(const Karty t[], Wymiary w)
{
    if (czy_tyle_samo_kart(t, w) == false)
    {
        return false; //liczba kart sie nie zgadza
    }

    Wymiary kolor1, kolor2;

    for (int i = 1; i < w.kociolki; i++)
    {
        switch (i)
        {
        case 1: kolor1 = wartosci_kart(t, blue, w); kolor2 = wartosci_kart(t, red, w); break;
        case 2: kolor1 = wartosci_kart(t, red, w); kolor2 = wartosci_kart(t, violet, w); break;
        case 3: kolor1 = wartosci_kart(t, violet, w); kolor2 = wartosci_kart(t, yellow, w); break;
        case 4: kolor1 = wartosci_kart(t, yellow, w); kolor2 = wartosci_kart(t, white, w); break;
        case 5: kolor1 = wartosci_kart(t, white, w); kolor2 = wartosci_kart(t, black, w); break;
        }
        for (int j = 0; j < w.ile_kart; j++)
        {
            if (kolor1.wartosci_kart[j] != kolor2.wartosci_kart[j])
            {
                return false;
            }
        }
    }
    return true;
}


void wypisz_wartosci_kart(const Karty t[], Wymiary& w)
{
    if (czy_takie_same_wartosci(t, w))
    {
        w.ile_kart = ile_kart_jednego_koloru(t, blue, w);
        Wymiary k = wartosci_kart(t, blue, w);
        for (int i = 0; i < w.ile_kart; i++)
        {
            w.wartosci_kart[i] = k.wartosci_kart[i];
        }
        cout << "The values of cards of all colors are identical:\n";
        for (int i = 0; i < w.ile_kart; i++)
        {
            cout << w.wartosci_kart[i] << " ";
        }
    }
    else
    {
        cout << "The values of cards of all colors are not identical:\n";
        for (int i = 1; i <= w.kociolki; i++)
        {
            wypisz_kolor(t, Kolor(i), w);
        }
    }
    cout << endl;
}


void wypisz_kolor(const Karty t[], Kolor k, const Wymiary& w)
{
    Wymiary pom = wartosci_kart(t, k, w);
    int ile_kart = ile_kart_jednego_koloru(t, k, w);
    cout << zamien_kolor_na_napis(k) << " cards values: ";
    for (int i = 0; i < ile_kart; i++)
    {
        cout << pom.wartosci_kart[i] << " ";
    }
    cout << endl;
}


int indeks_koloru(const Karty t[], Kolor k, Wymiary w)
{
    for (int i = 0; i < w.liczba_kart; i++)
    {
        if (t[i].kolor == k) return i;
    }
    return 0;
}

bool czy_kolory_w_kociolku(const Kociolki k[], int nr_kociolka)
{
    int i = 0;
    int licznik = 0;
    Kolor kolor_kociolka = green;
    while (k[nr_kociolka].stos[i].wartosc != 0)
    {
        if (k[nr_kociolka].stos[i].kolor != kolor_kociolka && k[nr_kociolka].stos[i].kolor != green)
        {
            kolor_kociolka = k[nr_kociolka].stos[i].kolor;
            licznik++;
            if (licznik >= 2)
            {
                return false;
            }
        }
        i++;
    }
    return true;
}


int suma_wartosci_w_kociolku(const Kociolki k[], int nr_kociolka)
{
    int i = 0, suma = 0;
    while (k[nr_kociolka].stos[i].wartosc != 0)
    {
        suma += k[nr_kociolka].stos[i].wartosc;
        i++;
    }

    return suma;
}


void stan_gry(const Gracze g[], const Kociolki k[], Wymiary w, int karty[], bool pierwszy)
{
    bool poprawny_stan = true;
    if (pierwszy == true) //pierwsze sprawdzenie na poczatku rundy przed wszystkimi ruchami
    {
        for (int i = 0; i < w.liczba_graczy; i++)
        {
            karty[i] = ile_kart_na_rece(g, i);
        }
        int maks = karty[0], min = karty[0];
        for (int i = 1; i < w.liczba_graczy; i++)
        {
            if (karty[i] > maks)
            {
                maks = karty[i];
            }
            if (karty[i] < min)
            {
                min = karty[i];
            }
            if (karty[i] != karty[i - 1] && karty[i] != karty[i - 1] - 1) //jezli nastepna karta jest inna niz poprzednia i inna niz mniejsza o 1 od poprzedniej
            {
                poprawny_stan = false;
            }
        }
        if (maks - min >= 2) //jesli ktos ma o conajmniej 2 karty wiecej od innego
        {
            poprawny_stan = false;
        }
    }

    else //jesli sprawdzenie jest w srodku rundy a nie na poczatku
    {
        int poprzedni_aktywny; //indeks gracza aktywnego w poprzedniej rundzie
        if (w.aktywny_gracz == 1)
        {
            poprzedni_aktywny = w.liczba_graczy - 1;
        }
        else
        {
            poprzedni_aktywny = w.aktywny_gracz - 2;
        }
        for (int i = 0; i < w.liczba_graczy; i++)
        {
            if (i == poprzedni_aktywny)
            {
                if (ile_kart_na_rece(g, i) != karty[i] - 1)
                {
                    poprawny_stan = false;
                }
            }
            else
            {
                if (ile_kart_na_rece(g, i) != karty[i])
                {
                    poprawny_stan = false;
                }
            }

            karty[i] = ile_kart_na_rece(g, i); //wartosc nadpisywana zeby mozna bylo jej uzyc w nastepnym wywolaniu
        }
    }

    if (poprawny_stan == false)
    {
        cout << "The number of players cards on hand is wrong\n";
    }

    for (int i = 0; i < w.kociolki; i++)
    {
        if (czy_kolory_w_kociolku(k, i) == false) //czy kolory w kociolkach sie zgadzaja
        {
            cout << "Two different colors were found on the " << i + 1 << " pile\n";
            poprawny_stan = false;
        }
    }

    for (int i = 0; i < w.kociolki; i++) //czy suma wartosci kart w kociolkach nie jest za duza
    {
        if (suma_wartosci_w_kociolku(k, i) > w.eksplozja)
        {
            cout << "Pile number " << i + 1 << " should explode earlier\n";
            poprawny_stan = false;
        }
    }

    if (poprawny_stan == true)
    {
        cout << "Current state of the game is ok\n";
    }
}


int ile_kart_w_kociolku(const Kociolki k[], int nr_kociolka)
{
    int i = 0;
    while (k[nr_kociolka].stos[i].wartosc != 0)
    {
        i++;
    }
    return i;
}

Kolor jaki_kolor_w_kociolku(const Kociolki k[], int nr_kociolka)
{
    for (int i = 0; i < ile_kart_w_kociolku(k, nr_kociolka); i++)
    {
        if ((k[nr_kociolka].stos[i].kolor != green))
        {
            return k[nr_kociolka].stos[i].kolor;
        }
    }
    return green;
}


void proste_posuniecie(Gracze g[], Kociolki k[], Wymiary& w)
{
    bool zrobiony = false; // jesli true to posuniecie juz bylo zrobione
    Karty karta = g[w.aktywny_gracz - 1].reka[0];

    if (karta.kolor == green)
    {
        posuniecie(g, 0, k, 0, w);
        zrobiony = true;
    }
    else
    {
        for (int i = 0; i < w.kociolki; i++) //czy w ktoryms kociolku juz jest dany kolor
        {
            if (karta.kolor == jaki_kolor_w_kociolku(k, i))
            {
                posuniecie(g, 0, k, i, w);
                zrobiony = true;
                break;
            }
        }

        if (zrobiony == false)
        {
            for (int i = 0; i < w.kociolki; i++)
            {
                if (jaki_kolor_w_kociolku(k, i) == green)
                {
                    posuniecie(g, 0, k, i, w);
                    zrobiony = true;
                    break;
                }
            }
        }
    }

}


void przerzuc_podana_karte_na_koniec(Gracze g[], int nr_gracza, int nr_karty)
{
    int ile = zliczaj_karty_na_rece(g, nr_gracza);
    Karty pom;
    for (int i = nr_karty; i < ile - 1; i++)
    {
        pom = g[nr_gracza].reka[i];
        g[nr_gracza].reka[i] = g[nr_gracza].reka[i + 1];
        g[nr_gracza].reka[i + 1] = pom;
    }
}


void posuniecie(Gracze g[], int nr_karty, Kociolki k[], int nr_kociolka, Wymiary& w)
{
    Karty karta = g[w.aktywny_gracz - 1].reka[nr_karty];
    k[nr_kociolka].stos[ile_kart_w_kociolku(k, nr_kociolka)] = karta;
    g[w.aktywny_gracz - 1].reka[nr_karty].wartosc = 0;
    g[w.aktywny_gracz - 1].reka[nr_karty].kolor = b_koloru;
    przerzuc_podana_karte_na_koniec(g, w.aktywny_gracz - 1, nr_karty);
    if (suma_wartosci_w_kociolku(k, nr_kociolka) > w.eksplozja) //obsluga eksplozji
    {
        int ile = ile_kart_w_kociolku(k, nr_kociolka);
        for (int i = 0; i < ile; i++)
        {
            g[w.aktywny_gracz - 1].stol[ile_kart_na_stole(g, w.aktywny_gracz - 1)] = k[nr_kociolka].stos[i];
            k[nr_kociolka].stos[i].wartosc = 0;
            k[nr_kociolka].stos[i].kolor = b_koloru;
        }
    }

    if (w.aktywny_gracz < w.liczba_graczy)
    {
        w.aktywny_gracz++;
    }
    else
    {
        w.aktywny_gracz = 1;
    }
}


int ile_kart_na_stole(const Gracze g[], int nr_gracza)
{
    int i = 0;
    while ((g[nr_gracza].stol[i].wartosc != 0))
    {
        i++;
    }

    return i;
}


int ile_kart_jednego_koloru(const Gracze g[], int nr_gracza, Kolor k)
{
    int licznik = 0;
    for (int i = 0; i < ile_kart_na_stole(g, nr_gracza); i++)
    {
        if (g[nr_gracza].stol[i].kolor == k)
        {
            licznik++;
        }
    }
    return licznik;
}


bool odporny(const Gracze g[], int nr_gracza, Kolor k, Wymiary w)
{
    if (k == green) return false;
    int ile = ile_kart_jednego_koloru(g, nr_gracza, k);
    for (int i = 0; i < w.liczba_graczy; i++)
    {
        if (ile_kart_jednego_koloru(g, i, k) >= ile && i != nr_gracza)
        {
            return false;
        }
    }
    return true;
}


void wypisz_wyniki(Wymiary w, const Gracze g[])
{
    for (int i = 1; i <= w.kociolki; i++) //wypisuje stan odpornosci graczy na kolory
    {
        for (int j = 0; j < w.liczba_graczy; j++)
        {
            if (odporny(g, j, Kolor(i), w))
            {
                cout << "Na kolor " << zamien_kolor_na_napis(Kolor(i)) << " odporny jest gracz " << j + 1 << endl;
            }
        }
    }

    for (int i = 0; i < w.liczba_graczy; i++)
    {
        cout << "Wynik gracza " << i + 1 << " = " << wynik(g, i, w) << endl;
    }
}


int wynik(const Gracze g[], int nr_gracza, const Wymiary& w)
{
    int suma = 0;
    int ile = ile_kart_na_stole(g, nr_gracza);
    Karty karta;
    for (int i = 0; i < ile; i++)
    {
        karta = g[nr_gracza].stol[i];
        if (odporny(g, nr_gracza, karta.kolor, w))
        {
            continue;
        }
        else if (karta.kolor == green)
        {
            suma += 2;
        }
        else
        {
            suma++;
        }
    }
    return suma;
}


void potasuj_karty(Karty t[], Wymiary w)
{
    srand(time(NULL)); //uruchamia gnerator liczb pseudolosowych
    const int ile = 40; //tyle razy funckja zamieni ze soba miejscami losowe karty w talii
    int a, b; //indeksy kart
    Karty pom;
    for (int i = 0; i < ile; i++)
    {
        a = rand() % w.liczba_kart;
        b = rand() % w.liczba_kart;
        pom = t[a];
        t[a] = t[b];
        t[b] = pom;
    }
}


void wypisz_komunikaty_o_stanie_gry(Karty t[], Gracze g[], Kociolki k[], Wymiary& w, int karty[], bool pierwszy)
{
    posortuj_karty(t, w);
    zliczaj(w, g, k);
    zielone(w, t);
    wypisz_liczbe_kart(t, w);
    wypisz_wartosci_kart(t, w);
    stan_gry(g, k, w, karty, pierwszy);
    cout << endl;
}


bool koniec_rundy(const Gracze g[], Wymiary w)
{
    for (int i = 0; i < w.liczba_graczy; i++)
    {
        if (g[i].reka[0].wartosc != 0)
        {
            return false;
        }
    }

    return true;
}


void posuniecie_najmniejsza_karta(Gracze g[], Kociolki k[], Wymiary& w)
{
    int najm = g[w.aktywny_gracz - 1].reka[0].wartosc;
    int i = 1;
    while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0) //sprawdza jaka wartosc na rece gracza jest najmniejsza
    {
        if ((g[w.aktywny_gracz - 1].reka[i].wartosc) < najm)
        {
            najm = g[w.aktywny_gracz - 1].reka[i].wartosc;
        }
        i++;
    }

    int najmniejsza_suma_w_kociolku = w.eksplozja + 1, indeks_karty = 0, indeks_kociolka = 0;
    i = 0;
    while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0) //dla wszystkich kart o wartosci rownej wartosci najmniejszej sprawdza kociolki
    {
        if (g[w.aktywny_gracz - 1].reka[i].wartosc == najm)
        {
            bool czy_jest_kolor = false; //jesli true to na ktoryms kociolku jest juz dany kolor
            for (int j = 0; j < w.kociolki; j++)
            {
                if (g[w.aktywny_gracz - 1].reka[i].kolor == green)
                {
                    if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                    {
                        najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                        indeks_karty = i;
                        indeks_kociolka = j;
                    }
                }
                else
                {
                    if (jaki_kolor_w_kociolku(k, j) == g[w.aktywny_gracz - 1].reka[i].kolor)
                    {
                        czy_jest_kolor = true;
                        if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                        {
                            najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                            indeks_karty = i;
                            indeks_kociolka = j;
                        }
                    }
                }
            }

            if (czy_jest_kolor == false)
            {
                for (int j = 0; j < w.kociolki; j++)
                {
                    if (jaki_kolor_w_kociolku(k, j) == green)
                    {
                        if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                        {
                            najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                            indeks_karty = i;
                            indeks_kociolka = j;
                        }
                    }

                }
            }
        }

        i++;
    }

    posuniecie(g, indeks_karty, k, indeks_kociolka, w);


}


void posuniecie_najwieksza_karta(Gracze g[], Kociolki k[], Wymiary& w)
{
    int najw = g[w.aktywny_gracz - 1].reka[0].wartosc;
    int i = 1;
    while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0) //sprawdza jaka wartosc na ręce gracza jest największa
    {
        if ((g[w.aktywny_gracz - 1].reka[i].wartosc) > najw)
        {
            najw = g[w.aktywny_gracz - 1].reka[i].wartosc;
        }
        i++;
    }

    int najmniejsza_suma_w_kociolku = w.eksplozja + 1, indeks_karty = 0, indeks_kociolka = 0;
    i = 0;
    while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0) //dla wszystkich kart o wartości równej wartosci największej sprawdza kociolki
    {
        if (g[w.aktywny_gracz - 1].reka[i].wartosc == najw)
        {
            bool czy_jest_kolor = false; //jesli true to na którymś kociolku jest juz dany kolor
            for (int j = 0; j < w.kociolki; j++)
            {
                if (g[w.aktywny_gracz - 1].reka[i].kolor == green)
                {
                    if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                    {
                        najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                        indeks_karty = i;
                        indeks_kociolka = j;
                    }
                }
                else
                {
                    if (jaki_kolor_w_kociolku(k, j) == g[w.aktywny_gracz - 1].reka[i].kolor)
                    {
                        czy_jest_kolor = true;
                        if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                        {
                            najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                            indeks_karty = i;
                            indeks_kociolka = j;
                        }
                    }
                }
            }

            if (czy_jest_kolor == false)
            {
                for (int j = 0; j < w.kociolki; j++)
                {
                    if (jaki_kolor_w_kociolku(k, j) == green)
                    {
                        if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                        {
                            najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                            indeks_karty = i;
                            indeks_kociolka = j;
                        }
                    }

                }
            }
        }

        i++;
    }

    posuniecie(g, indeks_karty, k, indeks_kociolka, w);
}


void posuniecie_najmniejsza_i_najwieksza(Gracze g[], Kociolki k[], Wymiary& w)
{
    int najm = g[w.aktywny_gracz - 1].reka[0].wartosc;
    int i = 1;
    while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0) //sprawdza jaka wartosc na rece gracza jest najmniejsza
    {
        if ((g[w.aktywny_gracz - 1].reka[i].wartosc) < najm)
        {
            najm = g[w.aktywny_gracz - 1].reka[i].wartosc;
        }
        i++;
    }

    int najmniejsza_suma_w_kociolku = w.eksplozja + 1, indeks_karty = 0, indeks_kociolka = 0;
    i = 0;
    while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0) //dla wszystkich kart o wartosci rownej wartosci najmniejszej sprawdza kociolki
    {
        if (g[w.aktywny_gracz - 1].reka[i].wartosc == najm)
        {
            bool czy_jest_kolor = false; //jesli true to na ktoryms kociolku jest juz dany kolor
            for (int j = 0; j < w.kociolki; j++)
            {
                if (g[w.aktywny_gracz - 1].reka[i].kolor == green)
                {
                    if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                    {
                        najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                        indeks_karty = i;
                        indeks_kociolka = j;
                    }
                }
                else
                {
                    if (jaki_kolor_w_kociolku(k, j) == g[w.aktywny_gracz - 1].reka[i].kolor)
                    {
                        czy_jest_kolor = true;
                        if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                        {
                            najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                            indeks_karty = i;
                            indeks_kociolka = j;
                        }
                    }
                }
            }

            if (czy_jest_kolor == false)
            {
                for (int j = 0; j < w.kociolki; j++)
                {
                    if (jaki_kolor_w_kociolku(k, j) == green)
                    {
                        if (suma_wartosci_w_kociolku(k, j) < najmniejsza_suma_w_kociolku)
                        {
                            najmniejsza_suma_w_kociolku = suma_wartosci_w_kociolku(k, j);
                            indeks_karty = i;
                            indeks_kociolka = j;
                        }
                    }

                }
            }
        }

        i++;
    }
    if (suma_wartosci_w_kociolku(k, indeks_kociolka) + g[w.aktywny_gracz - 1].reka[indeks_karty].wartosc > w.eksplozja)
    {
        posuniecie_najwieksza_karta(g, k, w);
    }
    else
    {
        posuniecie(g, indeks_karty, k, indeks_kociolka, w);
    }
}


void posuniecie_optymalna_karta(Gracze g[], Kociolki k[], Wymiary& w)
{
    Gracze pomgracz;
    int ile = zliczaj_karty_na_rece(g, w.aktywny_gracz - 1);
    pomgracz.reka = new Karty[ile];
    for (int i = 0; i < ile; i++)
    {
        pomgracz.reka[i] = g[w.aktywny_gracz - 1].reka[i];
    }
    posortuj_malejaco_karty_na_rece(pomgracz, ile, w);
    bool czy_zrobiony = false; //flaga mówiąca czy znalazła się juz taka karta która nie powoduje eksplozji
    Karty karta; //karta do posunięcia
    int nr_kociolka = 0;
    int i = 0;
    bool jest_kolor = false; //flagag mówiąca czy w którymkolwiek z kociołków jest już kolor danej karty
    while (pomgracz.reka[i].wartosc != 0 && czy_zrobiony == false)
    {
        jest_kolor = false;
        for (int j = 0; j < w.kociolki; j++)
        {
            if (jaki_kolor_w_kociolku(k, j) == pomgracz.reka[i].kolor)
            {
                jest_kolor = true;
            }
        }
        if (jest_kolor == true) //w którymś kociolku jest dany kolor
        {
            for (int j = 0; j < w.kociolki; j++)
            {
                if ((suma_wartosci_w_kociolku(k, j) + pomgracz.reka[i].wartosc <= w.eksplozja) && ((pomgracz.reka[i].kolor == jaki_kolor_w_kociolku(k, j)) || (pomgracz.reka[i].kolor == green)))
                {
                    czy_zrobiony = true;
                    karta = pomgracz.reka[i];
                    nr_kociolka = j;
                    break;
                }
            }
        }
        else if (jest_kolor == false) //w żadnym kociolku nie ma danego koloru
        {
            for (int j = 0; j < w.kociolki; j++)
            {
                if ((suma_wartosci_w_kociolku(k, j) + pomgracz.reka[i].wartosc <= w.eksplozja) && jaki_kolor_w_kociolku(k, j) == green) //kociolek jest pusty lub ma tylko zielone karty
                {
                    czy_zrobiony = true;
                    karta = pomgracz.reka[i];
                    nr_kociolka = j;
                    break;
                }
            }
        }
        i++;
    }
    delete[] pomgracz.reka;
    if (czy_zrobiony == true) //znalazła się karta która nie powoduje eksplozji więc teraz sprawdzam która to i robię posunięcie
    {
        i = 0;
        while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0)
        {
            if ((g[w.aktywny_gracz - 1].reka[i].wartosc == karta.wartosc) && (g[w.aktywny_gracz - 1].reka[i].kolor == karta.kolor))
            {
                posuniecie(g, i, k, nr_kociolka, w);
                break;
            }
            i++;
        }
    }
    else
    {
        posuniecie_najwieksza_karta(g, k, w);
    }
}


void posortuj_malejaco_karty_na_rece(Gracze& g, int ile, Wymiary w)
{
    qsort(g.reka, ile, sizeof(g.reka[0]), wiekszy);
}


int wiekszy(const void* a, const void* b)
{
    Karty* karta_a = (Karty*)a;
    Karty* karta_b = (Karty*)b;
    if (karta_a->wartosc == karta_b->wartosc)
    {
        if (karta_a->kolor > karta_b->kolor) return -1;
        return 1;
    }
    else
    {
        if (karta_a->wartosc > karta_b->wartosc) return -1;
        return 1;
    }
}


int ile_kart_na_rece(const Gracze g[], int nr_gracza)
{
    int i = 0;
    while ((g[nr_gracza].reka[i].wartosc != 0))
    {
        i++;
    }

    return i;
}


void posuniecie_maksymalizacja_koloru(Gracze g[], Kociolki k[], Wymiary& w)
{
    Gracze pomgracz;
    int ile1 = zliczaj_karty_na_rece(g, w.aktywny_gracz - 1);
    int ile2 = ile_kart_na_stole(g, w.aktywny_gracz - 1);
    pomgracz.reka = new Karty[ile1];
    pomgracz.stol = new Karty[ile2];
    for (int i = 0; i < ile1; i++)
    {
        pomgracz.reka[i] = g[w.aktywny_gracz - 1].reka[i];
    }
    for (int i = 0; i < ile2; i++)
    {
        pomgracz.stol[i] = g[w.aktywny_gracz - 1].stol[i];
    }
    posortuj_malejaco_karty_na_rece(pomgracz, ile1, w);
    bool czy_zrobiony = false; //flaga mówiąca czy znalazła się juz taka karta która nie powoduje eksplozji
    Karty karta; //karta do posunięcia
    int nr_kociolka = 0;
    int i = 0;
    bool jest_kolor = false; //flagag mówiąca czy w którymkolwiek z kociołków jest już kolor danej karty
    while (pomgracz.reka[i].wartosc != 0 && czy_zrobiony == false)
    {
        jest_kolor = false;
        for (int j = 0; j < w.kociolki; j++)
        {
            if (jaki_kolor_w_kociolku(k, j) == pomgracz.reka[i].kolor)
            {
                jest_kolor = true;
            }
        }
        if (jest_kolor == true) //w którymś kociolku jest dany kolor
        {
            for (int j = 0; j < w.kociolki; j++)
            {
                if ((suma_wartosci_w_kociolku(k, j) + pomgracz.reka[i].wartosc <= w.eksplozja) && ((pomgracz.reka[i].kolor == jaki_kolor_w_kociolku(k, j)) || (pomgracz.reka[i].kolor == green)))
                {
                    czy_zrobiony = true;
                    karta = pomgracz.reka[i];
                    nr_kociolka = j;
                    break;
                }
            }
        }
        else if (jest_kolor == false) //w żadnym kociolku nie ma danego koloru
        {
            for (int j = 0; j < w.kociolki; j++)
            {
                if ((suma_wartosci_w_kociolku(k, j) + pomgracz.reka[i].wartosc <= w.eksplozja) && jaki_kolor_w_kociolku(k, j) == green) //kociolek jest pusty lub ma tylko zielone karty
                {
                    czy_zrobiony = true;
                    karta = pomgracz.reka[i];
                    nr_kociolka = j;
                    break;
                }
            }
        }
        i++;
    }
    if (czy_zrobiony == true) //znalazła się karta która nie powoduje eksplozji więc teraz sprawdzam która to i robię posunięcie
    {
        i = 0;
        while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0)
        {
            if ((g[w.aktywny_gracz - 1].reka[i].wartosc == karta.wartosc) && (g[w.aktywny_gracz - 1].reka[i].kolor == karta.kolor))
            {
                posuniecie(g, i, k, nr_kociolka, w);
                break;
            }
            i++;
        }
    }
    else //maksymalizacja koloru
    {
        i = 0;
        int najwiecej_kart = 0; //sprawdzam która karta da najwięcej kart któregokolwiek koloru na stole
        int licznik = 0, j = 0;
        while (pomgracz.reka[i].wartosc != 0)
        {
            licznik = 0;
            for (j = 0; j < w.kociolki; j++)
            {
                if (jaki_kolor_w_kociolku(k, j) == pomgracz.reka[i].kolor && pomgracz.reka[i].kolor != green) //sprawdzam ile kart damego koloru będzie na stole dla każdej możliwej karty
                {
                    for (int l = 0; l < ile_kart_w_kociolku(k, l); l++)
                    {
                        if (k[j].stos[l].kolor == pomgracz.reka[i].kolor)
                        {
                            licznik++;
                        }
                    }
                    int l = 0;
                    while (pomgracz.stol[l].wartosc != 0)
                    {
                        if (pomgracz.stol[l].kolor == pomgracz.reka[i].kolor)
                        {
                            licznik++;
                        }
                        l++;
                    }
                    break;
                }
            }
            if (licznik > najwiecej_kart)
            {
                najwiecej_kart = licznik;
                karta = pomgracz.reka[i];
                nr_kociolka = j;
            }

            i++;
        }

        if (najwiecej_kart > 0)
        {
            i = 0;
            while (g[w.aktywny_gracz - 1].reka[i].wartosc != 0)
            {
                if ((g[w.aktywny_gracz - 1].reka[i].wartosc == karta.wartosc) && (g[w.aktywny_gracz - 1].reka[i].kolor == karta.kolor))
                {
                    posuniecie(g, i, k, nr_kociolka, w);
                    break;
                }
                i++;
            }
        }
        else
        {
            posuniecie_najwieksza_karta(g, k, w);
        }
    }
    delete[] pomgracz.reka;
    delete[] pomgracz.stol;
}