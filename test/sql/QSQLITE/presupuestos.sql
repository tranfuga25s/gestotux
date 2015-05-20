CREATE TABLE IF NOT EXISTS presupuestos ( id_presupuesto INTEGER PRIMARY KEY AUTOINCREMENT, id_cliente INTEGER NULL, destinatario TEXT NULL, direccion TEXT NULL, fecha TEXT NOT NULL, total NUMERIC, serie NUMERIC NOT NULL, numero NUMERIC NOT NULL, observaciones TEXT NULL );