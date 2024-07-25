# Instrucciones para el uso y la ejecución del decodificador

El siguiente documento contiene las instrucciones para la compilación y ejecución del programa, así como archivos generados para compilar automáticamente.

## Comandos de ejecución

Se agregan los siguientes comandos de ejecución utilizados:

### Generación de Datos

```bash
reechannel -q 256 -n {dimension} -R {cantidad de bloques} -s {semilla} {archivo_salida}.dat

rsencode -q 256 -n {largo_del_codigo} -r {redundancia} -l {archivo_salida}.dat {archivo_salida}.rse

reechannel -q 256 -n {largo_del_codigo} --delta {prob_de_errores_en_simbolos} --rho {prob_de_borraduras} {archivo_salida}.rse
```

### Compilación del Programa

Dentro de la carpeta "src" se deja el programa compilado. En particular se utilizó el siguiente comando para compilarlo en Windows:

```bash
g++ -O3 -o rsdecode main.cpp Decodificador.cpp ManejadorDeArchivos.cpp gf256.cpp Calculos.cpp Polinomio.cpp -std=c++11
```

### Ejecución del Programa

Para realizar una ejecución del programa, se utiliza el siguiente comando:

```bash
ubi\del\prog\rsdecode -n {largo_del_codigo} -r {redundancia} -s ubi\de\simb_borr\datos.ech -e ubi\de\simb_borr\datos.eras -o ..\salidas\datos.dat >> ubi\de\resultados_pruebas\resultados.txt
```

# Pruebas realizadas

Se dejan dos `.bat` dentro de la carpeta "pruebas", con las pruebas realizadas para los delta y rho pedidos en la letra del proyecto.
Los resultados de dichas pruebas se dejan registrados en la carpeta "resultados_pruebas".


# Importante
Se debe reemplazar los valores entre llaves `{}` con la información específica necesaria antes de ejecutar los comandos, al igual que las rutas especificas de los simbolos y borraduras.