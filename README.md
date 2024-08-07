¿Como se ejecutan los programas en linux?

*Generator*
1. Abrir la terminal dentro del directorio en el que se encuentra el archivo de generator.cpp
2. Para evitar conflictos se recomienda compilar el codigo con el siguiente comando:


              g++ generator.cpp -o generator

3. Una vez compilado por seguridad, se puede ejecutar la aplicación con el siguiente comando:
   
                ./generator –size <SIZE> -output <OUTPUT FILE PATH>
   Donde;
         
          <SIZE>  -> Puede tener los valores de SMALL(512MB), MEDIUM(1GB) O LARGE(2GB)

         <OUTPUT FILE PATH>  -> Es la ruta donde se quiere generar el archivo, 
por ejemplo:
                               /home/emanuel/Desktop/prueba/small_unsorted


5. Esperar unos segundos y el archivo se generará en la ruta descrita
6. Nota: En caso de que el argumento <SIZE>, sea diferente a los citados, se generará un error y el programa no se ejecutara, al igual forma como si se diera una ruta inadecuada para la generación del archivo binario.






*Sorter*
1. Abrir la terminal dentro del directorio en el que se encuentra el archivo de sorter.cpp
2. Para evitar conflictos se recomienda compilar el codigo con el siguiente comando:

          
              g++ sorter.cpp -o sorter

3. Una vez compilado por seguridad, se puede ejecutar la aplicación con el siguiente comando:
   
          ./sorter –input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITMO>
   Donde;

         
          <INPUT FILE PATH>  ->  Es la ruta del archivo binario que se desea ordenar de forma ascendente
         
          <OUTPUT FILE PATH> ->  Es la ruta donde se va a generar el archivo binario ya ordenado (junto con su csv)
         
          <ALGORITMO>  ->  Es el tipo de algoritmo de ordenamiento que se desea utilizar (1. QS: Quick sort, 2. IS: Insertion sort, 3. BS: bubble sort)

4. Nota técnica importante: La implementación de sorter no fue la mas optima, es decir todo funciona bien, sin embargo se dura *demasiado* en el ordenamiento de los archivos. Se recomienda para efectos de la revisón utilizar un archivo SMALL (512MB) y QS, ya que este va a durar alrededor de 30/35 minutos (no pude optimizarlo de otra forma).

5. En caso de dar un algoritmo que no sean los indicados o dar alguna de las rutas de los archivos de forma incorrecta el programa sorter no se va a ejecutar.
   

