var Module: any = {
  onRuntimeInitialized: function() {
    console.log('aaa')

    const { FlowGraph } = Module

    const g = new FlowGraph()
    const nodeA = g.addNode(200, 100)
    const nodeB = g.addNode(200, 100)
    g.addEdge(nodeA, nodeB)
    console.log(g.computeLayout())
    g.destroy()
  }
};

import '../dist/GraphLayout.js'