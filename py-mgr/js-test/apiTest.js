import { API, APIException } from '../js-src/API';

const chai = require('chai');
const assert = require('assert');

describe('API', () => {

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

        chai.expect(fThrows).to.throw(APIException, /No response/);
    });

});
