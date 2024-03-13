CREATE DATABASE centralina;
USE centralina;
CREATE TABLE DatiCentraline(
    id INT NOT NULL AUTO_INCREMENT,
    codiceCentralina VARCHAR(7) NOT NULL,
    temperatura FLOAT NOT NULL,
    luce FLOAT NOT NULL,
    umidita FLOAT NOT NULL,
    pm FLOAT,

    PRIMARY KEY(id)
);

CREATE TABLE avvisi(
    oraEvento DATETIME NOT NULL PRIMARY KEY,
    idCentralina INT NOT NULL UNIQUE,
    FOREIGN KEY (idCentralina) REFERENCES DatiCentraline(id)
)