import sys

from pycparser import c_ast, parse_file

# This is not required if you've installed pycparser into
# your site-packages/ with setup.py
sys.path.extend([".", ".."])

graph = {}
recursive_fns = set()


class FuncCallVisitor(c_ast.NodeVisitor):
    def __init__(self, parent_def):
        self.parent_def = parent_def

    def visit_FuncCall(self, node):
        call = node.name.name
        graph[self.parent_def].add(call)

        if node.args:
            self.visit(node.args)


class FuncDefVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        defined_fn = node.decl.name
        graph[defined_fn] = set()

        FuncCallVisitor(defined_fn).visit(node)


def walk(fn, seen):
    seen.add(fn)

    # If this is a call to an external function, like memcpy()
    if fn not in graph:
        return

    calls = graph[fn]

    for call in calls:
        if call in seen:
            recursive_fns.add(call)
            continue

        walk(call, seen.copy())


def run():
    # Note that cpp is used. Provide a path to your own cpp or
    # make sure one exists in PATH.
    ast = parse_file("../grug/grug.c", use_cpp=True, cpp_args=r"-Ifake_libc_include")

    FuncDefVisitor().visit(ast)

    for fn in graph.keys():
        walk(fn, set())

    print(recursive_fns)


if __name__ == "__main__":
    run()
