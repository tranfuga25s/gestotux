CREATE TABLE IF NOT EXISTS dueno ( id INTEGER PRIMARY KEY AUTOINCREMENT, nombre TEXT , apellido TEXT , direccion BLOB, email TEXT, telefono_celular TEXT, telefono_linea TEXT);
CREATE TABLE IF NOT EXISTS mascota (id INTEGER PRIMARY KEY AUTOINCREMENT, id_dueno NUMERIC , nombre TEXT , raza TEXT, genero TEXT );
CREATE TABLE IF NOT EXISTS peluqueria ( id INTEGER PRIMARY KEY AUTOINCREMENT, id_mascota INTEGER, descripcion TEXT, precio INTEGER, fecha TEXT );
