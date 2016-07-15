#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <cstdio>
#include <cstdlib>

using namespace std;
ofstream plikWyjsciowy;
ifstream plikWejsciowy;
string alfabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//funkcja kodujaca
string kodowanieBase64(char * kodowanyBlok, int dlugoscBloku)
{
    string ciagZwracany;
    int i = 0;
    int j = 0;
    char blok3Bajty[3];
    char blok4Bajty[4];

    while (dlugoscBloku>0)
    {
        blok3Bajty[i++] = *(kodowanyBlok++);
        if (i == 3)
        {
            blok4Bajty[0] = (blok3Bajty[0] & 0xfc) >> 2;
            blok4Bajty[1] = ((blok3Bajty[0] & 0x03) << 4) + ((blok3Bajty[1] & 0xf0) >> 4);
            blok4Bajty[2] = ((blok3Bajty[1] & 0x0f) << 2) + ((blok3Bajty[2] & 0xc0) >> 6);
            blok4Bajty[3] = blok3Bajty[2] & 0x3f;
            for(i = 0; i<4 ; i++)
            {
                ciagZwracany += alfabet[blok4Bajty[i]];
            }
            i = 0;
        }
        dlugoscBloku--;
    }
    if (i!=0)
    {
        for(j = i; j < 3; j++)
        {
            blok4Bajty[0] = (blok3Bajty[0] & 0xfc) >> 2;
            blok4Bajty[1] = ((blok3Bajty[0] & 0x03) << 4) + ((blok3Bajty[1] & 0xf0) >> 4);
            blok4Bajty[2] = ((blok3Bajty[1] & 0x0f) << 2) + ((blok3Bajty[2] & 0xc0) >> 6);
            blok4Bajty[3] = blok3Bajty[2] & 0x3f;
        }
        for (j=0; (j<i+1); j++)
        {
            ciagZwracany += alfabet[blok4Bajty[j]];
        }
        while((i++ <3))
        {
            ciagZwracany += '=';
        }
    }
    return ciagZwracany;
}

//funkcja dekodująca
string dekodowanieBase64(string & zakodowanaLinia)
{
    int dlugoscLinii = zakodowanaLinia.size();
    int i = 0;
    int j = 0;
    int poz = 0;
    char blok4Bajty[4], blok3Bajty[3];
    string ciagZwracany;

    while (dlugoscLinii-- && ( zakodowanaLinia[poz] != '='))
    {
        blok4Bajty[i++] = zakodowanaLinia[poz]; poz++;
        if (i ==4)
        {
            for (i = 0; i <4; i++)
            {
                blok4Bajty[i] = alfabet.find(blok4Bajty[i]);
                blok3Bajty[0] = (blok4Bajty[0] << 2) + ((blok4Bajty[1] & 0x30) >> 4);
                blok3Bajty[1] = ((blok4Bajty[1] & 0xf) << 4) + ((blok4Bajty[2] & 0x3c) >> 2);
                blok3Bajty[2] = ((blok4Bajty[2] & 0x3) << 6) + blok4Bajty[3];
            }
            for (i = 0; (i < 3); i++)
            {
                ciagZwracany += blok3Bajty[i];
            }
            i = 0;
        }
    }
    if (i!=0)
    {
    for (j = i; j <4; j++)
    {
        blok4Bajty[j] = 0;
    }
        for (j = 0; j <4; j++)
        {
            blok4Bajty[j] = alfabet.find(blok4Bajty[j]);
            blok3Bajty[0] = (blok4Bajty[0] << 2) + ((blok4Bajty[1] & 0x30) >> 4);
            blok3Bajty[1] = ((blok4Bajty[1] & 0xf) << 4) + ((blok4Bajty[2] & 0x3c) >> 2);
            blok3Bajty[2] = ((blok4Bajty[2] & 0x3) << 6) + blok4Bajty[3];
        }
        for (j = 0; (j < i - 1); j++)
        {
            ciagZwracany += blok3Bajty[j];
        }
    }
    return ciagZwracany;
}


