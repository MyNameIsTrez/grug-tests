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

    print(graph)


if __name__ == "__main__":
    run()
