import antlr4 from "antlr4";
import TinyPLListener from "./TinyPLListener.js";

class Scope {
  constructor(parent = null) {
    this.parent = parent;
    this.vars = {};
    this.funcs = {};
  }

  declareVar(name) {
    this.vars[name] = 0;
  }

  declareFunc(name, body, declaringScope) {
    this.funcs[name] = { body, scope: declaringScope };
  }

  lookupVar(name) {
    if (name in this.vars) return this.vars[name];
    if (this.parent) return this.parent.lookupVar(name);
    throw new Error("Variable not declared: " + name);
  }

  assignVar(name, value) {
    if (name in this.vars) {
      this.vars[name] = value;
      return;
    }
    if (this.parent) {
      this.parent.assignVar(name, value);
      return;
    }
    throw new Error("Variable not declared: " + name);
  }

  lookupFunc(name) {
    if (name in this.funcs) return this.funcs[name];
    if (this.parent) return this.parent.lookupFunc(name);
    throw new Error("Function not declared: " + name);
  }
}

class MyVisitor extends TinyPLListener {
  constructor(output) {
    super();
    this.output = output;
    this.globalScope = new Scope();
  }

  executeStatements(statementsCtx, scope) {
    for (const stmt of statementsCtx.statement()) {
      this.executeStatement(stmt, scope);
    }
  }

  executeStatement(stmt, scope) {
    if (stmt.variableDeclaration()) {
      const name = stmt.variableDeclaration().Identifier().getText();
      scope.declareVar(name);
    } else if (stmt.assignment()) {
      const a = stmt.assignment();
      const ids = a.Identifier();
      const targetName = ids[0].getText();
      let value;
      if (a.Number()) {
        value = parseInt(a.Number().getText(), 10);
      } else {
        value = scope.lookupVar(ids[1].getText());
      }
      scope.assignVar(targetName, value);
    } else if (stmt.functionDeclaration()) {
      const fd = stmt.functionDeclaration();
      const name = fd.Identifier().getText();
      scope.declareFunc(name, fd.statements(), scope);
    } else if (stmt.call()) {
      const c = stmt.call();
      if (c.PrintKeyword()) {
        const value = scope.lookupVar(c.Identifier().getText());
        this.output.push(String(value));
      } else {
        const fn = scope.lookupFunc(c.Identifier().getText());
        const callScope = new Scope(fn.scope);
        this.executeStatements(fn.body, callScope);
      }
    }
  }
}

export default class Milestone2 {
  constructor(tree) {
    this.tree = tree;
  }

  run() {
    var output = [];
    var treeVisitor = new MyVisitor(output);
    treeVisitor.executeStatements(
      this.tree.statements(),
      treeVisitor.globalScope,
    );
    return output;
  }
}
