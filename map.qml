import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15
import "dynamicMarkerCreation.js" as DMC

Rectangle{
    property int number: 1
    property int numberId: 1
    property double startLatitude: 59.9
    property double startLongitude: 30.5
    property double latitude: 0
    property double longitude: 0
    property double droneLatitude: 0
    property double droneLongitude: 0
    property bool isSetMarkerButton: false
    property bool isCreated: false
    property bool isDronePlaced: false
    property var sprite : []
    property var spriteDrone : []
    property var copyPath

    PositionSource {
        id: positionSource
        updateInterval: 1000
        active: true

        onPositionChanged: {
            var coord = positionSource.position.coordinate
            console.log("Coordinate:", coord.longitude, coord.latitude)
            startLongitude = coord.longitude
            startLatitude = coord.latitude
            copyPath[0].latitide = startLatitude
            copyPath[0].longitude = startLongitude
            mapPolyline.path = copyPath
        }
    }


    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(startLatitude, startLongitude)
        zoomLevel: 14

        MapQuickItem {
            id: marker
            coordinate: QtPositioning.coordinate(startLatitude, startLongitude)
            anchorPoint.x: image.width/2
            anchorPoint.y: image.height
            sourceItem: Column {
                Image { id: image; source: "marker.png" }
                Text { text: "  "+number; font.bold: true; }
            }
        }

        MouseArea {
            anchors.fill: parent
            onDoubleClicked:
            {
                if (isSetMarkerButton)
                {
                    if (isCreated)
                    {
                        clearLastWaypoint()
                    }
                    numberId++
                    var target_coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                    latitude = target_coord.latitude
                    longitude = target_coord.longitude
                    var distance = 50
                    var newSpriteDrone = DMC.createSprite(map, distance, latitude, longitude, numberId, false)
                    sprite.push(newSpriteDrone)
                    map.addMapItem(newSpriteDrone)
                    mapPolyline.addCoordinate(QtPositioning.coordinate(latitude, longitude))
                    console.log("Added point #", mapPolyline.pathLength(), "Lat: ", latitude, "Long: ", longitude)
                    copyPath = mapPolyline.path
                    isCreated = true
                }
            }
        }

        MapPolyline
        {
            id:mapPolyline
            line.width: 3
            line.color: 'red'
            path: [
                QtPositioning.coordinate(startLatitude, startLongitude),
            ]
        }
    }

    function setMarkerButton(isSMB)
    {
        isSetMarkerButton = isSMB
    }

    function clearLastWaypoint()
    {
        if (sprite.length > 0)
        {
            var lastPoint = copyPath[copyPath.length - 1]
            console.log("Removed point #", mapPolyline.pathLength(), "Lat: ", lastPoint.latitude, "Long: ", lastPoint.longitude)
            map.removeMapItem(sprite.pop())
            mapPolyline.removeCoordinate(numberId - 1)
            numberId--
            copyPath = mapPolyline.path
        }
    }

    function clearDroneLocation()
    {
        if (spriteDrone.length > 0)
        {
            map.removeMapItem(spriteDrone.pop())
        }
    }

    function placeDrone()
    {
        clearDroneLocation()
        var distance = 50
        var newSpriteDrone = DMC.createSprite(map, distance, droneLatitude, droneLongitude, -1, true)
        spriteDrone.push(newSpriteDrone)
        map.addMapItem(newSpriteDrone)
        isDronePlaced = true
    }

    Component.onDestruction: {
        positionSource.active = false
        mapPolyline.path = []
    }
}
