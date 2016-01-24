import TableModel from "./TableModel";
import TableRowModel from "./TableRowModel";
import { callApi } from "./API";

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

export { TableController };
