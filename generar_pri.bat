echo off
del ./src/cliente.pri
if "%1"=="eliminar" goto fin
echo TARGET = %1 >> src/cliente.pri
echo win32 { >> src/cliente.pri
echo   RC_FILE = ../plugins/%2 >> src/cliente.pri
echo } >> src/cliente.pri
goto fin
:fin
echo Archivo del cliente terminado.
