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
    Usando los valores predeterminados de instancia y semilla, en la terminal de linux (WSL) se ejecuta:
        $ make
    El cual ejecutará el archivo makefile, escribiendo los resultados en el archivo <instancia>_<semilla>.out. En caso de 
    querer borrar el ejecutable y el output se ejecuta:
        $ make clean 
    Si se quieren usar otra instancia y/o semilla, se pueden modificar estas variables en el archivo makefile o se puede
    hacer directamente en la línea de comando ejecutando:
        $ make INSTANCIA="<instancia_nueva>" SEMILLA="<semilla_nueva>"
    Lo cual reemplaza los valores predeterminados, si se quiere cambiar un solo un valor se escribe solamente esa variable,
    por último, si se quiere borrar el ejecutable y output de estos casos, también se deben entregar los valores cambiados:
        $ make clean INSTANCIA="<instancia_nueva>" SEMILLA="<semilla_nueva>"

☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆.。.:*・°☆

Comentarios extras:
    - Se asume que la instancia de prueba se encuentra en el mismo directorio.
	- En caso de tener las instancias de prueba en carpetas en el mismo directorio, se debe modificar el archivo makefile 
      para incluir las carpetas correspondientes. Esto también haría que el output se escriba en dicha carpeta.
    - No está implementado Hill Climbing Alguna Mejora.
    - Falta una revisión mejor de la funcion canBePlaced() y addRectangleGreedy(), ya que en las instancias conocidas 
      NGCUT07 y NGCUT10 se encuentra una altura menor a la óptima.
    - Se deberán hacer las modificaciones necesarias para implementar el movimiento y generar el vecindario.
    - Están comentadas las líneas usadas para calcular los tiempos de ejecución.