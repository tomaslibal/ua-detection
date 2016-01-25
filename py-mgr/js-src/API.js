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

class APIException extends Error {
    constructor(msg) {
        super(msg);
        this.name = "APIException";
        this.message = msg;
        this.stack = (new Error()).stack;
    }
}

class API {

    constructor() {

    }

    getTableRowCount(tableName) {
        if (!tableName) {
            throw new APIException("no table name given for getTableRowCount");
        }
    }
}

export { API, callApi };
