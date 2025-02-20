import matplotlib.pyplot as plt
from networkx.drawing.nx_agraph import graphviz_layout
import networkx as nx
import sys

# This is not required if you've installed pycparser into
# your site-packages/ with setup.py
sys.path.extend([".", ".."])

from pycparser import c_ast, parse_file

# TODO:
# 1. Add a dict which has the name of the defined fn as the key, and a Set of fns it calls as a value
# 2. Render the dict as a graph


graph = {}


class FuncCallVisitor(c_ast.NodeVisitor):
    def __init__(self, parent):
        self.parent = parent

    def visit_FuncCall(self, node):
        graph[self.parent].add(node.name.name)
        # print(f"CALL {node.name.name} at {node.name.coord}")

        # Visit args in case they contain more func calls.
        if node.args:
            self.visit(node.args)


# A simple visitor for FuncDef nodes that prints the names and
# locations of function definitions.
class FuncDefVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        graph[node.decl.name] = set()
        # print(f"DEF  {node.decl.name} at {node.decl.coord}")
        v = FuncCallVisitor(node.decl.name)
        v.visit(node)


def run():
    # Note that cpp is used. Provide a path to your own cpp or
    # make sure one exists in PATH.
    # ast = parse_file("../grug/grug.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")
    ast = parse_file("foo.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")

    v = FuncDefVisitor()
    v.visit(ast)

    G = nx.DiGraph()
    labels = {}
    for definition, calls in graph.items():
        labels[definition] = definition

        for call in calls:
            label_key = definition + "_calls_" + call
            labels[label_key] = call

            print(definition, call)
            # G.add_edge(definition, call)
            G.add_edge(definition, label_key)
    print(labels)
    # G.add_edge(0, 1)
    # G.add_edge(0, 2)
    # G.add_edges_from([(0, 1), (0, 2)])
    # pos = nx.spring_layout(G)
    # pos = graphviz_layout(G, prog='dot', root=0, args='-Grankdir="LR"')
    pos = graphviz_layout(G, prog='dot')
    print(pos)

    plt.axis('off')
    fig = plt.figure(1)
    # fig.set_figwidth(100.0)
    # fig.set_figheight(100.0)

    nx.draw_networkx_nodes(G, pos, node_shape='o', node_size=1000)
    nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrows=True)
    # nx.draw_networkx_labels(G, pos, font_size=10, font_color="white")
    # TODO: The reason no line is drawn between main() and foo(), is because foo() its key is "main_calls_foo"
    nx.draw_networkx_labels(G, pos, labels, font_size=10, font_color="white")
    plt.savefig("graph.png")


    # plt.figure()
    # plt.axis('off')
    # fig = plt.figure(1)
    # pos = nx.spring_layout(G)
    # # pos = graphviz_layout(G, prog='dot')
    # nx.draw_networkx_nodes(G, pos)
    # nx.draw_networkx_edges(G, pos)
    # nx.draw_networkx_labels(G, pos)

    # # plt.xlim(min(x for x, y in pos.values()), max(x for x, y in pos.values()))
    # # plt.ylim(min(y for x, y in pos.values()), max(y for x, y in pos.values()))

    # fig.set_figwidth(100.0)
    # fig.set_figheight(100.0)

    # plt.savefig("graph.png",bbox_inches="tight")
    # plt.close()


if __name__ == "__main__":
    run()
