#!/bin/bash

if cd ./ayuda; then
 echo "Generando Docuemntacion interna del programa....";
 eval 'qhelpgenerator documentacion.qhp -o ../bin/documentacion.qch';
else
 echo "No estoy en el directorio del gestotux. Saliendo...";
fi
echo "Listo."