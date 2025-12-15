const canvas = document.getElementById("healthChart");

if (canvas) {
  new Chart(canvas, {
    type: "line",
    data: {
      labels: ["Sleep (hrs)", "Study (hrs)", "Water (L)", "Steps"],
      datasets: [{
        label: "Daily Health Metrics",
        data: [7, 6, 2.5, 6500],
        borderColor: "#0d6efd",
        backgroundColor: "rgba(13,110,253,0.2)",
        tension: 0.3
      }]
    },
    options: {
      responsive: true,
      scales: {
        y: {
          beginAtZero: true
        }
      }
    }
  });
}
