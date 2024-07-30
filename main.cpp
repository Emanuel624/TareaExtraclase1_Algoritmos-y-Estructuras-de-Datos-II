#include <iostream>
#include <fstream>
using namespace  std; //Sirve para no tener que poner std::

int main()
{

    fstream fout;

    unsigned short x =  8675;

    fout.open("BinaryFile.dat", ios::out | ios::binary); //Se genera el archivo binario como tal

    if (fout)
    {
        fout.write(reinterpret_cast<char*>(&x),sizeof(unsigned short));
        fout.close();
    }
    else
        cout << "Error opening file!\n";

    //Pausar el programa hasta que se presione "Enter"
    cin.ignore();
    cin.get();

    return 0;
}
