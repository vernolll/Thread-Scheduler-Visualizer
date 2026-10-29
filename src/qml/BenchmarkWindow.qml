import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: benchmarkPopup
    modal: true
    focus: true
    width: 820
    height: 480
    x: parent ? (parent.width - width) / 2 : 100
    y: parent ? (parent.height - height) / 2 : 100
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#1e1e1e"
        border.color: "#444444"
        border.width: 1
        radius: 8
    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 12

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Algorithm Comparison Benchmark"
                color: "#ffffff"
                font.pixelSize: 16
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
                    color: "#ffffff"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: benchmarkPopup.close()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 32
            color: "#252526"
            radius: 4
            border.color: "#333333"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                Text { text: "Algorithm"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 110 }
                Text { text: "Ticks"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 60 }
                Text { text: "Throughput"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 95 }
                Text { text: "Context Sw."; color: "#aaa"; font.bold: true; Layout.preferredWidth: 95 }
                Text { text: "Fairness (Jain)"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 110 }
                Text { text: "Avg Waiting"; color: "#aaa"; font.bold: true; Layout.preferredWidth: 95 }
                Text { text: "Avg Turnaround"; color: "#aaa"; font.bold: true; Layout.fillWidth: true }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: guiController ? guiController.benchmarkResults : []
            clip: true
            spacing: 4

            delegate: Rectangle {
                width: ListView.view.width
                height: 36
                color: "#2a2a2a"
                radius: 4
                border.color: "#383838"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10

                    Text { text: modelData.schedulerName; color: "#ffffff"; font.bold: true; Layout.preferredWidth: 110 }
                    Text { text: modelData.totalTicks; color: "#cccccc"; Layout.preferredWidth: 60 }
                    Text { text: Number(modelData.throughput).toFixed(4); color: "#4caf50"; Layout.preferredWidth: 95 }
                    Text { text: modelData.contextSwitches; color: "#cccccc"; Layout.preferredWidth: 95 }
                    Text { text: Number(modelData.jainsFairness).toFixed(3); color: "#2196f3"; Layout.preferredWidth: 110 }
                    Text { text: Number(modelData.avgWaiting).toFixed(1); color: "#cccccc"; Layout.preferredWidth: 95 }
                    Text { text: Number(modelData.avgTurnaround).toFixed(1); color: "#ff9800"; Layout.fillWidth: true }
                }
            }
        }
    }
}