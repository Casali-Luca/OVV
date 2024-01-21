CREATE DATABASE centralina;
USE centralina;
CREATE TABLE centrale(
    id INT NOT NULL AUTO_INCREMENT,
    --temperatura, umidita, pm
    temp FLOAT NOT NULL,
    umidita FLOAT NOT NULL,
    pm FLOAT NOT NULL,
    PRIMARY KEY(id)
)