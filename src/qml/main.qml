import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.3

ApplicationWindow {
    id: window
    width: 1500
    height: 900
    visible: true
    title: "Thread Scheduler Visualizer"

    color: (guiController && guiController.isDarkMode) ? "#121212" : "#f5f5f5"

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: mainColumn.implicitHeight + 20
        clip: true

        ColumnLayout {
            id: mainColumn
            width: parent.width
            anchors.margins: 10
            spacing: 10

            // Simulation & Algorithm Controls
            GroupBox {
                title: "Simulation & Algorithm Controls"
                Layout.fillWidth: true

                background: Rectangle {
                    color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#ffffff"
                    border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
                    radius: 4
                }

                label: Label {
                    text: parent.title
                    color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    font.bold: true
                }

                RowLayout {
                    spacing: 15
                    anchors.fill: parent

                    Label {
                        text: "Algorithm:"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    }
                    ComboBox {
                        id: algoSelector
                        model: [
                            "FCFS",
                            "Round Robin",
                            "Priority",
                            "SJF",
                            "SRTF",
                            "MLFQ"
                        ]
                        currentIndex: 0
                        onCurrentTextChanged: {
                            if (guiController) guiController.currentScheduler = currentText
                        }
                    }

                    Label {
                        text: "Quantum:"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                        visible: algoSelector.currentText === "Round Robin"
                    }
                    SpinBox {
                        id: quantumSpinBox
                        from: 1
                        to: 10
                        value: guiController ? guiController.timeQuantum : 1
                        visible: algoSelector.currentText === "Round Robin"
                        onValueChanged: {
                            if (guiController) guiController.timeQuantum = value
                        }
                    }

                    Item { Layout.fillWidth: true } // Spacer

                    Switch {
                        text: "Dark Theme"
                        checked: guiController ? guiController.isDarkMode : true
                        onCheckedChanged: {
                            if (guiController) guiController.isDarkMode = checked
                        }

                        contentItem: Text {
                            text: parent.text
                            color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }

                    Switch {
                        text: "Animations"
                        checked: guiController ? guiController.animationsEnabled : true
                        onCheckedChanged: {
                            if (guiController) guiController.animationsEnabled = checked
                        }

                        contentItem: Text {
                            text: parent.text
                            color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }

                    Button {
                        text: "Compare Algorithms"
                        onClicked: {
                            if (guiController) guiController.runAlgorithmBenchmark()
                            benchmarkWindow.open()
                        }
                    }

                    Button {
                        text: (guiController && guiController.isRunning) ? "Pause" : "Start"
                        onClicked: {
                            if (guiController) {
                                if (guiController.isRunning)
                                    guiController.pauseSimulation()
                                else
                                    guiController.startSimulation()
                            }
                        }
                    }

                    Button {
                        text: "Step"
                        enabled: !(guiController && guiController.isRunning)
                        onClicked: {
                            if (guiController) guiController.stepSimulation()
                        }
                    }

                    Button {
                        text: "Step Back"
                        enabled: !(guiController && guiController.isRunning) && (guiController && guiController.currentTime > 0)
                        onClicked: {
                            if (guiController) guiController.stepBack()
                        }
                    }

                    Button {
                        text: "Reset"
                        onClicked: {
                            if (guiController) guiController.resetSimulation()
                        }
                    }
                }
            }

            // Predefined Workload Scenarios & IO Panel
            GroupBox {
                title: "Predefined Workload Scenarios & I/O"
                Layout.fillWidth: true

                background: Rectangle {
                    color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#ffffff"
                    border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
                    radius: 4
                }

                label: Label {
                    text: parent.title
                    color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    font.bold: true
                }

                RowLayout {
                    spacing: 15
                    anchors.fill: parent

                    Label {
                        text: "Scenario:"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    }
                    ComboBox {
                        id: scenarioSelector
                        model: guiController ? guiController.scenarioNames : []
                        Layout.preferredWidth: 180
                    }

                    Button {
                        text: "Load Scenario"
                        onClicked: {
                            if (guiController) {
                                guiController.loadScenario(scenarioSelector.currentIndex)
                            }
                        }
                    }

                    Item { width: 10 }

                    Button {
                        text: "Save JSON"
                        onClicked: saveJsonDialog.open()
                    }

                    Button {
                        text: "Load JSON"
                        onClicked: loadJsonDialog.open()
                    }

                    Button {
                        text: "Export CSV"
                        onClicked: exportCsvDialog.open()
                    }

                    Item { Layout.fillWidth: true }
                }
            }

            // The Flow Addition Panel
            GroupBox {
                title: "Add New Thread"
                Layout.fillWidth: true

                background: Rectangle {
                    color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#ffffff"
                    border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
                    radius: 4
                }

                label: Label {
                    text: parent.title
                    color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    font.bold: true
                }

                RowLayout {
                    spacing: 15
                    anchors.fill: parent

                    Label {
                        text: "ID:"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    }
                    SpinBox {
                        id: threadIdInput
                        from: 1
                        to: 99
                        value: 1

                        Connections {
                            target: guiController ? guiController : null
                            function onCurrentTimeChanged() {
                                if (guiController && guiController.currentTime === 0) {
                                    threadIdInput.value = 1
                                }
                            }
                        }
                    }

                    Label {
                        text: "Burst Time:"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    }
                    SpinBox { id: burstTimeInput; from: 1; to: 20; value: 3 }

                    Label {
                        text: "Priority:"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                    }
                    SpinBox { id: priorityInput; from: 1; to: 10; value: 1 }

                    Button {
                        text: "Add Thread"
                        onClicked: {
                            if (guiController) {
                                guiController.addThread(threadIdInput.value, burstTimeInput.value, priorityInput.value)
                                threadIdInput.value = threadIdInput.value + 1
                            }
                        }
                    }

                    Item { Layout.fillWidth: true }
                }
            }

            // Ready Queue Panel with MLFQ Level Support
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 110
                color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#ffffff"
                border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
                radius: 5
                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 5

                    Text {
                        text: "Ready Queue / MLFQ Priority Levels"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                        Row {
                            spacing: 12

                            Repeater {
                                model: guiController ? guiController.readyQueueModel : null

                                delegate: Rectangle {
                                    width: 100
                                    height: 60
                                    color: (guiController && guiController.isDarkMode) ? "#2a2a2a" : "#eaeaea"
                                    border.color: model.color || "#0277bd"
                                    border.width: 2
                                    radius: 4

                                    scale: hoverHandler.hovered ? 1.03 : 1.0
                                    Behavior on scale {
                                        NumberAnimation {
                                            duration: 150
                                            running: guiController ? guiController.animationsEnabled : true
                                        }
                                    }

                                    HoverHandler {
                                        id: hoverHandler
                                    }

                                    ToolTip.visible: hoverHandler.hovered
                                    ToolTip.text: "Thread T" + model.threadId + "\nQueue Level: " + model.queueLevel + "\nPriority: " + model.priority + "\nBurst: " + model.burstTime

                                    Column {
                                        anchors.centerIn: parent
                                        spacing: 2
                                        Text {
                                            text: "T" + model.threadId + " (L" + model.queueLevel + ")"
                                            color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                                            font.bold: true
                                            font.pixelSize: 11
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }
                                        Text {
                                            text: "Burst: " + model.burstTime
                                            color: (guiController && guiController.isDarkMode) ? "#aaaaaa" : "#555555"
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

            // Visualization display area (Gantt Chart)
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 140
                color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#ffffff"
                border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
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
                            color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                            font.pixelSize: 16
                            font.bold: true
                        }
                        Item { Layout.fillWidth: true }
                        Text {
                            text: "Current Time: " + (guiController ? guiController.currentTime : 0)
                            color: (guiController && guiController.isDarkMode) ? "#aaaaaa" : "#555555"
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

                            Repeater {
                                model: guiController ? guiController.ganttBlocksModel : null

                                delegate: Rectangle {
                                    id: ganttDelegate

                                    width: model.duration * 40
                                    height: 70
                                    color: hoverHandlerGantt.hovered ? Qt.lighter(model.color, 1.2) : model.color
                                    radius: 4
                                    border.color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                                    border.width: 1

                                    // ===== ИСПРАВЛЕНИЕ БАГА =====
                                    // Флаг "появился" — после первого Component.onCompleted
                                    // становится true и больше никогда не сбрасывается.
                                    // Благодаря этому переключение animationsEnabled
                                    // НЕ пересчитывает opacity существующих блоков в 0.
                                    property bool appeared: false

                                    opacity: appeared
                                             ? 1.0
                                             : ((guiController && guiController.animationsEnabled) ? 0.0 : 1.0)

                                    Component.onCompleted: {
                                        // Устанавливаем appeared = true. При включённых
                                        // анимациях это вызовет пересчёт opacity 0.0 -> 1.0,
                                        // и Behavior ниже проиграет fade-in. При выключенных
                                        // анимациях opacity и так 1.0 — визуально ничего
                                        // не меняется, но binding всё равно "замораживается"
                                        // на значении 1.0, что и предотвращает исчезновение
                                        // блоков при повторном включении анимаций.
                                        appeared = true
                                    }

                                    Behavior on opacity {
                                        NumberAnimation {
                                            duration: 250
                                            running: guiController ? guiController.animationsEnabled : true
                                        }
                                    }
                                    // ===== КОНЕЦ ИСПРАВЛЕНИЯ =====

                                    scale: hoverHandlerGantt.hovered ? 1.04 : 1.0

                                    Behavior on scale {
                                        NumberAnimation {
                                            duration: 150
                                            easing.type: Easing.OutQuad
                                            running: guiController ? guiController.animationsEnabled : true
                                        }
                                    }

                                    HoverHandler {
                                        id: hoverHandlerGantt
                                    }

                                    ToolTip.visible: hoverHandlerGantt.hovered
                                    ToolTip.text: "Execution Block: " + model.name + "\nInterval: [" + model.startTime + " - " + (model.startTime + model.duration) + "]"

                                    Column {
                                        anchors.centerIn: parent
                                        Text {
                                            text: model.name
                                            color: "#ffffff"
                                            font.bold: true
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }
                                        Text {
                                            text: "[" + model.startTime + " - " + (model.startTime + model.duration) + "]"
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

            // Status Table Panel
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 160
                color: (guiController && guiController.isDarkMode) ? "#1e1e1e" : "#ffffff"
                border.color: (guiController && guiController.isDarkMode) ? "#333333" : "#cccccc"
                radius: 5
                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Text {
                        text: "Process / Thread Status Table"
                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 25
                        color: (guiController && guiController.isDarkMode) ? "#2d2d2d" : "#e0e0e0"
                        radius: 3

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10

                            Text { text: "ID"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 60 }
                            Text { text: "Priority"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 80 }
                            Text { text: "State"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.preferredWidth: 100 }
                            Text { text: "Remaining / Burst"; color: (guiController && guiController.isDarkMode) ? "#aaa" : "#333"; font.bold: true; Layout.fillWidth: true }
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: guiController ? guiController.threadListModel : null
                        clip: true
                        spacing: 4

                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 30
                            color: (guiController && guiController.isDarkMode) ? "#252526" : "#f0f0f0"
                            radius: 3
                            border.color: (guiController && guiController.isDarkMode) ? "#383838" : "#d0d0d0"

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
                                        color: model.color
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Text {
                                        text: "T" + model.threadId
                                        color: (guiController && guiController.isDarkMode) ? "#ffffff" : "#000000"
                                        font.bold: true
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }

                                Text {
                                    text: model.priority
                                    color: (guiController && guiController.isDarkMode) ? "#cccccc" : "#333333"
                                    Layout.preferredWidth: 80
                                }

                                Rectangle {
                                    Layout.preferredWidth: 90
                                    height: 20
                                    radius: 4
                                    color: {
                                        if (model.state === "RUNNING") return "#2e7d32"
                                        if (model.state === "READY") return "#0277bd"
                                        if (model.state === "TERMINATED") return "#424242"
                                        return "#616161"
                                    }

                                    Text {
                                        text: model.state
                                        color: "#ffffff"
                                        font.pixelSize: 11
                                        font.bold: true
                                        anchors.centerIn: parent
                                    }
                                }

                                Text {
                                    text: model.remainingTime + " / " + model.burstTime + " ticks"
                                    color: (guiController && guiController.isDarkMode) ? "#cccccc" : "#333333"
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
            }

            // Thread State Transition Diagram Panel
            ThreadStateDiagram {
                Layout.fillWidth: true
                implicitHeight: 200
                threadModel: guiController ? guiController.threadListModel : null
            }
        }
    }

    FileDialog {
        id: saveJsonDialog
        title: "Save Workload Scenario"
        selectExisting: false
        nameFilters: ["JSON Files (*.json)"]
        onAccepted: {
            if (guiController) guiController.saveToJson(fileUrl)
        }
    }

    FileDialog {
        id: loadJsonDialog
        title: "Load Workload Scenario"
        selectExisting: true
        nameFilters: ["JSON Files (*.json)"]
        onAccepted: {
            if (guiController) guiController.loadFromJson(fileUrl)
        }
    }

    FileDialog {
        id: exportCsvDialog
        title: "Export Metrics to CSV"
        selectExisting: false
        nameFilters: ["CSV Files (*.csv)"]
        onAccepted: {
            if (guiController) guiController.exportCsv(fileUrl)
        }
    }

    BenchmarkWindow {
        id: benchmarkWindow
    }
}