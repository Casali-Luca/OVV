CREATE DATABASE centralina;
USE centralina;
CREATE TABLE centrale(
    id INT NOT NULL AUTO_INCREMENT,
    temperatura FLOAT NOT NULL,
    luce FLOAT NOT NULL,
    umidita FLOAT NOT NULL,
    pm FLOAT NOT NULL,
    PRIMARY KEY(id)
)