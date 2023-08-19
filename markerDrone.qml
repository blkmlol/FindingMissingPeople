import QtQuick 2.0
import QtPositioning 5.6
import QtLocation 5.6

MapQuickItem {
    property double latitude: 0
    property double longitude: 0
    property double distance: 0

    id: markerDroneItem
    anchorPoint.x: imageGreen.width / 2
    anchorPoint.y: imageGreen.height
    coordinate: QtPositioning.coordinate(latitude, longitude)

    sourceItem: Column {
        Image { id: imageGreen; source: "markerGreen.png" }
    }
}
