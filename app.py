from flask import Flask, request, jsonify, render_template
from random import *
from pymysql import *
app = Flask(__name__)

@app.route("/")
def home():
    return render_template("index.html")

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


@app.route("/print_all/<station>")
def printall(station : str):
    db = connect(host='localhost',
                 user="root",
                 password="",
                 db="centralina")
    cur = db.cursor()
    cur.execute('SELECT * FROM ' + station)
    results = cur.fetchall()
    db.close()
    return jsonify(results), 202


@app.route("/insert_data/<station>/<temp>,<umidity>,<pm>")
def insert_data(temp, umidity, pm, station):
    ins_data = {
        "temperatura" : temp,
        "umidity" : umidity,
        "pm" : pm
    }
    extra = request.args.get("extra")

    if extra:
        ins_data["extra"] = extra

    db = connect(host='localhost',
                 user="root",
                 password="",
                 db="centralina")
    cur = db.cursor()

    #ex:
    #INSERT INTO `centrale` (`id`, `temperatura`, `umidita`, `pm`) VALUES (NULL, '28.7', '93.1', '1.1')
    cur.execute("INSERT INTO `"+station+"` (temperatura, umidita, pm) VALUES ("+ temp +"," + umidity+ "," + pm+")")
    db.commit()
    cur.close()

    return jsonify(ins_data), 200

if __name__ == '__main__':
    app.run(debug=True)