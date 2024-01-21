document.addEventListener('DOMContentLoaded', function() {
    // Funzione per recuperare dati dalle API
    function fetchData(apiName, params) {
        // Indirizzo del server Flask che fornisce le API
        let serverAddress = 'http://localhost:5000';
        
        // Costruzione dell'URL per la richiesta API
        let url = new URL(`/api/${apiName}`, serverAddress);
        
        // Aggiunta dei parametri all'URL della richiesta
        Object.keys(params).forEach(key => url.searchParams.append(key, params[key]));

        // Effettua la richiesta API
        return fetch(url)
            .then(response => {
                // Controlla se la richiesta ha avuto successo (status code 2xx)
                if (!response.ok) {
                    throw new Error(`Errore durante la richiesta API ${apiName}: ${response.status} ${response.statusText}`);
                }

                return response.text();
            })
            .catch(error => {
                console.error(error);
                return null;
            });
    }

    function updateValue(apiName, elementId, params) {
        // Recupera i dati dalle API
        fetchData(apiName, params)
            .then(data => {
                if (data !== null) {
                    // Assegna il testo HTML direttamente all'elemento senza parsare JSON
                    document.getElementById(elementId).innerHTML = data;
                }
            });
    }

    function updateValues() {
        // Aggiorna i valori per ciascuna API
        updateValue('weather', 'weatherValue', {});
        updateValue('airQuality', 'airQualityValue', {});
        updateValue('humidity', 'humidityValue', {});
        updateValue('temperature', 'temperatureValue', {});
        updateValue('light', 'lightValue', {});

        // Aggiorna i valori ogni 90 secondi
        setTimeout(updateValues, 90000);
    }

    // Inizia il processo di aggiornamento dei valori quando il DOM è completamente caricato
    updateValues();
});
