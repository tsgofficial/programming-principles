import antlr4 from "antlr4";
import TinyPLListener from "./TinyPLListener.js";

class MyVisitor extends TinyPLListener {
  constructor(output) {
    super();
    this.output = output;
    this.scopeStack = [{ vars: {}, funcs: {} }];
  }

  topScope() {
    return this.scopeStack[this.scopeStack.length - 1];
  }

  lookupVar(name) {
    for (let i = this.scopeStack.length - 1; i >= 0; i--) {
      if (name in this.scopeStack[i].vars) return this.scopeStack[i].vars[name];
    }
    throw new Error("Variable not declared: " + name);
  }

  assignVar(name, value) {
    for (let i = this.scopeStack.length - 1; i >= 0; i--) {
      if (name in this.scopeStack[i].vars) {
        this.scopeStack[i].vars[name] = value;
        return;
      }
    }
    throw new Error("Variable not declared: " + name);
  }

  lookupFunc(name) {
    for (let i = this.scopeStack.length - 1; i >= 0; i--) {
      if (name in this.scopeStack[i].funcs)
        return this.scopeStack[i].funcs[name];
    }
    throw new Error("Function not declared: " + name);
  }

  executeStatements(statementsCtx) {
    for (const stmt of statementsCtx.statement()) {
      this.executeStatement(stmt);
    }
  }

  executeStatement(stmt) {
    if (stmt.variableDeclaration()) {
      const name = stmt.variableDeclaration().Identifier().getText();
      this.topScope().vars[name] = 0;
    } else if (stmt.assignment()) {
      const a = stmt.assignment();
      const ids = a.Identifier();
      const targetName = ids[0].getText();
      let value;
      if (a.Number()) {
        value = parseInt(a.Number().getText(), 10);
      } else {
        value = this.lookupVar(ids[1].getText());
      }
      this.assignVar(targetName, value);
    } else if (stmt.functionDeclaration()) {
      const fd = stmt.functionDeclaration();
      const name = fd.Identifier().getText();
      this.topScope().funcs[name] = fd.statements();
    } else if (stmt.call()) {
      const c = stmt.call();
      if (c.PrintKeyword()) {
        const value = this.lookupVar(c.Identifier().getText());
        this.output.push(String(value));
      } else {
        const body = this.lookupFunc(c.Identifier().getText());
        this.scopeStack.push({ vars: {}, funcs: {} });
        this.executeStatements(body);
        this.scopeStack.pop();
      }
    }
  }
}

export default class Milestone1 {
  constructor(tree) {
    this.tree = tree;
  }

  run() {
    var output = [];
    var treeVisitor = new MyVisitor(output);
    treeVisitor.executeStatements(this.tree.statements());
    return output;
  }
}
