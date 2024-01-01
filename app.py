from flask import Flask, request, jsonify, render_template, make_response
from random import *
import subprocess as sp
from flask_mysqldb import MySQL
app = Flask(__name__)
app.config["MYSQL_HOST"] = "localhost"
app.config["MYSQL_USER"] = "root"
app.config["MYSQL_PASSWORD"] = ""
app.config["MYSQL_DB"] = "centralina"

mysql = MySQL(app)


@app.route("/")
def home():
    # out = sp.run(["php", "index.php"], stdout=sp.PIPE)
    # return out.stdout
    return render_template("from.html")


@app.route("/get-data/<temp>,<umidity>,<pm>")
def get_data(temp, umidity, pm):
    data = {
        "temperatura" : temp,
        "umidity" : umidity,
        "pm" : pm
    }
    extra = request.args.get("extra")

    if extra:
        data["extra"] = extra

    return jsonify(data), 200


@app.route("/insert-data/<temp>,<umidity>,<pm>")
def insert_data(temp, umidity, pm):
    ins_data = {
        "temperatura" : temp,
        "umidity" : umidity,
        "pm" : pm
    }
    extra = request.args.get("extra")

    if extra:
        ins_data["extra"] = extra

    cursor = mysql.connection.cursor()
    #ex:
    #INSERT INTO `centrale` (`id`, `temperatura`, `umidita`, `pm`) VALUES (NULL, '28.7', '93.1', '1.1')
    cursor.execute("INSERT INTO " + app.config["MYSQL_DB"] + " (`id`, `temperatura`, `umidita`, `pm`) VALUES (NULL,%f,%f,%f)", temp,umidity, pm)
    mysql.connection.commit()
    cursor.close()

    return "dati inseriti:\n" +jsonify(ins_data), 200

if __name__ == '__main__':
    app.run(debug=True)