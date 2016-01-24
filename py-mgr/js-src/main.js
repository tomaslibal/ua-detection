import TableModel from "./TableModel";
import TableRowModel from "./TableRowModel";
import { TableController } from "./TableController";

var tableInstances = ['datapoints', 'labels'];

tableInstances
    .map(tableName => {
        return new TableController(document.querySelector(`#${tableName}Table`), tableName)
    })
    .forEach(tableInstance => {
        tableInstance.populate()
    });

