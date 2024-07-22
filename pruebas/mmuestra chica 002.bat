@echo off
REM Cambia al directorio donde se encuentran las herramientas
cd /d "..\herramientas_brindadas"

REM Ejecuta los comandos
reechannel -q 256 -n 42 -R 1050000 -s 25 ..\simb_borr\bloques.dat
rsencode -q 256 -n 48 -r 6 -l ..\simb_borr\bloques.dat ..\simb_borr\bloques.rse
reechannel -q 256 -n 48 --delta 0.00001 --rho 0.02 -s 25 ..\simb_borr\bloques.rse

cd /d "..\src"
rsdecode -n 48 -r 6 -s ..\simb_borr\bloques.ech -e ..\simb_borr\bloques.eras -o ..\salidas\salida.dat >> ..\resultados_pruebas\resultados.txt

cd /d "..\herramientas_brindadas"
echo "--------- resultados con delta = 0.00001 y rho = 0.02 -------------" >> ..\resultados_pruebas\resultados.txt
diffblock -q 256 -n 42 ..\simb_borr\bloques.dat ..\salidas\salida.dat >> ..\resultados_pruebas\resultados.txt
echo "----------------------" >> ..\resultados_pruebas\resultados.txt