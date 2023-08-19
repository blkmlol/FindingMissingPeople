import QtQuick 2.0
import QtPositioning 5.6
import QtLocation 5.6

MapQuickItem {
    property double latitude: 0
    property double longitude: 0
    property double distance: 0
    property double markerId: 0

    id: markerItem
    anchorPoint.x: image.width / 2
    anchorPoint.y: image.height
    coordinate: QtPositioning.coordinate(latitude, longitude)

    sourceItem: Column {
        Image { id: image; source: "marker2.png" }
        Text { text: " " + markerId; font.bold: true; }
    }
}
