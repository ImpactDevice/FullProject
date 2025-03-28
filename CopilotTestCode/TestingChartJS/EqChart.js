
// Define the polynomial equation, e.g., y = x^3 - 6x^2 + 11x - 6
function polynomial(x) {
    return Math.sin( 10/ Math.PI * x);
}

// Generate x and y values
function Get_Data() {
    const xValues = [];
    const yValues = [];
    for (let x = 0; x <= 2 * Math.PI; x += 0.01) {
        xValues.push(x);
        yValues.push(polynomial(x));
    }
    return [xValues, yValues];
}

const xValues = [];
const yValues = [];
for (let x = 0; x <= 2 * Math.PI; x += 0.01) {
    xValues.push(x);
    yValues.push(polynomial(x));
}


function Get_xValues() {
    const values = Get_Data();
    return values[0];
}

function Get_yValues() {
    const values = Get_Data();
    return values[1];
}


// Create the chart
const ctx = document.getElementById('myChart').getContext('2d');
const myChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: xValues,
        datasets: [{
            label: 'y = sin(500 * π * x)',
            data: yValues,
            backgroundColor: 'rgba(75, 192, 192, 0.2)',
            borderColor: 'rgba(75, 192, 192, 1)',
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
        scales: {
            x: {
                type: 'linear',
                title: {
                    display: true,
                    text: 'x'
                },
                min: 0,
                max: 2 * Math.PI
            },
            y: {
                title: {
                    display: true,
                    text: 'y'
                },
                beginAtZero: true
            }
        },
        plugins: {
            title: {
                display: true,
                text: 'Polynomial Equation Plot'
            }
        }
    }
});

// Add event listener to the button to zoom in on the range π to 1.1π
document.getElementById('zoomButton').addEventListener('click', function() {
    myChart.options.scales.x.min = Math.PI;
    myChart.options.scales.x.max = 1.1 * Math.PI;
    myChart.update();
});




//===================Done Using Ploty========================
// Define the polynomial equation, e.g., y = x^3 - 6x^2 + 11x - 6
// function polynomial(x) {
//     return x**3;
// }

// // Generate x values
// const xValues = [];
// const yValues = [];
// for (let x = -2; x <= 5; x += 0.1) {
//     xValues.push(x);
//     yValues.push(polynomial(x));
// }

// // Create the plot
// const trace = {
//     x: xValues,
//     y: yValues,
//     type: 'scatter',
//     mode: 'lines',
//     name: 'y = x^3 - 6x^2 + 11x - 6'
// };

// const layout = {
//     title: 'Polynomial Equation Plot',
//     xaxis: {
//         title: 'x'
//     },
//     yaxis: {
//         title: 'y'
//     }
// };

// Plotly.newPlot('plot', [trace], layout);

// const ctx = document.getElementById('myChart').getContext('2d');
// const myChart = new Chart(ctx, {
//     type: 'line', // You can change this to 'bar', 'pie', etc.
//     data: {
//         labels: data.map((_, index) => `Label ${index + 1}`), // Generate labels for the data points
//         datasets: [{
//             label: 'Sample Data',
//             data: data,
//             backgroundColor: 'rgba(75, 192, 192, 0.2)',
//             borderColor: 'rgba(75, 192, 192, 1)',
//             borderWidth: 1
//         }]
//     },
//     options: {
//         scales: {
//             y: {
//                 beginAtZero: true
//             }
//         }
//     }
// });