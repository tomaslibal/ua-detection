export default class TableModel {
        constructor() {
            // current page
            this.index = 0;
            // displays this many rows
            this.limit = 20;
            // keeps the items/rows
            this.items = [];
        }
        
        getNumPages() {
            return Math.ceil(this.items.length / this.limit);
        }
        
        addItem(item) {
            this.items.push(item);
        }
}


