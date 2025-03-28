const ctx = document.getElementById('myChart').getContext('2d');

const data = {
  labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
  datasets: [
    {
      label: 'Line 1',
      data: [65, 59, 80, 81, 56, 55, 40],
      borderColor: 'rgba(255, 99, 132, 1)',
      backgroundColor: 'rgba(255, 99, 132, 0.2)',
      hidden: false
    },
    {
      label: 'Line 2',
      data: [28, 48, 40, 19, 86, 27, 90],
      borderColor: 'rgba(54, 162, 235, 1)',
      backgroundColor: 'rgba(54, 162, 235, 0.2)',
      hidden: false
    },
    {
      label: 'Line 3',
      data: [18, 48, 77, 9, 100, 27, 40],
      borderColor: 'rgba(75, 192, 192, 1)',
      backgroundColor: 'rgba(75, 192, 192, 0.2)',
      hidden: false
    }
  ]
};

const config = {
  type: 'line',
  data: data,
  options: {
    responsive: true,
    plugins: {
      legend: {
        display: true
      }
    }
  }
};

const myChart = new Chart(ctx, config);

function toggleDataset(index) {
  const dataset = myChart.data.datasets[index];
  dataset.hidden = !dataset.hidden;
  myChart.update();
}