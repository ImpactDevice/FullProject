var timeFormat = "MM/DD/YYYY HH:mm";
function randomScalingFactor() {
    return Math.round(Math.random() * 100 * (Math.random() > 0.5 ? -1 : 1));
}
function randomColorFactor() {
    return Math.round(Math.random() * 255);
}
function randomColor(opacity) {
    return (
        "rgba(" +
        randomColorFactor() +
        "," +
        randomColorFactor() +
        "," +
        randomColorFactor() +
        "," +
        (opacity || ".3") +
        ")"
    );
}
function newDate(days) {
    return moment().add(days, "d").toDate();
}
function newDateString(days) {
    return moment().add(days, "d").format(timeFormat);
}
function newTimestamp(days) {
    return moment().add(days, "d").unix();
}
function resetZoom() {
    window.myLine.resetZoom();
}
var config = {
    type: "line",
    data: {
        labels: [
            newDate(0),
            newDate(1),
            newDate(2),
            newDate(3),
            newDate(4),
            newDate(5),
            newDate(6)
        ], // Date Objects
        datasets: [
            {
                label: "My First dataset",
                data: [
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor()
                ],
                fill: false,
                borderDash: [5, 5]
            },
            {
                label: "My Second dataset",
                data: [
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor()
                ]
            },
            {
                label: "Dataset with point data",
                data: [
                    { x: newDateString(0), y: randomScalingFactor() },
                    { x: newDateString(5), y: randomScalingFactor() },
                    { x: newDateString(7), y: randomScalingFactor() },
                    { x: newDateString(15), y: randomScalingFactor() }
                ],
                fill: false
            }
        ]
    },
    options: {
        responsive: true,
        title: {
            display: true,
            text: "Chart.js Time Scale"
        },
        scales: {
            x: {
                type: "time",
                time: {
                    format: timeFormat,
                    tooltipFormat: "ll HH:mm"
                },
                title: {
                    display: true,
                    text: "Date"
                },
                ticks: {
                    maxRotation: 0
                }
            },
            y: {
                title: {
                    display: true,
                    text: "value"
                }
            }
        },
        pan: {
            enabled: true,
            mode: "x",
            speed: 10,
            threshold: 10
        },
        zoom: {
            enabled: true,
            drag: false,
            mode: "xy",
            limits: {
                max: 10,
                min: 0.5
            }
        }
    }
};
config.data.datasets.forEach(function (dataset) {
    dataset.borderColor = randomColor(0.4);
    dataset.backgroundColor = randomColor(0.5);
    dataset.pointBorderColor = randomColor(0.7);
    dataset.pointBackgroundColor = randomColor(0.5);
    dataset.pointBorderWidth = 1;
});
window.onload = function () {
    var ctx = document.getElementById("canvas").getContext("2d");
    window.myLine = new Chart(ctx, config);
};