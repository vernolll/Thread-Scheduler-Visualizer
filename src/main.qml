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

        // Visualization display area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1e1e1e"
            border.color: "#333333"
            radius: 5

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10

                Text {
                    text: "Gantt Chart & Queue Visualization Placeholder"
                    color: "#888888"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    text: "Active Algorithm: " + guiController.currentScheduler
                    color: "#aaaaaa"
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}