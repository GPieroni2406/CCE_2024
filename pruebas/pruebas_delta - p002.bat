@echo off
REM Cambia al directorio donde se encuentran las herramientas
cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\herramientas_brindadas"

REM Ejecuta los comandos
reechannel -q 256 -n 42 -R 1050000 -s 25 ..\simb_borr\bloques_p002.dat
rsencode -q 256 -n 48 -r 6 -l ..\simb_borr\bloques_p002.dat ..\simb_borr\bloques_p002.rse
reechannel -q 256 -n 48 --delta 0.00001 --rho 0.02 -s 25 ..\simb_borr\bloques_p002.rse

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\src"
rsdecode -n 48 -r 6 -s ..\simb_borr\bloques_p002.ech -e ..\simb_borr\bloques_p002.eras -o ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\herramientas_brindadas"
echo "--------- resultados con delta = 0.00001 y rho = 0.02 -------------" >> ..\resultados_pruebas\resultados_p002.txt
diffblock -q 256 -n 42 ..\simb_borr\bloques_p002.dat ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt
echo "----------------------" >> ..\resultados_pruebas\resultados_p002.txt

rsencode -q 256 -n 48 -r 6 -l ..\simb_borr\bloques_p002.dat ..\simb_borr\bloques_p002.rse
reechannel -q 256 -n 48 --delta 0.0001 --rho 0.02 -s 25 ..\simb_borr\bloques_p002.rse

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\src"
rsdecode -n 48 -r 6 -s ..\simb_borr\bloques_p002.ech -e ..\simb_borr\bloques_p002.eras -o ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\herramientas_brindadas"
echo "--------- resultados con delta = 0.0001 y rho = 0.02 -------------" >> ..\resultados_pruebas\resultados_p002.txt
diffblock -q 256 -n 42 ..\simb_borr\bloques_p002.dat ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt
echo "----------------------" >> ..\resultados_pruebas\resultados_p002.txt

rsencode -q 256 -n 48 -r 6 -l ..\simb_borr\bloques_p002.dat ..\simb_borr\bloques_p002.rse
reechannel -q 256 -n 48 --delta 0.001 --rho 0.02 -s 25 ..\simb_borr\bloques_p002.rse

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\src"
rsdecode -n 48 -r 6 -s ..\simb_borr\bloques_p002.ech -e ..\simb_borr\bloques_p002.eras -o ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\herramientas_brindadas"
echo "--------- resultados con delta = 0.001 y rho = 0.02 -------------" >> ..\resultados_pruebas\resultados_p002.txt
diffblock -q 256 -n 42 ..\simb_borr\bloques_p002.dat ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt
echo "----------------------" >> ..\resultados_pruebas\resultados_p002.txt


rsencode -q 256 -n 48 -r 6 -l ..\simb_borr\bloques_p002.dat ..\simb_borr\bloques_p002.rse
reechannel -q 256 -n 48 --delta 0.01 --rho 0.02 -s 25 ..\simb_borr\bloques_p002.rse

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\src"
rsdecode -n 48 -r 6 -s ..\simb_borr\bloques_p002.ech -e ..\simb_borr\bloques_p002.eras -o ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\herramientas_brindadas"
echo "--------- resultados con delta = 0.01 y rho = 0.02 -------------" >> ..\resultados_pruebas\resultados_p002.txt
diffblock -q 256 -n 42 ..\simb_borr\bloques_p002.dat ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt
echo "----------------------" >> ..\resultados_pruebas\resultados_p002.txt

rsencode -q 256 -n 48 -r 6 -l ..\simb_borr\bloques_p002.dat ..\simb_borr\bloques_p002.rse
reechannel -q 256 -n 48 --delta 0.1 --rho 0.02 -s 25 ..\simb_borr\bloques_p002.rse

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\src"
rsdecode -n 48 -r 6 -s ..\simb_borr\bloques_p002.ech -e ..\simb_borr\bloques_p002.eras -o ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt

cd /d "C:\Users\GPieroni\Desktop\FACU\CCE\laboratorio_cce_2024\herramientas_brindadas"
echo "--------- resultados con delta = 0.1 y rho = 0.02 -------------" >> ..\resultados_pruebas\resultados_p002.txt
diffblock -q 256 -n 42 ..\simb_borr\bloques_p002.dat ..\salidas\salida.dat >> ..\resultados_pruebas\resultados_p002.txt
echo "----------------------" >> ..\resultados_pruebas\resultados_p002.txt