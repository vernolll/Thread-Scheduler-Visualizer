import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: stateDiagramRoot
    color: "#1e1e1e"
    border.color: "#333333"
    radius: 5

    property var threadModel: null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        Text {
            text: "Thread State Transition Diagram"
            color: "#ffffff"
            font.pixelSize: 14
            font.bold: true
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 3
            rows: 2
            rowSpacing: 10
            columnSpacing: 10

            // 1. NEW State Box
            StateBox {
                stateName: "NEW"
                stateColor: "#616161"
                threadModel: stateDiagramRoot.threadModel
                filterState: "NEW"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // 2. READY State Box
            StateBox {
                stateName: "READY"
                stateColor: "#0277bd"
                threadModel: stateDiagramRoot.threadModel
                filterState: "READY"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // 3. RUNNING State Box
            StateBox {
                stateName: "RUNNING"
                stateColor: "#2e7d32"
                threadModel: stateDiagramRoot.threadModel
                filterState: "RUNNING"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // 4. WAITING State Box
            StateBox {
                stateName: "WAITING"
                stateColor: "#ef6c00"
                threadModel: stateDiagramRoot.threadModel
                filterState: "WAITING"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // 5. TERMINATED State Box
            StateBox {
                stateName: "TERMINATED"
                stateColor: "#424242"
                threadModel: stateDiagramRoot.threadModel
                filterState: "TERMINATED"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // Пустая ячейка для сетки 3x2
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    // Внутренний компонент для контейнера конкретного состояния
    component StateBox: GroupBox {
        property string stateName: ""
        property color stateColor: "#fff"
        property var threadModel: null
        property string filterState: ""

        title: stateName
        
        background: Rectangle {
            color: "#252526"
            border.color: stateColor
            border.width: 1
            radius: 4
        }

        label: Label {
            text: stateName
            color: stateColor
            font.bold: true
            font.pixelSize: 11
            leftPadding: 4
        }

        ScrollView {
            anchors.fill: parent
            contentWidth: availableWidth
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            Flow {
                width: parent.width
                spacing: 6

                Repeater {
                    model: threadModel

                    delegate: Rectangle {
                        readonly property bool isTargetState: model.state === filterState
                        
                        width: isTargetState ? 80 : 0
                        height: isTargetState ? 40 : 0
                        visible: width > 0
                        
                        color: model.color || "#333"
                        radius: 4
                        border.color: "#ffffff"
                        border.width: 1

                        Behavior on width { NumberAnimation { duration: 250; easing.type: Easing.OutBack } }
                        Behavior on height { NumberAnimation { duration: 250; easing.type: Easing.OutBack } }

                        scale: hoverHandler.hovered ? 1.05 : 1.0
                        Behavior on scale { NumberAnimation { duration: 150 } }

                        HoverHandler { id: hoverHandler }

                        ToolTip.visible: hoverHandler.hovered
                        ToolTip.text: "Thread T" + model.threadId + "\nState: " + model.state + "\nRemaining: " + model.remainingTime

                        Column {
                            anchors.centerIn: parent
                            spacing: 2
                            Text {
                                text: "T" + model.threadId
                                color: "#ffffff"
                                font.bold: true
                                font.pixelSize: 11
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Text {
                                text: model.remainingTime + "/" + model.burstTime
                                color: "#dddddd"
                                font.pixelSize: 9
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }
            }
        }
    }
}