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

class BaseView {
    constructor() {}
    render() {}
}

class TableModelView extends BaseView {
    constructor(tableModel) {
        super();

        this.tableModel = tableModel;
    }

    getTableHeader() {
        return '<div><span>Id</span><span>Value</span><span>...</span><span>Actions</span></div>';
    }

    render() {
        // render table header
        const tableHeader = this.getTableHeader();
        // table rows are constructed from the tableRowModels
        let tableRows = this.tableModel.items
                                .map(rowModel => {
                                    return new TableRowView(rowModel).render();
                                })
                                .reduce((prev, current, idx) => {
                                    return prev + current.render();
                                });
        return tableHeader + tableRows;
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

class TableRowView extends BaseView {

    constructor(tableRowModel) {
        super();

        if (!tableRowModel) {
            throw new Error("You must pass in TableRowModel into the constructor");
            return;
        }

        this.tableRowModel = tableRowModel;
    }

    render() {
        
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

    render() {
        const tableView = new TableModelView(this.tableModel);
        tableView.render();
    }
}

export { TableModel, TableRowModel, TableRowView, TableController };
