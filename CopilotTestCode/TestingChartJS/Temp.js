// Sample data array
const data = [12, 19, 3, 5, 2, 3, 7, 10, 15, 8];

// Create a chart
const ctx = document.getElementById('myChart').getContext('2d');
const myChart = new Chart(ctx, {
    type: 'line', // You can change this to 'bar', 'pie', etc.
    data: {
        labels: data.map((_, index) => `Label ${index + 1}`), // Generate labels for the data points
        datasets: [{
            label: 'Sample Data',
            data: data,
            backgroundColor: 'rgba(75, 192, 192, 0.2)',
            borderColor: 'rgba(75, 192, 192, 1)',
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});