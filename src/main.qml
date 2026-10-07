import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: window
    width: 1024
    height: 700
    visible: true
    title: "Thread Scheduler Visualizer"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Simulation & Algorithm Controls
        GroupBox {
            title: "Simulation & Algorithm Controls"
            Layout.fillWidth: true

            RowLayout {
                spacing: 15
                anchors.fill: parent

                Label { text: "Algorithm:" }
                ComboBox {
                    id: algoSelector
                    model: ["FCFS", "Round Robin"]
                    currentIndex: 0
                    onCurrentTextChanged: guiController.currentScheduler = currentText
                }

                Label { 
                    text: "Quantum:" 
                    visible: algoSelector.currentText === "Round Robin"
                }
                SpinBox {
                    id: quantumSpinBox
                    from: 1
                    to: 10
                    value: guiController.timeQuantum
                    visible: algoSelector.currentText === "Round Robin"
                    onValueChanged: guiController.timeQuantum = value
                }

                Item { Layout.fillWidth: true } // Spacer

                Button {
                    text: guiController.isRunning ? "Pause" : "Start"
                    onClicked: {
                        if (guiController.isRunning)
                            guiController.pauseSimulation()
                        else
                            guiController.startSimulation()
                    }
                }

                Button {
                    text: "Step"
                    enabled: !guiController.isRunning
                    onClicked: guiController.stepSimulation()
                }

                Button {
                    text: "Reset"
                    onClicked: guiController.resetSimulation()
                }
            }
        }

        // The Flow Addition Panel
        GroupBox {
            title: "Add New Thread"
            Layout.fillWidth: true

            RowLayout {
                spacing: 15
                anchors.fill: parent

                Label { text: "ID:" }
                SpinBox { id: threadIdInput; from: 1; to: 99; value: 1 }

                Label { text: "Burst Time:" }
                SpinBox { id: burstTimeInput; from: 1; to: 20; value: 3 }

                Label { text: "Priority:" }
                SpinBox { id: priorityInput; from: 1; to: 10; value: 1 }

                Button {
                    text: "Add Thread"
                    onClicked: {
                        guiController.addThread(threadIdInput.value, burstTimeInput.value, priorityInput.value)
                        threadIdInput.value = threadIdInput.value + 1
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 90
            color: "#1e1e1e"
            border.color: "#333333"
            radius: 5
            clip: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 5

                Text {
                    text: "Ready Queue (Waiting Threads)"
                    color: "#ffffff"
                    font.pixelSize: 14
                    font.bold: true
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                    Row {
                        spacing: 8
                        
                        Repeater {
                            model: guiController ? guiController.readyQueue : []

                            Rectangle {
                                width: 90
                                height: 50
                                color: "#2a2a2a"
                                border.color: modelData.color
                                border.width: 2
                                radius: 4

                                Column {
                                    anchors.centerIn: parent
                                    spacing: 2
                                    Text {
                                        text: "Thread " + modelData.id
                                        color: "#ffffff"
                                        font.bold: true
                                        font.pixelSize: 11
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                    Text {
                                        text: "Burst: " + modelData.burstTime
                                        color: "#aaaaaa"
                                        font.pixelSize: 10
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Visualization display area
       Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1e1e1e"
            border.color: "#333333"
            radius: 5
            clip: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        text: "Gantt Chart Execution Timeline"
                        color: "#ffffff"
                        font.pixelSize: 16
                        font.bold: true
                    }
                    Item { Layout.fillWidth: true }
                    Text {
                        text: "Current Time: " + guiController.currentTime
                        color: "#aaaaaa"
                        font.pixelSize: 14
                    }
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                    Row {
                        id: ganttRow
                        spacing: 2
                        height: parent.height - 40

                        Repeater {
                            model: guiController.ganttBlocks

                            Rectangle {
                                width: modelData.duration * 40
                                height: 70
                                color: modelData.color
                                radius: 4
                                border.color: "#ffffff"
                                border.width: 1

                                Column {
                                    anchors.centerIn: parent
                                    Text {
                                        text: modelData.name
                                        color: "#ffffff"
                                        font.bold: true
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                    Text {
                                        text: "[" + modelData.startTime + " - " + (modelData.startTime + modelData.duration) + "]"
                                        color: "#dddddd"
                                        font.pixelSize: 10
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 180
            color: "#1e1e1e"
            border.color: "#333333"
            radius: 5
            clip: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8

                Text {
                    text: "Process / Thread Status Table"
                    color: "#ffffff"
                    font.pixelSize: 14
                    font.bold: true
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 25
                    color: "#2d2d2d"
                    radius: 3

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10

                        Text { text: "ID"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 60 }
                        Text { text: "Priority"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 80 }
                        Text { text: "State"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 100 }
                        Text { text: "Remaining / Burst"; color: "#aaa"; font.bold: true; Layout.fillWidth: true }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: guiController ? guiController.threadList : []
                    clip: true
                    spacing: 4

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 30
                        color: "#252526"
                        radius: 3
                        border.color: "#383838"

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10

                            Row {
                                Layout.preferredWidth: 60
                                spacing: 6
                                Rectangle {
                                    width: 10
                                    height: 10
                                    radius: 5
                                    color: modelData.color
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    text: "T" + modelData.id
                                    color: "#ffffff"
                                    font.bold: true
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }

                            Text {
                                text: modelData.priority
                                color: "#cccccc"
                                Layout.preferredWidth: 80
                            }

                            Rectangle {
                                Layout.preferredWidth: 90
                                height: 20
                                radius: 4
                                color: {
                                    if (modelData.state === "RUNNING") return "#2e7d32"
                                    if (modelData.state === "READY") return "#0277bd"
                                    if (modelData.state === "TERMINATED") return "#424242"
                                    return "#616161"
                                }

                                Text {
                                    text: modelData.state
                                    color: "#ffffff"
                                    font.pixelSize: 11
                                    font.bold: true
                                    anchors.centerIn: parent
                                }
                            }

                            Text {
                                text: modelData.remainingTime + " / " + modelData.burstTime + " ticks"
                                color: "#cccccc"
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }
        }
    }
}