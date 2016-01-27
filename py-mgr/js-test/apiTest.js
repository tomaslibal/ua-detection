import { API } from '../js-src/API';

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

});
