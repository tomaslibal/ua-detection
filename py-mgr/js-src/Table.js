import { callApi } from "./API";

class TableModel {
        constructor(tableId) {
            // current page
            this.index = 0;
            // displays this many rows
            this.limit = 20;
            // keeps the items/rows
            this.items = [];
            // tableId is also the selector
            this.tableId = tableId;
        }

        getNumRows() {

        }

        getNumPages() {
            return Math.ceil(this.items.length / this.limit);
        }

        addItem(item) {
            this.items.push(item);
        }
}

class TableRowModel {

        constructor(columnSchema=[]) {
            // Array<String> columnNames: columns' schema
            // <example>
            //     ["id", "value"]
            this.columnSchema = columnSchema
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

class TableController {

    constructor(tableEl, tableName) {
        this.tableEl = tableEl;
        this.tableName = tableName;
        this.limit = 10;
        this.offset = 0;
    }

    getTableApiCall(name, limit, offset) {
        return "/table?name=" + name + "&limit=" + limit + "&offset=" + offset;
    }

    getTable(name, limit, offset, callback) {
        var url = this.getTableApiCall(name, limit, offset);
        callApi(url, callback);
    }

    populate() {
        const domTableEl = this.tableEl;

        var callback = function() {
            var response = this.responseText;
            domTableEl.innerHTML = response;
        }

        this.getTable(this.tableName, this.limit, this.offset, callback);
    }

}

export { TableModel, TableRowModel, TableController };
