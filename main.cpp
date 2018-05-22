#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;
class Marker
{
public:
    vector<string> dane;
    string marker_name;

    Marker(void)
    {
        marker_name = "nowy";
    }
    ~Marker() {}
};

class Sample
{
public:
    string name;
    string robocza;
    Marker *marker_;
    int It_marker;
    static int ile_alleli;

    Sample(void)
    {

        marker_ = new Marker[50];
        name = "";
        It_marker = 0;

    }
    ~Sample()
    {
        delete [] marker_;
        //cout<<"Destruktor Sample";
    }

friend ostream & operator<<(ostream & os, Sample & v);


};
int Sample::ile_alleli = 0;
static bool header = false;
ostream & operator<<(ostream & os, Sample & v)
{
    if(!header){
        os<< "Nazwa probki\\Markery";
        for( int i = 0; i < v.It_marker; i++)
            os<<'\t'<<v.marker_[i].marker_name;
        os<<'\n';
        header = true;
    }

    os<<v.name;
    for(int i = 0; i < v.It_marker; i++)
    {
        os<<'\t';
        bool next = false;
        for(int j = 0; j < Sample::ile_alleli; j++)
        {

            if(v.marker_[i].dane.at(j) == "" || v.marker_[i].dane.at(j) == "OL")
                continue;
            else {
                if(next)
                    os<<", ";

                os<<v.marker_[i].dane.at(j);
                next = true;
            }
        }

    }
    //os<<'\n';

    return os;
}

void status(Sample & sp)
{
    cout<<"Zawartosc probki:\n";
    cout<<"\nNazwa probki: "<< sp.name<<"\n";
    for(int i = 0; i < sp.It_marker;i++)
    {
        cout<<"Marker["<<i+1<<"]: "<<sp.marker_[i].marker_name<<"\n";
        //for( int j = 0; j < sp.It_marker; j++)
        for( int j = 0; j < Sample::ile_alleli; j++)
            cout<<sp.marker_[i].dane.at(j)<<" ";

        cout<<'\n';


    }
    cout<<"Koniec\n";
    //getchar();

}
int main(int argc, char* argv[])
{
    if(argc == 2)
        if(!strcmp( argv[ 1 ], "--info" ))
        {
            cout<<"Wersja 4 (17.05.2018)";
            exit(0);
        }
    if(argc == 1)
    {
        cout<<"Podaj nazwe pliku *.txt";
        exit(1);
    }
    if(argc != 2)
    {
        cout<<"Podaj tylko nazwe pliku np. nazwa.txt";
        exit(2);
    }
    Sample *ptr_probka = new Sample();

    string nazwa_pliku ="[v4]convert-";
    ifstream odczyt;
    odczyt.open(argv[1]);
    if(odczyt.fail())
    {
        cout<<"Brak pliku lub nie mozna go otworzyc. \n"
        << "Sprawdz czy program i plik sa w tym samym katalogu lub czy sciezka do pliku jest prawidlowa";
        exit(3);
    }
    ofstream zapis(nazwa_pliku+argv[1]);

    string item;    //bufor wyrazu
    odczyt.ignore(300,'\t'); //ignoruje Sample File
    odczyt.ignore(300,'\t'); //ignoruje Sample Name
    odczyt.ignore(300,'\t'); //ignoruje Marker
    while(getline(odczyt,item,'\t'))
    {
        ptr_probka->ile_alleli++;
        if(odczyt.peek() == '\n')
        {
            getline(odczyt,item,'\n');
            break;
        }
    }

    cout<<"Ilosc alleli: "<<Sample::ile_alleli<<endl;

    while(odczyt) {


            getline(odczyt,item, '\t'); // odczyt nazwy roboczej

            if(ptr_probka->robocza.empty())    //Czy to nowa probka?
            {
                ptr_probka->robocza = item;
            }
            if(ptr_probka->robocza == item)     //Czy dalej ta sama probka?
            {
                getline(odczyt,item, '\t'); // odczyt nazwy
                ptr_probka->name = item;
                getline(odczyt,item, '\t'); //odczyt markera
                ptr_probka->marker_[ptr_probka->It_marker].marker_name = item;

                while(getline(odczyt,item,'\t') && odczyt.peek() != '\n') //odczyt danych
                {
                    ptr_probka->marker_[ptr_probka->It_marker].dane.push_back(item);
                }

                ptr_probka->marker_[ptr_probka->It_marker].dane.push_back(item);
                ptr_probka->It_marker++;
                getline(odczyt,item,'\n');

            } else {

                //ptr_probka->It_marker--;
                //status(*ptr_probka);
                zapis<<*ptr_probka<<endl;   //zrzut danych do pliku
                delete ptr_probka;         //zwolnienie pamieci
                ptr_probka = new Sample();  //alokacja

                ptr_probka->robocza = item;
                getline(odczyt,item, '\t'); //odczyt nazwy
                ptr_probka->name = item;
                getline(odczyt,item, '\t'); //odczyt markera
                ptr_probka->marker_[ptr_probka->It_marker].marker_name = item;

                while(getline(odczyt,item,'\t') && odczyt.peek() != '\n') //odczyt danych
                {
                    ptr_probka->marker_[ptr_probka->It_marker].dane.push_back(item);
                }

                ptr_probka->marker_[ptr_probka->It_marker].dane.push_back(item);
                ptr_probka->It_marker++;
                getline(odczyt,item,'\n');

            }
    }
    delete ptr_probka;
    cout<<"Plik przetworzony\n";

}

