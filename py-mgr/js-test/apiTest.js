import { API, APIException, CompletableFuture, __RewireAPI__ as APIModuleRewire } from '../js-src/API';

const chai = require('chai');
const assert = require('assert');

describe('API', () => {

    class MockXMLHttpRequest {
        constructor() {

        }

        open() {}
        send() {}
    }

    beforeEach(() => {

    });

    it('constructor takes a settings object', () => {
        const settings = {
            portno: 10128
        };

        const api = new API(settings);

        chai.assert.equal(api.portno, settings.portno, "port number is copied");
    });

    it('onCallDone takes a httpResponse and returns the response text', () => {
        const mockResponse = {
            responseText: 'foo bar'
        };

        const api = new API();
        const responseText = api.onCallDone(mockResponse);

        chai.assert.equal(responseText, mockResponse.responseText);
    });

    it('onCallDone throws an APIException if no response is passed', () => {
        const api = new API();
        const fThrows = () => {
            api.onCallDone();
        };

        /*
         * Somehow APIException imported into this module is not matching
         * the APIException in the API module so the following didn't work:
         * const expectedException = new APIException("No response");
         * chai.expect(fThrows).to.throw(APIException, "No response");
         */
        chai.expect(fThrows).to.throw("No response");
    });

    it('method call returns a Promise', () => {
        const api = new API();
        APIModuleRewire.__Rewire__('BrowserObjectsWrapper', {
            getXMLHttpRequest: () => { return MockXMLHttpRequest; }
        });

        const promise = api.call();

        chai.expect(promise).to.be.instanceof(api.completableFuture);
    });

});
