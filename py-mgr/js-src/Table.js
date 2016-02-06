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

        /*
         * TableRowModel item
         */
        addItem(item) {
            this.items.push(item);
        }
}

class TableRowModel {

        constructor(parentTableModel, rowData='', columnSchema=[]) {
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

class TableRowView {

    constructor(tableRowModel) {
        if (!tableRowModel) {
            throw new Error("You must pass in TableRowModel into the constructor");
            return;
        }

        this.tableRowModel = tableRowModel;
    }

    getHTML() {
        return "";
    }
}

class TableController {

    constructor(tableEl, tableName) {
        this.tableEl = tableEl;
        this.tableName = tableName;
        this.limit = 10;
        this.offset = 0;

        this.tableModel = new TableModel(tableName);
    }

    getTableApiCallWithMode(mode, name, limit, offset) {
        return "/table" + mode + "?name=" + name + "&limit=" + limit + "&offset=" + offset;
    }

    /*
     * String mode: set this value to an empty string to use the legacy
     * API endpoint which returns a formatted HTML string, or set it to
     * "_plain" to get back plain text separated by new line characters.
     */
    getTableApiCall(name, limit, offset, mode='_plain') {
        return this.getTableApiCallWithMode(mode, name, limit, offset)        
    }

    getTable(name, limit, offset, callback) {
        var url = this.getTableApiCall(name, limit, offset, '');
        callApi(url, callback);
    }

    populate() {
        const domTableEl = this.tableEl;
        const tableModel = this.tableModel;

        var callback = function() {
            var response = this.responseText;
            var lines = response.split('\n');
            lines
                .filter(line => {
                    return line.length > 0;
                })
                .forEach(line => {
                    tableModel.addItem(new TableRowModel(tableModel, line, ["id", "value"]));
                });

            domTableEl.innerHTML = response;
        }

        this.getTable(this.tableName, this.limit, this.offset, callback);
    }

}

export { TableModel, TableRowModel, TableRowView, TableController };
