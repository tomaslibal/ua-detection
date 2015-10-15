export default class TableRowModel {
        
        constructor() {
            // each column holds one cell
            this.columns = [];
        }
        
        getNumColumns() {
            return this.columns.length;
        }
        
        addColumn(cell) {
            return this.columns.push(cell);
        }
       
        
}


