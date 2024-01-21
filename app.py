from flask import Flask, request, jsonify, render_template
from random import *
from pymysql import *
import python_weather
import asyncio

app = Flask(__name__)
#semplice funzionamento delle api del meteo (WIP)
async def get_weather():
    nomecity = "Bergamo"
    #dopo aver scelto la città, andiamo a richiamare le api del meteo tramite la classe presente in pythonWeather "client"
    async with python_weather.Client(unit=python_weather.IMPERIAL) as client:
        weather = await client.get(f"{nomecity}")
        #ritornerà una hash formata da ogni 3 ore con la relativa condizione metereologica
        for forecast in weather.forecasts:
            for hourly in forecast.hourly:
                print(f' --> {hourly!r}')
            break
#route di partenza
@app.route("/")
def home():
    #renderizza l'index.html (WIP)
    return render_template("index.html",)

#va a stampare tutti i valori presenti in una tabella a scelta del db
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

#inserimento data da parte dell'arduino o qualsiasi richiesta di pull
@app.route("/insert_data/<station>/<temp>,<umidity>,<pm>,<luce>")
def insert_data(temp :float, umidity:float, pm:float, luce:float, station:str):
    
    if(station !=  "centrale"):
        return render_template("error.html")
    
    ins_data = {
        "temperatura" : temp,
        "umidity" : umidity,
        "pm" : pm,
        "luce" : luce
    }
    extra = request.args.get("extra")

    db = connect(host='localhost',
                 user="root",
                 password="",
                 db="centralina")
    #creazione cursore (indirizzerà la query)
    cur = db.cursor()

    #ex:
    #INSERT INTO `centrale` (`id`, `temperatura`, `umidita`, `pm`) VALUES (NULL, '28.7', '93.1', '1.1')
    cur.execute("INSERT INTO `"+station+"` (id, temperatura, luce, umidita, pm) VALUES (NULL,"+ temp +"," + luce +","+ umidity+ "," + pm+")")
    db.commit()
    cur.close()
    #ritorno del json di ciò che abbiamo appena inserito
    return jsonify(ins_data), 200

#directory adibita alla chiamata API
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
    #da ciò che si inserisce nell'URL restituirà il select dalla centrale
    var = ""
    if api_name == 'weather':
        return ...
    elif api_name == 'airQuality':
        var = 'pm'
    elif api_name == 'humidity':
        var = 'umidita'
    elif api_name == 'temperature':
        var = 'temperatura'
    elif api_name == 'light':
        var = "luce"

    sql = "SELECT "+ var +" FROM centrale"
    cur.execute(sql)
    
    results = cur.fetchall()
    
    db.commit()
    cur.close()
    return results 

if __name__ == '__main__':
    app.run(debug=True)

