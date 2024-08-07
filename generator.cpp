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

    int iterations;
    if (size == "SMALL"){
        iterations = 128000000;
    } else if (size == "MEDIUM"){
        iterations = 250000000;
    } else if (size == "LARGE"){
        iterations = 500000000;
    } else {
        cout << "Error: SIZE debe ser SMALL, MEDIUM o LARGE.\n";
        return 1; // Terminar el programa con un código de error
    }

    fstream fout; //Inicializa el objeto fstreamg+
    fout.open(outputFilePath, ios::out | ios::binary); //Se abre/genera un archivo de tipo binario

    if (fout)
    {
        const int bufferSize = 1000000; // Tamaño del buffer
        int* buffer = new int[bufferSize]; // Crear el buffer

        for(int i = 0; i < iterations; i++)
        {
            buffer[i % bufferSize] = rand(); // Generar un número aleatorio y guardarlo en el buffer

            // Cuando el buffer esté lleno, escribirlo en el archivo y vaciarlo
            if(i % bufferSize == bufferSize - 1){
                fout.write(reinterpret_cast<char*>(buffer), bufferSize * sizeof(int));
            }
        }

        // Escribir los números restantes en el buffer
        if(iterations % bufferSize != 0){
            fout.write(reinterpret_cast<char*>(buffer), (iterations % bufferSize) * sizeof(int));
        }

        delete[] buffer; // Liberar la memoria del buffer
        fout.close();
    }else{
        cout << "Error abriendo el archivo!\n";
        return 1;
    }

    return 0;
}