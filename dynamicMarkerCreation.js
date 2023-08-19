function createSprite(map, distance, latitude, longitude, numberId, isDrone) {
    var component;
    if (isDrone){
        component = Qt.createComponent("markerDrone.qml");
    } else {
        component = Qt.createComponent("marker.qml");
    }
    var sprite = component.createObject(map, {distance: distance, latitude:latitude, longitude:longitude, markerId:numberId});
    return sprite;
}
