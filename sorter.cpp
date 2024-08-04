#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;


class PagedArray {
private:
    string inputFilePath;
    string outputFilePath;
    int frame1[1024];
    int frame2[1024];
    int frame3[1024];
    int frame4[1024];

    void copyBinaryFile() {
        ifstream inputFile(inputFilePath, ios::binary);
        ofstream outputFile(outputFilePath, ios::binary);

        outputFile << inputFile.rdbuf();

        inputFile.close();
        outputFile.close();
        cout<<"Se ha copiado el archivo binario de forma correcta" << endl ;
    }

    void loadFrames() {
        ifstream inputFile(outputFilePath, ios::binary);

        inputFile.read(reinterpret_cast<char*>(frame1), sizeof(frame1));
        inputFile.read(reinterpret_cast<char*>(frame2), sizeof(frame2));
        inputFile.read(reinterpret_cast<char*>(frame3), sizeof(frame3));
        inputFile.read(reinterpret_cast<char*>(frame4), sizeof(frame4));

        inputFile.close();
        cout<<"Se han cargado los datos en los frames" << endl ;
    }

public:
    PagedArray(string inputFilePath, string outputFilePath)
        : inputFilePath(inputFilePath), outputFilePath(outputFilePath){
        copyBinaryFile();
        loadFrames();
    }

    int& operator[](size_t index) {
        int pagina = index / 1024;
        int* currentFrame = nullptr;

        // Determinar en qué frame se encuentra el valor
        if (pagina == 0) {
            currentFrame = frame1;
        } else if (pagina == 1) {
            currentFrame = frame2;
        } else if (pagina == 2) {
            currentFrame = frame3;
        } else if (pagina == 3) {
            currentFrame = frame4;
        } else {
            // Si la página no está en memoria, la cargamos desde el disco
            ifstream inputFile(outputFilePath, ios::binary);
            inputFile.seekg(index * sizeof(int), ios::beg);

            // Si todos los marcos están llenos, reemplazamos un marco aleatorio con la nueva página
            int randomFrame = rand() % 4; // Genera un número aleatorio entre 0 y 3
            switch (randomFrame) {
                case 0:
                    inputFile.read(reinterpret_cast<char*>(frame1), sizeof(frame1));
                    currentFrame = frame1;
                    break;
                case 1:
                    inputFile.read(reinterpret_cast<char*>(frame2), sizeof(frame2));
                    currentFrame = frame2;
                    break;
                case 2:
                    inputFile.read(reinterpret_cast<char*>(frame3), sizeof(frame3));
                    currentFrame = frame3;
                    break;
                case 3:
                    inputFile.read(reinterpret_cast<char*>(frame4), sizeof(frame4));
                    currentFrame = frame4;
                    break;
            }

            inputFile.close();
        }
        return currentFrame[index % 1024];
    }
};

//QuickSort
void quickSort(PagedArray& arr, int start, int end) {
    if (start >= end)
        return;
    int pivot = arr[start];
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }
    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);
    int i = start, j = end;
    while (i < pivotIndex && j > pivotIndex) {
        while (arr[i] <= pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }
    quickSort(arr, start, pivotIndex - 1);
    quickSort(arr, pivotIndex + 1, end);
}

//InsertionSort
void insertionSort(PagedArray& arr, int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

//BubbleSort
void bubbleSort(PagedArray& arr, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

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

    // Abrir el archivo de salida en modo binario
    std::ifstream file(outputFilePath, std::ios::binary | std::ios::ate);
    // Obtener el tamaño del archivo
    std::streamsize size = file.tellg();
    // Cerrar el archivo
    file.close();

    // Calcular el número de enteros en el archivo
    int numIntegers = size / sizeof(int);

    //Saber que algoritmo utilizar
    if (algoritmo == "QS") {
        cout << "Algoritmo de ordenamiento: Quick Sort" << endl;
        quickSort(*pagedArray, 0, numIntegers - 1); // Asegúrate de que el rango sea correcto
    }else if(algoritmo == "IS"){
        cout << "Algoritmo de ordenamiento: Insertion Sort" << endl;
        insertionSort(*pagedArray, numIntegers); // Asegúrate de que el tamaño sea correcto
    }else if(algoritmo == "BS") {
        cout << "Algoritmo de ordenamiento: Bubble Sort" << endl;
        bubbleSort(*pagedArray, numIntegers); // Asegúrate de que el tamaño sea correcto
    }else{
        cout << "Error: Se debe escoger solamente QS (Quick Sort), IS (Insertion Sort) o BS (Bubble Sort) como algoritmos de ordenamiento" <<endl;
    }
    cout << "Input File Path: " << inputFilePath << endl;
    cout << "Output File Path: " << outputFilePath << endl;
}