int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout<<"Nieprawidlowa liczba argumentow."<<endl<<"Uzycie: b64 (-k|-d) plik_wej plik_wyj"<<endl;
        cout<<"-k - koduj do base64"<<endl<<"-d - dekoduj z base64"<<endl<<"plik_wej - nazwa pliku wejsciowego (z rozszerzeniem)"<<endl;
        cout<<"plik_wyj - nazwa pliku wynikowego (z ew. rozszerzeniem)"<<endl;
        exit(1);
    }
    char *typPracy = {0};
    char *plikWej = argv[2];
    char *plikWyj = argv[3];
    typPracy=argv[1];
    if(typPracy[0]!='-' or typPracy[2]!=0)
    {
        cout<<"Nieprawidłowa składnia"<<endl;
        cout<<"Prawidłowe wywołanie to: b64 (-k|-d) plik_wej plik_wyj"<<endl;
    }
    else
    switch(typPracy[1])
    {
        case 'k':
        {
            char linia[54] = {0};
            plikWejsciowy.open(plikWej, ios::binary);
            plikWyjsciowy.open(plikWyj, ios::binary);
            int dlugosc;
            if(plikWejsciowy && plikWyjsciowy)
            {
                plikWejsciowy.seekg(0, ios::end);
                dlugosc = plikWejsciowy.tellg();
                char a;
                for (int i=0; i<(dlugosc/54); i++)
                {
                    for (int j=0; j<54; j++)
                    {
                        plikWejsciowy.seekg(i*54+j, ios::beg);
                        plikWejsciowy.get(a);
                        linia[j] = a;
                    }
                    linia[54]='\0';
                    string zakodowanaLinia = kodowanieBase64(linia, 54);
                    plikWyjsciowy<<zakodowanaLinia<<endl;
                }
                if (dlugosc%54!=0)
                {
                    int reszta = dlugosc%54;
                    for (int j=0; j<reszta; j++)
                    {
                        plikWejsciowy.seekg((dlugosc/54)*54+j, ios::beg);
                        plikWejsciowy.get(a);
                        linia[j] = a;
                    }
                    linia[reszta]='\0';
                    string zakodowanaLinia = kodowanieBase64(linia, reszta);
                    plikWyjsciowy<<zakodowanaLinia<<endl;
                }
                cout<<endl<<"Zakodowano plik \""<<plikWej<<"\" o rozmiarze "<<dlugosc<<" bajtow do pliku \""<<plikWyj<<"\"."<<endl;
            }
            else cout << "BLAD! Nie udalo sie otworzyc ktoregos pliku!" << endl;
            plikWejsciowy.close();
            plikWyjsciowy.close();
            break;
        }
        case 'd':
        {
            plikWejsciowy.open(plikWej, ios::binary);
            plikWyjsciowy.open(plikWyj, ios::binary);
            if (plikWejsciowy && plikWyjsciowy)
            {
                plikWejsciowy.seekg(0, ios::end);
                int dlugosc = plikWejsciowy.tellg();
                /*if(dlugosc%4!=0)
                {
                    cout<<"\""<<plikWej<<"\" to nie jest plik prawidlowo zakodowany w base64!"<<endl;
                    exit(1);
                }*/
                plikWejsciowy.seekg(0, ios::beg);
                string liniaDekodowana;
                string zdekodowane;
                while (!plikWejsciowy.eof())
                {
                    getline(plikWejsciowy, liniaDekodowana);
                    zdekodowane = dekodowanieBase64(liniaDekodowana);
                    plikWyjsciowy<<zdekodowane;
                }
                cout<<endl<<"Zdekodowano plik \""<<plikWej<<"\" do pliku \""<<plikWyj<<"\"."<<endl;
            }
            else cout << "BLAD! Nie udalo sie otworzyc ktoregos pliku!" << endl;
            plikWejsciowy.close();
            plikWyjsciowy.close();
            break;
        }
        default:
        cout<<"Chyba cos pomyliles w wywolaniu..."<<endl;
    }
return 0;
}
