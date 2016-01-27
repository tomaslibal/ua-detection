import TableModel from '../js-src/Table';

let chai = require('chai');
let assert = require('assert');

describe('TableModel', () => {
   
   let table;
   
   beforeEach('setup the s.u.t', () => {
      table = new TableModel(); 
   });
   
   it('inits with 0 rows, index = 0', () => {
       chai.assert.equal(table.index, 0);
       chai.assert.equal(table.items.length, 0);
   });
   
   it('default pagination is 20', () => {
       chai.assert.equal(table.limit, 20); 
   });
   
   it('computes number of pages as ceil(items.length / limit)', () => {
        table.items = [1,2,3,4,5,6,7,8,9];
        table.limit = 2;
        
        chai.assert.equal(table.getNumPages(), 5);
   });
   
   it('adds an item to the list of items', () => {
       table.addItem('foo');
       
       chai.assert.deepEqual(table.items, ['foo']);
       
       table.addItem('bar');
       
       chai.assert.deepEqual(table.items, ['foo', 'bar']);
   });
    
});

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
