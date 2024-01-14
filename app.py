from flask import Flask, request, jsonify, render_template
from random import *
from pymysql import *
app = Flask(__name__)

@app.route("/")
def home():
    return render_template("index.html",)

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
def insert_data(temp :float, umidity:float, pm:float, station:str):
    
    if(station !=  "centrale"):
        return render_template("error.html")
    
    ins_data = {
        "temperatura" : temp,
        "umidity" : umidity,
        "pm" : pm
    }
    extra = request.args.get("extra")

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


@app.route('/api/<api_name>')
def get_api_value(api_name):
    try:
        value = get_api_data(api_name)
        return jsonify({'value': value})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

def get_api_data(api_name):
    db = connect(host='localhost',
                    user="root",
                    password="",
                    db="centralina")
    cur = db.cursor()    
    
    var = ""
    if api_name == 'weather':
        ...
    elif api_name == 'airQuality':
        var = 'pm'
    elif api_name == 'humidity':
        var = 'umidita'
    elif api_name == 'temperature':
        var = 'temperatura'
    elif api_name == 'light':
        ...

    sql = "SELECT "+ var +" FROM centrale"
    cur.execute(sql)
    
    results = cur.fetchall()
    
    db.commit()
    cur.close()
    return results 

if __name__ == '__main__':
    app.run(debug=True)

