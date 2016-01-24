import { TableController } from "./Table";

var tableInstances = ['datapoints', 'labels'];

tableInstances
    .map(tableName => {
        return new TableController(document.querySelector(`#${tableName}Table`), tableName)
    })
    .forEach(tableInstance => {
        tableInstance.populate()
    });

