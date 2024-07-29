#include <iostream>
#include <fstream>
#include <cstdlib> // Para usar la función rand()
#include <ctime> // Para usar la función time()
using namespace std;

int main()
{
    // Inicializar la semilla del generador de números aleatorios
    srand(time(0));

    fstream fout; //Inicializa el objeto fstream
    fout.open("BinaryFile.dat", ios::out | ios::binary); //Se abre/genera un archivo de tipo binario

    if (fout)
    {
        string input;
        cout << "Escribe 'SMALL' para generar un archivo binario de 512MB: ";
        cin >> input;

        if (input == "SMALL")
        {
            // 512MB es aproximadamente 512000000 bytes
            // Un int tiene 4 bytes, por lo que necesitamos generar aproximadamente 128000000 números
            for(int i = 0; i < 128000000; i++)
            {
                int x = rand(); // Generar un número aleatorio
                //cout << x << endl; //imprimir numeros

                fout.write(reinterpret_cast<char*>(&x), sizeof(int));
            }
        }


        if (input == "MEDIUM")
        {
            // 1GB es aproximadamente 1000000000 bytes
            // Un int tiene 4 bytes, por lo que necesitamos generar aproximadamente 250000000 números enteros
            for(int i = 0; i < 250000000; i++)
            {
                int x = rand(); // Generar un número aleatorio
                //cout << x << endl; //imprimir numeros

                fout.write(reinterpret_cast<char*>(&x), sizeof(int));
            }
        }


        if (input == "LARGE")
        {
            //2GB es aproximadamente 2000000000 bytes
            // Un int tiene 4 bytes, por lo que se deben generar 500000000 números enteros
            for (int i = 0; i < 500000000; i++)
            {
                int x = rand(); // Generar un número aleatorio
                //cout << x << endl; //imprimir numeros

                fout.write(reinterpret_cast<char*>(&x), sizeof(int));
            }
        }

        fout.close();
    }
    else
    {
        cout << "Error opening file!\n";
    }

    cin.ignore();
    cin.get();

    return 0;
}
