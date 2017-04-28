import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Item {
    property var database

    Component.onCompleted: {
        database = LocalStorage.openDatabaseSync(
                    "DictDB",
                    "1.0",
                    "Test Database",
                    1000000,
                    function(db) {
                            db.changeVersion("", "1.0");
                        });
//        database.transaction(function(tx) {
//            tx.executeSql("CREATE TABLE IF NOT EXISTS TestTable(
//                 id INTEGER PRIMARY KEY AUTOINCREMENT,
//                 datatxt TEXT)");

//            var result = tx.executeSql("SELECT COUNT(*) as count FROM TestTable").rows.item(0).count;
//            if (result === 0) {
//                console.log("Adding items to new database...")
//                tx.executeSql("INSERT INTO TestTable(datatxt) VALUES(?)",
//                              [qsTr("Test string 1")]);
//                tx.executeSql("INSERT INTO TestTable(datatxt) VALUES(?)",
//                              [qsTr("Test string 2")]);
//                console.log("Done!")
//            }
//        });
    }

    function retrieveTextEntries(callback) {
        //database = LocalStorage.openDatabaseSync("DictDB", "1.0");
        database.readTransaction(function(tx) {
            var result = tx.executeSql("SELECT * FROM TestTable");
            callback(result.rows)
        });
    }

    function getSuggestions(searchString, callback){
        //database = LocalStorage.openDatabaseSync("DictDB", "1.0");
        database.readTransaction(function(tx){
            var result = tx.executeSql("SELECT DISTINCT word, dict FROM
                         EntriesSearch WHERE word MATCH '" + searchString + "*'
                         and 1 = (SELECT isActive FROM Dicts WHERE EntriesSearch.dict = Dicts.dict)
                         ORDER BY LENGTH(word);")
            callback(result.rows)
        });
    }

    function getTranslations(word, dict, callback){
        //database = LocalStorage.openDatabaseSync("DictDB", "1.0");
        database.readTransaction(function(tx){
            var result = tx.executeSql("SELECT info FROM EntriesSearch WHERE word ==  '"
                                       + word + "' AND dict == '" + dict + "'")
            callback(result.rows)
        });
    }

    function changeDictState(dict){
        //database = LocalStorage.openDatabaseSync("DictDB", "1.0");
        database.transaction(function(tx){
            tx.executeSql("UPDATE Dicts SET isActive = CASE WHEN isActive = 0 then 1 ELSE 0 END WHERE dict == '" + dict + "';");
            //callback(result.rows)
        });

    }

    function getDicts(callback){
            //database = LocalStorage.openDatabaseSync("DictDB", "1.0");
            database.readTransaction(function(tx){
                var result = tx.executeSql("SELECT * FROM Dicts")
                callback(result.rows)
            });
        }

    function delDict(dict){
        database = LocalStorage.openDatabaseSync("DictDB", "1.0");
        database.transaction(function(tx){
            tx.executeSql("DELETE FROM Dicts WHERE dict == '" + dict + "'")
            tx.executeSql("DELETE FROM EntriesSearch WHERE dict == '" + dict + "'")
            tx.executeSql("DELETE FROM Entries WHERE dict == '" + dict + "'")

            //tx.executeSql("VACUUM;");
            //callback(result.rows)
        });
    }
}

