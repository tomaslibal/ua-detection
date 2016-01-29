/**
 * 
 * API Module
 *
 * Defines API class and others which abstract calls to the HTTP endpoints
 * of the api.
 *
 */

import { BrowserObjectsWrapper } from './BrowserObjectsWrapper';

/*
 * @private
 *
 */
function httpreq(method, url, onprogress, onload) {
    var xhrClass = BrowserObjectsWrapper.getXMLHttpRequest();
    var xhr = new xhrClass();
    xhr.open(method, url, true);
    xhr.onprogress = onprogress;
    xhr.onload = onload;
    xhr.send(null);
}

/*
 * @private
 *
 */
function callApi(apiUrl, callback) {
    var noop = function() {}
    httpreq('GET', apiUrl, noop, () => {
        callback(this);
    });
}

class APIException extends Error {
    constructor(msg) {
        super(msg);
        this.name = "APIException";
        this.message = msg;
        this.stack = (new Error()).stack;
    }
}

class NanoPromise {
    constructor() {
        this.state = 'pending';
        this.thenCallbacks = [];
    }

    then(callback) {
        this.thenCallbacks.push(callback);
    }

    resolve(data) {
        this.state = 'resolved';
        this.thenCallbacks.forEach(callback => {
            callback(data);
        });
    }
}

/*
 * Makes HTTP requests to the given endpoint.
 *
 * <example>
 *     const api = new API({ portno: 7000 });
 *     const getSomeTable = api.call('/someTable');
 *
 *     getSomeTable.then(data => processData);
 * </example>
 */
class API {

    constructor(settings={}) {
        this.portno = settings.portno || '';
        this.completableFuture = NanoPromise;
    }

    onCallDone(httpResponse) {
        if (!httpResponse) {
            throw new APIException("No response");
        }

        return httpResponse.responseText;
    }

    call(urlPath='/') {
        const callPromise = new this.completableFuture();

        callApi(urlPath, (httpResponse) => {
            const finalize = this.onCallDone(httpResponse);
            callPromise.resolve(finalize);
        });

        return callPromise;
    }
}

export { API, callApi, APIException, NanoPromise };
