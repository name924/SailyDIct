import QtQuick 2.0

ListModel {

    function addTestEntry(id, datatxt) {
        append({
                   id: id,
                   datatxt: datatxt
               });
    }

    function updateTextEntry(index, datatxt) {
        set(index, {datatxt: datatxt})
    }
}
