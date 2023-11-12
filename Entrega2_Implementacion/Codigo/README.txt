Información:
    Florencia Ramírez Sancristoful
    ROL 202073522-0
    Paralelo 200

☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆

Especificación de archivos:
    2dspp.cpp:
        Código principal con la implementación de la solución.
    2dspp.hpp:
        Header del archivo 2dspp.cpp.
    makefile:
        Ejecuta y compila el código.
    Estado_de_Avance.pdf:
        Diapositivas para la presentación del lunes 06 de noviembre de 2023.

☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆

Instrucciones de compilación:
    Se debe modificar el archivo makefile con el nombre de la instancia, sin incluir ".txt":
        INSTANCIA = <nombre_de_la_instancia>
    Luego, en la terminal de linux (WSL), ejecutar:
        $ make
    El cual ejecutará el archivo makefile, escribiendo los resultados en el archivo <nombre_de_la_instancia>.out,
    y finalmente, en caso de querer borrar el ejecutable y el output:
        $ make clean

☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆

Comentarios extras:
    - En caso de tener las instancias de prueba en carpetas en el mismo directorio, se debe modificar el archivo makefile
      para incluir las carpetas correspondientes.
    - Al no estar implementada la parte de Hill Climbing Alguna Mejora, no se utiliza una semilla, por lo que no se agrega
      al nombre del archivo del output.
    - Falta una revisión mejor de la funcion canBePlaced() y addRectangleGreedy(), ya que en las instancias conocidas 
      NGCUT07 y NGCUT10 se encuentra una altura menor a la óptima.
    - Se deberán hacer las modificaciones necesarias para implementar el movimiento y generar el vecindario.
    - Están comentadas las líneas usadas para calcular los tiempos de ejecución.