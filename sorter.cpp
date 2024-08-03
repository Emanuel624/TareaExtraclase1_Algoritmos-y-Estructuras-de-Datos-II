#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
using namespace std;

class PagedArray {
private:
    string inputFilePath;
    string outputFilePath;
    int frame1[1000];
    int frame2[1000];
    int frame3[1000];
    int frame4[1000];

    void copyBinaryFile() {
        std::ifstream inputFile(inputFilePath, ios::binary);
        std::ofstream outputFile(outputFilePath, ios::binary);

        outputFile << inputFile.rdbuf();

        inputFile.close();
        outputFile.close();
        cout<<"Se ha copiado el archivo binario de forma correcta" << endl ;
    }

    void loadFrames() {
        std::ifstream inputFile(outputFilePath, ios::binary);

        inputFile.read(reinterpret_cast<char*>(frame1), sizeof(frame1));
        inputFile.read(reinterpret_cast<char*>(frame2), sizeof(frame2));
        inputFile.read(reinterpret_cast<char*>(frame3), sizeof(frame3));
        inputFile.read(reinterpret_cast<char*>(frame4), sizeof(frame4));

        inputFile.close();
        cout<<"Se han cargado los datos en los frames" << endl ;
    }

public:
    PagedArray(string inputFilePath, string outputFilePath)
        : inputFilePath(inputFilePath), outputFilePath(outputFilePath) {
        copyBinaryFile();
        loadFrames();
    }
};




int main(int argc, char* argv[]) {
    string inputFilePath;
    string outputFilePath;
    string algoritmo;

    // Leer los argumentos de la línea de comandos
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-input") == 0) {
            inputFilePath = argv[++i];
        } else if(strcmp(argv[i], "-output") == 0) {
            outputFilePath = argv[++i];
        }else if(strcmp(argv[i],"-alg") == 0) {
            algoritmo =  argv[++i];
        }
    }

    // Crear una nueva instancia de PagedArray
    PagedArray* pagedArray = new PagedArray(inputFilePath, outputFilePath);


    //Saber que algoritmo utilizar
    if (algoritmo == "QS") {
        cout << "Algoritmo de ordenamiento: Quick Sort" << endl;

    }else if(algoritmo == "IS"){
        cout << "Algoritmo de ordenamiento: Insertion Sort" << endl;
    }else if(algoritmo == "BS") {
        cout << "Algoritmo de ordenamiento: Bubble Sort" << endl;
    }else{
        cout << "Error: Se debe escoger solamente QS (Quick Sort), IS (Insertion Sort) o BS (Bubble Sort) como algoritmos de ordenamiento" <<endl;
    }
    cout << "Input File Path: " << inputFilePath << endl;
    cout << "Output File Path: " << outputFilePath << endl;
}

