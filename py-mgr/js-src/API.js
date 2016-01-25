function httpreq(method, url, onprogress, onload) {
    var xhr = new XMLHttpRequest();
    xhr.open(method, url, true);
    xhr.onprogress = onprogress;
    xhr.onload = onload;
    xhr.send(null);
}

function callApi(apiUrl, callback) {
    var noop = function() {}
    httpreq('GET', apiUrl, noop, callback);
}

class API {

    constructor() {

    }

}

export { API, callApi };
