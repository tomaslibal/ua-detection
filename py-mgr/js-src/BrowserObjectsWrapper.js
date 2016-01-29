export class BrowserObjectsWrapper {
    constructor() {
    }

    getXMLHttpRequest() {
        return XMLHttpRequest;
    }

    getCompletableFuture() {
        return Promise;
    }
}
