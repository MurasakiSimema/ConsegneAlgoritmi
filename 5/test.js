const fs = require('fs');

function readCSV(filename) {
    const data = fs.readFileSync(filename, 'utf-8');
    const lines = data.trim().split('\n');
    const edges = [];

    for (const line of lines) {
        const [u, v, w] = line.split(',').map(Number);
        edges.push({ u, v, w });
    }

    return edges;
}

// Esempio di utilizzo
const edges = readCSV('soc-sign-bit.csv');

var nodes = [];

edges.forEach((edge) => {
  if(nodes[edge.u] == undefined){
    nodes[edge.u] = 1;
  } 
  if(nodes[edge.v] == undefined){
    nodes[edge.v] = 1;
  }
});

var test = [];
test[10]=0;

console.log(nodes.length);
nodes.forEach((node, idx) => {
  if(node == undefined)
    console.log(idx);
});