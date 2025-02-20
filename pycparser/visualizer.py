import sys

import matplotlib.pyplot as plt
import networkx as nx
from networkx.drawing.nx_agraph import graphviz_layout

# This is not required if you've installed pycparser into
# your site-packages/ with setup.py
sys.path.extend([".", ".."])

from pycparser import c_ast, parse_file

graph = {}
called_fns = set()


key = 0


class FuncCallVisitor(c_ast.NodeVisitor):
    def __init__(self, parent_def):
        self.parent_def = parent_def

    def visit_FuncCall(self, node):
        graph[self.parent_def].add(node.name.name)

        # TODO: If the chain goes A->B->A, then this will mark both as being called, meaning neither will be graphed right now
        if self.parent_def != node.name.name:
            called_fns.add(node.name.name)

        if node.args:
            self.visit(node.args)


class FuncDefVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        graph[node.decl.name] = set()
        v = FuncCallVisitor(node.decl.name)
        v.visit(node)


def run():
    global key

    # Note that cpp is used. Provide a path to your own cpp or
    # make sure one exists in PATH.
    # ast = parse_file("../grug/grug.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")
    ast = parse_file("grug_3k.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")
    # ast = parse_file("grug_1k.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")
    # ast = parse_file("foo.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")

    v = FuncDefVisitor()
    v.visit(ast)

    G = nx.DiGraph()

    labels = {}
    definition_keys = []

    def walk(fn, seen):
        global key

        # print(f"fn: {fn}, key: {key}")
        seen.add(fn)

        definition_key = key
        G.add_node(definition_key)

        # If this is a libc function call, like memcpy(), return
        if fn not in graph:
            return
        calls = graph[fn]

        for call in calls:
            if call in seen:
                # if fn == call:
                #     print(f"continuing: recursive {call}")
                # else:
                #     print(f"continuing: seen {call}")
                continue

            key += 1
            call_key = key
            labels[call_key] = call

            G.add_edge(definition_key, call_key)

            walk(call, seen.copy())

    for fn in graph.keys():
        if fn in called_fns:
            continue
        key += 1
        labels[key] = fn
        definition_keys.append(key)
        print(f"fn def: {fn}")
        walk(fn, set())
        print("done")

    print("a")
    # pos = nx.spectral_layout(G)
    # pos = nx.spring_layout(G, k=1)
    # pos = graphviz_layout(G, prog='dot', root=0, args='-Grankdir="LR"')
    # pos = graphviz_layout(G, prog="dot")
    pos = graphviz_layout(G)

    print("b")
    plt.axis("off")
    fig = plt.figure(1)
    fig.set_figwidth(20.0)
    fig.set_figheight(20.0)

    print("c")
    colors = ["blue"] * len(G)
    for definition_key in definition_keys:
        colors[definition_key - 1] = "red"

    print("d")
    nx.draw_networkx_nodes(G, pos, node_shape="o", node_size=300, node_color=colors)
    nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrows=True)
    nx.draw_networkx_labels(G, pos, labels, font_size=10, font_color="black")

    # plt.xlim(min(x for x, y in pos.values()), max(x for x, y in pos.values()))
    # plt.ylim(min(y for x, y in pos.values()), max(y for x, y in pos.values()))

    # plt.savefig("graph.png", bbox_inches="tight")
    plt.savefig("graph.png")


if __name__ == "__main__":
    run()
