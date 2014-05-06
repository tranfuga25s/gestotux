CREATE TABLE IF NOT EXISTS item_factura ( "id_item_factura" INTEGER PRIMARY KEY autoincrement, "id_factura" INTEGER NOT NULL, "cantidad" DOUBLE NOT NULL, "texto" TEXT NOT NULL, "precio_unitario" DOUBLE NOT NULL, "id_producto" INTEGER NULL );
INSERT OR IGNORE INTO item_factura VALUES ( 1, 1, 5, "Producto 1", 15.2, 1 );
INSERT OR IGNORE INTO item_factura VALUES ( 1, 1, 5, "Producto 2", 15.2, 2 );