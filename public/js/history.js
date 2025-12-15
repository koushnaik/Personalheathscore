
fetch('/api/history')
.then(r=>r.json())
.then(d=>{
 new Chart(document.getElementById('historyChart'),{
  type:'bar',
  data:{labels:d.labels,datasets:[{label:'Steps',data:d.steps}]}
 });
});
