import TableRowModel from '../js-src/TableRowModel';

let chai = require('chai');
let assert = require('assert');

describe('TableRowModel', () => {
   
   let row;
   
   beforeEach('setup the s.u.t', () => {
      row = new TableRowModel(); 
   });
   
   it('inits with 0 columns', () => {
       chai.assert.deepEqual(row.columns, []);
   });
   
   it('computes number of columns as columns.length', () => {
       row.columns = [1,2,3];
       
       chai.assert.equal(row.getNumColumns(), 3);
   });
   
   it('adds a cell to the list of cells (columns)', () => {
        row.addColumn('foo');
        
        chai.assert.deepEqual(row.columns, ['foo']);
        
        row.addColumn('bar');
        
        chai.assert.deepEqual(row.columns, ['foo', 'bar']);
   });
   
});
