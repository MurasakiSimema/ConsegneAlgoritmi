var fs = require('fs');
var json = JSON.parse(fs.readFileSync('railway.json', 'utf8'));


var data = json.elements;
var nodes = data.filter((e)=>{
  return e.type == "node" && e.tags?.name == undefined;
})

console.log(nodes.length);

var edges = data.filter((e)=>{
  return e.type == "way";
})

nodesMap ={};
nodes.forEach((node, idx) => {
  nodesMap[node.id] = {
    idx: idx,
    ...node
  };
});

console.log(Object.keys(nodesMap).length);

edgesMap = [];
/*
  edges {
    id: int
    nodes: int[]
    tags: {}
  }
*/

var keys = Object.keys(nodesMap);
edges.forEach((edge) => {
  let nodes = edge.nodes.filter((n)=>{
    return keys.includes(n)
  });
  if(nodes.length>0)
  console.log(nodes.length)
})


fs.writeFile('graph.json', JSON.stringify(json), (err) => {
  if (err) {
    console.error(err);
    return;
  }
  console.log("File scritto con successo");
})