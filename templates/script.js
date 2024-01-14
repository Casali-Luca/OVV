document.addEventListener('DOMContentLoaded', function() {
    function fetchData(apiName, params) {
        let url = new URL('/api/' + apiName, window.location.origin);
        Object.keys(params).forEach(key => url.searchParams.append(key, params[key]));
        return fetch(url)
            .then(response => response.json())
            .catch(error => {
                console.error(`Errore durante la richiesta API ${apiName}:`, error);
                return null;
            });
    }

    function updateValue(apiName, elementId, params) {
        fetchData(apiName, params)
            .then(data => {
                if (data !== null) {
                    document.getElementById(elementId).innerText = data.value;
                    applyStyles(data.value, elementId);
                }
            });
    }

    function updateValues() {
        updateValue('weather', 'weatherValue', params);
        updateValue('airQuality', 'airQualityValue', params);
        updateValue('humidity', 'humidityValue', params);
        updateValue('temperature', 'temperatureValue', params);
        updateValue('light', 'lightValue', params);

        setTimeout(updateValues, 90000);
    }

    function applyStyles(value, elementId) {
        let element = document.getElementById(elementId);

        element.parentElement.classList.remove('ottimale', 'leggermente-alto', 'basso', 'alto');

        //Parametri da modificare
        if (value >= 70 && value <= 100) {
            element.parentElement.classList.add('ottimale');
        } else if (value > 50 && value < 70) {
            element.parentElement.classList.add('leggermente-alto');
        } else if (value >= 0 && value <= 50) {
            element.parentElement.classList.add('basso');
        } else {
            element.parentElement.classList.add('alto');
        }
    }

    updateValues();
});
