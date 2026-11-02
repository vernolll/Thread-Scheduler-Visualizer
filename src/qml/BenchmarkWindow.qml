import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: benchmarkPopup
    modal: true
    focus: true
    width: 960
    height: 640
    x: parent ? (parent.width - width) / 2 : 100
    y: parent ? (parent.height - height) / 2 : 100
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#f5f5f5"
        border.color: (guiController && guiController.isDarkMode) ? "#444444" : "#cccccc"
        border.width: 1
        radius: 8
    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        // Header
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Comparative Algorithm Analysis Dashboard"
                color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                font.pixelSize: 18
                font.bold: true
                Layout.fillWidth: true
            }

            Button {
                id: closeBtn
                text: "✕"
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                background: Rectangle {
                    color: closeBtn.hovered ? "#d32f2f" : "transparent"
                    radius: 4
                }
                contentItem: Text {
                    text: "✕"
                    color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: benchmarkPopup.close()
            }
        }

        // Section 1: Side-by-Side Summary Table
        GroupBox {
            title: "Performance Summary Metrics Table"
            Layout.fillWidth: true
            Layout.preferredHeight: 250

            background: Rectangle {
                color: (guiController && guiController.isDarkMode) ? "#252526" : "#ffffff"
                border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
                radius: 4
            }

            label: Label {
                text: parent.title
                color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                font.bold: true
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 6

                Rectangle {
                    Layout.fillWidth: true
                    height: 28
                    color: (guiController && guiController.isDarkMode) ? "#2d2d2d" : "#e0e0e0"
                    radius: 3

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10

                        Text { text: "Algorithm"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 110 }
                        Text { text: "Ticks"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 60 }
                        Text { text: "Throughput"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 95 }
                        Text { text: "Context Sw."; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 95 }
                        Text { text: "Fairness (Jain)"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 110 }
                        Text { text: "Avg Waiting"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 95 }
                        Text { text: "Avg Turnaround"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.fillWidth: true }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: guiController ? guiController.benchmarkResultsModel : null
                    clip: true
                    spacing: 4

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 32
                        color: (guiController && guiController.isDarkMode) ? "#2a2a2a" : "#f9f9f9"
                        radius: 3
                        border.color: (guiController && guiController.isDarkMode) ? "#383838" : "#e0e0e0"

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10

                            Text { text: model.schedulerName; color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"; font.bold: true; Layout.preferredWidth: 110 }
                            Text { text: model.totalTicks; color: (guiController && guiController.isDarkMode) ? "#cccccc" : "#333333"; Layout.preferredWidth: 60 }
                            Text { text: Number(model.throughput).toFixed(4); color: "#4caf50"; font.bold: true; Layout.preferredWidth: 95 }
                            Text { text: model.contextSwitches; color: (guiController && guiController.isDarkMode) ? "#cccccc" : "#333333"; Layout.preferredWidth: 95 }
                            Text { text: Number(model.jainsFairness).toFixed(3); color: "#2196f3"; font.bold: true; Layout.preferredWidth: 110 }
                            Text { text: Number(model.avgWaiting).toFixed(1); color: (guiController && guiController.isDarkMode) ? "#cccccc" : "#333333"; Layout.preferredWidth: 95 }
                            Text { text: Number(model.avgTurnaround).toFixed(1); color: "#ff9800"; font.bold: true; Layout.fillWidth: true }
                        }
                    }
                }
            }
        }

        // Section 2: Visual Comparative Bar Charts Dashboard
        GroupBox {
            title: "Comparative Visual Metrics (Throughput & Fairness)"
            Layout.fillWidth: true
            Layout.fillHeight: true

            background: Rectangle {
                color: (guiController && guiController.isDarkMode) ? "#252526" : "#ffffff"
                border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
                radius: 4
            }

            label: Label {
                text: parent.title
                color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                font.bold: true
            }

            RowLayout {
                anchors.fill: parent
                spacing: 20

                // Throughput Bar Chart Preview
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#fafafa"
                    border.color: (guiController && guiController.isDarkMode) ? "#333" : "#ddd"
                    radius: 4

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Text {
                            text: "Throughput Comparison"
                            color: (guiController && guiController.isDarkMode) ? "#aaaaaa" : "#444444"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                            Column {
                                width: parent.width
                                spacing: 8

                                Repeater {
                                    model: guiController ? guiController.benchmarkResultsModel : null

                                    RowLayout {
                                        width: parent.width
                                        spacing: 10

                                        Text {
                                            text: model.schedulerName
                                            color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                                            font.pixelSize: 11
                                            Layout.preferredWidth: 80
                                        }

                                        Rectangle {
                                            Layout.fillWidth: true
                                            height: 16
                                            color: (guiController && guiController.isDarkMode) ? "#333333" : "#e0e0e0"
                                            radius: 3

                                            Rectangle {
                                                width: Math.min(parent.width, parent.width * (model.throughput * 2.0)) // Нормализация для визуализации
                                                height: parent.height
                                                color: "#4caf50"
                                                radius: 3
                                            }
                                        }

                                        Text {
                                            text: Number(model.throughput).toFixed(3)
                                            color: "#4caf50"
                                            font.pixelSize: 11
                                            font.bold: true
                                            Layout.preferredWidth: 50
                                            horizontalAlignment: Text.AlignRight
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Jain's Fairness Bar Chart Preview
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#fafafa"
                    border.color: (guiController && guiController.isDarkMode) ? "#333" : "#ddd"
                    radius: 4

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Text {
                            text: "Jain's Fairness Index"
                            color: (guiController && guiController.isDarkMode) ? "#aaaaaa" : "#444444"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                            Column {
                                width: parent.width
                                spacing: 8

                                Repeater {
                                    model: guiController ? guiController.benchmarkResultsModel : null

                                    RowLayout {
                                        width: parent.width
                                        spacing: 10

                                        Text {
                                            text: model.schedulerName
                                            color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                                            font.pixelSize: 11
                                            Layout.preferredWidth: 80
                                        }

                                        Rectangle {
                                            Layout.fillWidth: true
                                            height: 16
                                            color: (guiController && guiController.isDarkMode) ? "#333333" : "#e0e0e0"
                                            radius: 3

                                            Rectangle {
                                                width: Math.max(0, Math.min(parent.width, parent.width * model.jainsFairness)) // Индекс от 0 до 1
                                                height: parent.height
                                                color: "#2196f3"
                                                radius: 3
                                            }
                                        }

                                        Text {
                                            text: Number(model.jainsFairness).toFixed(3)
                                            color: "#2196f3"
                                            font.pixelSize: 11
                                            font.bold: true
                                            Layout.preferredWidth: 50
                                            horizontalAlignment: Text.AlignRight
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}