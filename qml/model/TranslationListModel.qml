import QtQuick 2.0

ListModel{
    function addDictEntry(entry, dict) {
        append({
                   entry: entry,
                   dict: dict
               });
    }

//    function updateDictEntry(index, datatxt) {
//        set(index, {datatxt: datatxt})
//    }
}
