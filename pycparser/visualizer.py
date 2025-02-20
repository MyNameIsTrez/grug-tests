import sys

import matplotlib.pyplot as plt
import networkx as nx
from networkx.drawing.nx_agraph import graphviz_layout

# This is not required if you've installed pycparser into
# your site-packages/ with setup.py
sys.path.extend([".", ".."])

from pycparser import c_ast, parse_file

graph = {}


key = 0


class FuncCallVisitor(c_ast.NodeVisitor):
    def __init__(self, parent_def):
        self.parent_def = parent_def

    def visit_FuncCall(self, node):
        graph[self.parent_def].add(node.name.name)

        if node.args:
            self.visit(node.args)


class FuncDefVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        graph[node.decl.name] = set()
        v = FuncCallVisitor(node.decl.name)
        v.visit(node)


def run():
    # Note that cpp is used. Provide a path to your own cpp or
    # make sure one exists in PATH.
    # ast = parse_file("../grug/grug.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")
    ast = parse_file("foo.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")

    v = FuncDefVisitor()
    v.visit(ast)

    print(graph)

    G = nx.DiGraph()

    labels = {}
    seen_definitions = set()

    def walk(fn, seen):
        global key

        print(f"fn: {fn}, key: {key}")
        seen.add(fn)
        seen_definitions.add(fn)

        definition_key = key

        if fn not in graph:
            return
        calls = graph[fn]

        for call in calls:
            if call in seen:
                if fn == call:
                    print(f"continuing: recursive {call}")
                else:
                    print(f"continuing: seen {call}")
                continue

            key += 1
            call_key = key
            labels[call_key] = call

            G.add_edge(definition_key, call_key)

            walk(call, seen.copy())

    # TODO: Create a new networkx graph for every not seen fn definition
    # for fn in graph.keys():
    fn = "main"
    # if fn in seen_definitions:
    #     continue
    print(f"wow: {fn}")
    labels[0] = fn
    walk(fn, set())

    # key = 0
    # for definition, calls in graph.items():
    #     definition_key = key
    #     key += 1
    #     labels[definition_key] = definition

    #     for call in calls:
    #         call_key = key
    #         key += 1
    #         labels[call_key] = call

    #         # print(definition, call)
    #         G.add_edge(definition_key, call_key)
    print(labels)

    # pos = nx.spring_layout(G)
    # pos = graphviz_layout(G, prog='dot', root=0, args='-Grankdir="LR"')
    pos = graphviz_layout(G, prog="dot")

    plt.axis("off")
    fig = plt.figure(1)
    # fig.set_figwidth(100.0)
    # fig.set_figheight(100.0)

    nx.draw_networkx_nodes(G, pos, node_shape="o", node_size=1000)
    nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrows=True)
    nx.draw_networkx_labels(G, pos, labels, font_size=10, font_color="white")

    # plt.xlim(min(x for x, y in pos.values()), max(x for x, y in pos.values()))
    # plt.ylim(min(y for x, y in pos.values()), max(y for x, y in pos.values()))

    # plt.savefig("graph.png",bbox_inches="tight")
    plt.savefig("graph.png")


if __name__ == "__main__":
    run()
