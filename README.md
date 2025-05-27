# 📚 Sistema para el Préstamo de Libros

Este proyecto simula el funcionamiento básico de una biblioteca, permitiendo gestionar operaciones de **préstamo**, **renovación** y **devolución** de libros mediante procesos concurrentes que se comunican a través de **pipes** y se sincronizan con **hilos y semáforos POSIX**.

## 👥 Integrantes

* Juan Martín Sánchez
* Juan Sebastián Téllez
* Nathalia Ramírez
* Juan Esteban Camargo
* Santiago Martínez

**Materia:** Sistemas Operativos
**Profesor:** John Jairo Corredor

---

## 🫠 Descripción del sistema

El sistema está compuesto por tres módulos principales:

* `Solicitante.c`: representa al proceso solicitante (PS), que genera solicitudes desde un menú o un archivo y las envía por un pipe.
* `Receptor.c`: proceso receptor (RP), que recibe solicitudes, procesa préstamos directamente y delega renovaciones/devoluciones a un hilo auxiliar.
* `SistemaDePrestamoDeLibros.c`: módulo de lógica para cargar, modificar y guardar la base de datos en memoria.

Además, se incluye un hilo de consola que permite detener el sistema (`s`) o generar un reporte simulado (`r`).

---

## 🛠️ Compilación

Este proyecto se compila utilizando `make`.

```bash
make       # Compila solicitante y receptor
make clean # Limpia archivos objeto y ejecutables
```

---

## ▶️ Ejecución del sistema

### 1. Crear el pipe nominal (una sola vez)

```bash
mkfifo pipeRP
```

---

### 2. Ejecutar el receptor

```bash
./receptor -p pipeRP -f bd.txt -v -s bd_final.txt
```

**Parámetros:**

* `-p` → nombre del pipe
* `-f` → archivo de base de datos de entrada
* `-v` → modo verbose (opcional)
* `-s` → archivo de salida (opcional)

---

### 3. Ejecutar el solicitante

**Modo interactivo:**

```bash
./solicitante -p pipeRP
```

**Modo automático (desde archivo):**

```bash
./solicitante -i solicitudes.txt -p pipeRP
```

---

## 📜 Formato de solicitudes

Cada línea representa una operación con el siguiente formato:

```php
<tipo>,<nombre del libro>,<isbn>
```

* `P` → Préstamo
* `R` → Renovación
* `D` → Devolución
* `Q` → Finalizar sistema

**Ejemplo:**

```css
P,Operating Systems,2233
R,Operating Systems,2233
D,Operating Systems,2233
Q,Salir,0
```

---

## 📄 Formato del archivo de base de datos (`bd.txt`)

```txt
Nombre del libro,ISBN,Cantidad
1,D,12-05-2024
2,P,10-05-2024
```

* Cada libro tiene una línea de encabezado con su nombre, ISBN y cantidad.
* Le siguen tantas líneas como ejemplares, con el formato:

  ```
  <número>,<estado>,<fecha>
  ```

  Donde `estado` puede ser:

  * `D` → disponible
  * `P` → prestado

---

## 🗂️ Estructura del proyecto

```bash
Sistema-para-el-Prestamo-de-Libros/
├── Solicitante.c                  # Código del proceso solicitante
├── Receptor.c                    # Código del proceso receptor
├── SistemaDePrestamoDeLibros.c  # Módulo de base de datos
├── SistemaDePrestamoDeLibros.h  # Header del módulo BD
├── bd.txt                        # Base de datos de entrada
├── solicitudes.txt               # Solicitudes para modo automático
├── bd_final.txt                  # Archivo de salida (base final)
├── Makefile                      # Script de compilación
└── README.md                     # Documentación del proyecto
```

---

## 🔎 Modo verbose

Si ejecutas el receptor con la opción `-v`, se mostrará información detallada de cada paso, como:

```less
[RP] Recibido: P,Operating Systems,2233
[RP] Procesando préstamo de Operating Systems
[RP] Préstamo realizado con éxito (ISBN 2233)
```

Esto permite seguir el flujo interno del sistema durante la ejecución, útil para pruebas y depuración.

---

## ❗ Notas importantes

* El pipe (`pipeRP`) debe existir antes de ejecutar los procesos.
* El receptor debe estar activo antes de iniciar el solicitante.
* El sistema puede finalizarse de dos formas:

  * Enviando una solicitud `Q` desde el solicitante.
  * Presionando `s` en la consola del receptor.

---

## ✅ Requisitos

* Sistema operativo tipo Unix/Linux
* Compilador `gcc`
* Soporte para:

  * `<pthread.h>`
  * `<semaphore.h>`
  * `<fcntl.h>`
  * `<unistd.h>`
