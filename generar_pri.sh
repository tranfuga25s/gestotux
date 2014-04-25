#!/bin/bash
# Genera los pri para cada cliente segun el nombre
# Parametros: 1º nombre del programa
# 2º ruta del archivo rc para iconos relativo a plugins

if [ $1 == "eliminar" ]; then
  # elimino el archivo para que se genere el gestotux
  rm  -f src/cliente.pri
else
  rm -f src/cliente.pri
  touch src/cliente.pri

  echo "TARGET = $1" >> src/cliente.pri
  echo "win32 {" >> src/cliente.pri
  echo "  RC_FILE = ../plugins/$2" >> src/cliente.pri
  echo "}" >> src/cliente.pri
fi