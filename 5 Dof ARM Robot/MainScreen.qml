import QtQuick
import QtQuick3D
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: mainScreen
    width: 1280
    height: 720
    property bool isDarkMode: true  // Default to dark mode

    // 3D View
    View3D {
        id: view3D
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: controlPanel.left
        anchors.bottom: parent.bottom
        anchors.margins: 10

        environment: SceneEnvironment {
            clearColor: isDarkMode ? "#1a1a1a" : "#f0f0f0"
            backgroundMode: SceneEnvironment.Color
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
        }

        // Camera
        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(330, 660, 330)
            eulerRotation.x: -25
            eulerRotation.y: 35
        }

        // Lighting
        DirectionalLight {
            id: mainLight
            eulerRotation.x: -35
            eulerRotation.y: -60
            brightness: isDarkMode ? 1.2 : 1.5
            castsShadow: true
        }

        DirectionalLight {
            id: fillLight
            eulerRotation.x: -20
            eulerRotation.y: 120
            brightness: isDarkMode ? 0.6 : 0.9
        }

        DirectionalLight {
            id: ambientLight
            eulerRotation.x: 90
            brightness: isDarkMode ? 0.3 : 0.5
        }

        // Work surface
        Model {
            source: "#Cube"
            scale: Qt.vector3d(6, 0.03, 6)
            y: -0.6
            materials: PrincipledMaterial {
                baseColor: isDarkMode ? "#2c2c2c" : "#e0e0e0"
                metalness: 0.1
                roughness: 0.7
            }
        }

        // Grid lines
        Model {
            source: "#Cube"
            scale: Qt.vector3d(0.01, 0.01, 6)
            y: -0.58
            materials: PrincipledMaterial {
                baseColor: isDarkMode ? "#404040" : "#b0b0b0"
                roughness: 1.0
            }
        }

        Model {
            source: "#Cube"
            scale: Qt.vector3d(6, 0.01, 0.01)
            y: -0.58
            materials: PrincipledMaterial {
                baseColor: isDarkMode ? "#404040" : "#b0b0b0"
                roughness: 1.0
            }
        }

        // Robotic Arm
        RoboticArm {
            id: newRoboticArm
            Behavior on joint1Rotation {
                PropertyAnimation { duration: 1500; easing.type: Easing.InOutCubic }
            }
            Behavior on joint2Rotation {
                PropertyAnimation { duration: 1500; easing.type: Easing.InOutCubic }
            }
            Behavior on joint3Rotation {
                PropertyAnimation { duration: 1500; easing.type: Easing.InOutCubic }
            }
            Behavior on joint4Rotation {
                PropertyAnimation { duration: 1500; easing.type: Easing.InOutCubic }
            }
            Behavior on joint5Rotation {
                PropertyAnimation { duration: 1500; easing.type: Easing.InOutCubic }
            }
            Behavior on joint6Rotation {
                PropertyAnimation { duration: 1500; easing.type: Easing.InOutCubic }
            }
            Behavior on gripperAngle {
                PropertyAnimation { duration: 800; easing.type: Easing.InOutQuad }
            }
        }
    }

    // Control Panel
    Rectangle {
        id: controlPanel
        width: 320
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10
        color: isDarkMode ? "#2b2b2b" : "#ffffff"
        border.color: isDarkMode ? "#555555" : "#cccccc"
        border.width: 2
        radius: 8

        ScrollView {
            anchors.fill: parent
            anchors.margins: 15
            clip: true

            ColumnLayout {
                width: controlPanel.width - 30
                spacing: 20

                // Theme Toggle
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Text {
                        text: "Theme:"
                        color: isDarkMode ? "#ffffff" : "#000000"
                        font.pixelSize: 12
                    }
                    Button {
                        id: themeToggle
                        text: isDarkMode ? "☀️ Light" : "🌙 Dark"
                        onClicked: isDarkMode = !isDarkMode
                        background: Rectangle {
                            color: isDarkMode ? "#333333" : "#dddddd"
                            radius: 4
                        }
                        contentItem: Text {
                            text: parent.text
                            color: isDarkMode ? "#ffffff" : "#000000"
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }

                // Title
                Text {
                    text: "6-DOF Robotic Arm Control"
                    color: isDarkMode ? "#ffffff" : "#000000"
                    font.pixelSize: 20
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                // Status
                Rectangle {
                    Layout.fillWidth: true
                    height: 30
                    color: isDarkMode ? "#1e3a5f" : "#d8e6f3"
                    radius: 4
                    Text {
                        anchors.centerIn: parent
                        text: "System Ready"
                        color: "#4CAF50"
                        font.pixelSize: 14
                    }
                }

                // Joint Controls
                GroupBox {
                    title: "Joint Controls"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    background: Rectangle {
                        color: isDarkMode ? "#383838" : "#f5f5f5"
                        border.color: isDarkMode ? "#555555" : "#cccccc"
                        radius: 6
                    }
                    label: Text {
                        color: isDarkMode ? "#ffffff" : "#000000"
                        text: parent.title
                        font.bold: true
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 12

                        // Joint 1
                        RowLayout {
                            Text {
                                text: "Base Rotation:"
                                color: isDarkMode ? "#ffffff" : "#000000"
                                width: 80
                                font.pixelSize: 11
                            }
                            Slider {
                                id: joint1Slider
                                from: -180; to: 180; value: 0
                                Layout.fillWidth: true
                                onValueChanged: newRoboticArm.joint1Rotation = value
                                background: Rectangle {
                                    color: isDarkMode ? "#555555" : "#cccccc"
                                    radius: 3
                                }
                            }
                            Text {
                                text: joint1Slider.value.toFixed(0) + "°"
                                color: "#4CAF50"
                                width: 35
                                font.pixelSize: 11
                            }
                        }

                        // Joint 2
                        RowLayout {
                            Text {
                                text: "Shoulder:"
                                color: isDarkMode ? "#ffffff" : "#000000"
                                width: 80
                                font.pixelSize: 11
                            }
                            Slider {
                                id: joint2Slider
                                from: -90; to: 90; value: 0
                                Layout.fillWidth: true
                                onValueChanged: newRoboticArm.joint2Rotation = value
                                background: Rectangle {
                                    color: isDarkMode ? "#555555" : "#cccccc"
                                    radius: 3
                                }
                            }
                            Text {
                                text: joint2Slider.value.toFixed(0) + "°"
                                color: "#4CAF50"
                                width: 35
                                font.pixelSize: 11
                            }
                        }

                        // Joint 3
                        RowLayout {
                            Text {
                                text: "Elbow:"
                                color: isDarkMode ? "#ffffff" : "#000000"
                                width: 80
                                font.pixelSize: 11
                            }
                            Slider {
                                id: joint3Slider
                                from: -135; to: 135; value: 0
                                Layout.fillWidth: true
                                onValueChanged: newRoboticArm.joint3Rotation = value
                                background: Rectangle {
                                    color: isDarkMode ? "#555555" : "#cccccc"
                                    radius: 3
                                }
                            }
                            Text {
                                text: joint3Slider.value.toFixed(0) + "°"
                                color: "#4CAF50"
                                width: 35
                                font.pixelSize: 11
                            }
                        }

                        // Joint 4
                        RowLayout {
                            Text {
                                text: "Wrist Roll:"
                                color: isDarkMode ? "#ffffff" : "#000000"
                                width: 80
                                font.pixelSize: 11
                            }
                            Slider {
                                id: joint4Slider
                                from: -180; to: 180; value: 0
                                Layout.fillWidth: true
                                onValueChanged: newRoboticArm.joint4Rotation = value
                                background: Rectangle {
                                    color: isDarkMode ? "#555555" : "#cccccc"
                                    radius: 3
                                }
                            }
                            Text {
                                text: joint4Slider.value.toFixed(0) + "°"
                                color: "#4CAF50"
                                width: 35
                                font.pixelSize: 11
                            }
                        }

                        // Joint 5
                        RowLayout {
                            Text {
                                text: "Wrist Pitch:"
                                color: isDarkMode ? "#ffffff" : "#000000"
                                width: 80
                                font.pixelSize: 11
                            }
                            Slider {
                                id: joint5Slider
                                from: -90; to: 90; value: 0
                                Layout.fillWidth: true
                                onValueChanged: newRoboticArm.joint5Rotation = value
                                background: Rectangle {
                                    color: isDarkMode ? "#555555" : "#cccccc"
                                    radius: 3
                                }
                            }
                            Text {
                                text: joint5Slider.value.toFixed(0) + "°"
                                color: "#4CAF50"
                                width: 35
                                font.pixelSize: 11
                            }
                        }
                    }
                }

                // Preset Positions
                GroupBox {
                    title: "Preset Positions"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    background: Rectangle {
                        color: isDarkMode ? "#383838" : "#f5f5f5"
                        border.color: isDarkMode ? "#555555" : "#cccccc"
                        radius: 6
                    }
                    label: Text {
                        color: isDarkMode ? "#ffffff" : "#000000"
                        text: parent.title
                        font.bold: true
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Button {
                            text: "🏠 Home Position"
                            Layout.fillWidth: true
                            font.pixelSize: 12
                            onClicked: {
                                joint1Slider.value = 0
                                joint2Slider.value = 0
                                joint3Slider.value = 0
                                joint4Slider.value = 0
                                joint5Slider.value = 0
                            }
                            background: Rectangle {
                                color: parent.pressed ? "#4CAF50" : "#388E3C"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }

                        Button {
                            text: "🎯 Pick Position"
                            Layout.fillWidth: true
                            font.pixelSize: 12
                            onClicked: {
                                joint1Slider.value = 0
                                joint2Slider.value = -45
                                joint3Slider.value = 90
                                joint4Slider.value = 0
                                joint5Slider.value = -45
                            }
                            background: Rectangle {
                                color: parent.pressed ? "#2196F3" : "#1976D2"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }

                        Button {
                            text: "💤 Rest Position"
                            Layout.fillWidth: true
                            font.pixelSize: 12
                            onClicked: {
                                joint1Slider.value = 0
                                joint2Slider.value = 75
                                joint3Slider.value = -110
                                joint4Slider.value = 0
                                joint5Slider.value = 35
                            }
                            background: Rectangle {
                                color: parent.pressed ? "#FF9800" : "#F57C00"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }

                // Camera Controls
                GroupBox {
                    title: "Camera Views"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    background: Rectangle {
                        color: isDarkMode ? "#383838" : "#f5f5f5"
                        border.color: isDarkMode ? "#555555" : "#cccccc"
                        radius: 6
                    }
                    label: Text {
                        color: isDarkMode ? "#ffffff" : "#000000"
                        text: parent.title
                        font.bold: true
                    }

                    GridLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        columns: 2
                        rowSpacing: 8
                        columnSpacing: 8

                        Button {
                            text: "Back"
                            onClicked: {
                                camera.position = Qt.vector3d(642.4, 160.6, 0)
                                camera.eulerRotation = Qt.vector3d(0, 90, 0)
                            }
                            background: Rectangle {
                                color: parent.pressed ? "#455A64" : "#37474F"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }

                        Button {
                            text: "Top"
                            onClicked: {
                                camera.position = Qt.vector3d(0, 693.9, 0.231)
                                camera.eulerRotation = Qt.vector3d(-90, 0, 0)
                            }
                            background: Rectangle {
                                color: parent.pressed ? "#455A64" : "#37474F"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }

                        Button {
                            text: "Side"
                            onClicked: {
                                camera.position = Qt.vector3d(0, 160, 642)
                                camera.eulerRotation = Qt.vector3d(0, 0, 0)
                            }
                            background: Rectangle {
                                color: parent.pressed ? "#455A64" : "#37474F"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }

                        Button {
                            text: "Default"
                            onClicked: {
                                camera.position = Qt.vector3d(-693.9, 347.0, 0)
                                camera.eulerRotation = Qt.vector3d(0, 90, 0)
                            }
                            background: Rectangle {
                                color: parent.pressed ? "#455A64" : "#37474F"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }

                // Zoom Controls
                GroupBox {
                    title: "Zoom & Scale"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    background: Rectangle {
                        color: isDarkMode ? "#383838" : "#f5f5f5"
                        border.color: isDarkMode ? "#555555" : "#cccccc"
                        radius: 6
                    }
                    label: Text {
                        color: isDarkMode ? "#ffffff" : "#000000"
                        text: parent.title
                        font.bold: true
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Button {
                            text: "🔍 Zoom In"
                            Layout.fillWidth: true
                            onClicked: camera.position = camera.position.times(0.85)
                            background: Rectangle {
                                color: parent.pressed ? "#689F38" : "#558B2F"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }

                        Button {
                            text: "🔍 Zoom Out"
                            Layout.fillWidth: true
                            onClicked: camera.position = camera.position.times(1.15)
                            background: Rectangle {
                                color: parent.pressed ? "#F57C00" : "#EF6C00"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }
            }
        }
    }
}
