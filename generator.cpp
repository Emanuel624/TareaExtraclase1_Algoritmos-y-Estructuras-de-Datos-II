#include <iostream>
#include <fstream>
#include <cstdlib> // Para usar la función rand()
#include <ctime> // Para usar la función time()
#include <cstring> // Para usar la función strcmp()
using namespace std;

int main(int argc, char* argv[])
{
    // Inicializar la semilla del generador de números aleatorios
    srand(time(0));

    string size;
    string outputFilePath;

    // Leer los argumentos de la línea de comandos
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-size") == 0) {
            size = argv[++i];
        } else if(strcmp(argv[i], "-output") == 0) {
            outputFilePath = argv[++i];
        }
    }

    fstream fout; //Inicializa el objeto fstreamg+
    fout.open(outputFilePath, ios::out | ios::binary); //Se abre/genera un archivo de tipo binario

    if (fout)
    {
        if (size == "SMALL"){
            // 512MB es aproximadamente 512000000 bytes
            // Un int tiene 4 bytes, por lo que necesitamos generar aproximadamente 128000000 números
            for(int i = 0; i < 128000000; i++)
            {
                int x = rand(); // Generar un número aleatorio
                fout.write(reinterpret_cast<char*>(&x), sizeof(int));
            }
        }

        if (size == "MEDIUM"){
            // 1GB es aproximadamente 1000000000 bytes
            // Un int tiene 4 bytes, por lo que necesitamos generar aproximadamente 250000000 números enteros
            for(int i = 0; i < 250000000; i++)
            {
                int x = rand(); // Generar un número aleatorio
                fout.write(reinterpret_cast<char*>(&x), sizeof(int));
            }
        }

        if (size == "LARGE"){
            //2GB es aproximadamente 2000000000 bytes
            // Un int tiene 4 bytes, por lo que se deben generar 500000000 números enteros
            for (int i = 0; i < 500000000; i++)
            {
                int x = rand(); // Generar un número aleatorio
                fout.write(reinterpret_cast<char*>(&x), sizeof(int));
            }
        }

        fout.close();
    }else{
        cout << "Error abriendo el archivo!\n";
    }

    return 0;
}
