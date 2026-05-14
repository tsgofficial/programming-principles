import antlr4 from 'antlr4';
import TinyPLListener from './TinyPLListener.js'

class MyVisitor extends TinyPLListener {
    constructor(output) {
        super();
        this.output = output;
        this.functions = {};
        this.scopeStack = [{}];
        this.skipDepth = 0;
    }

    currentScope() {
        return this.scopeStack[this.scopeStack.length - 1];
    }

    lookup(name) {
        for (let i = this.scopeStack.length - 1; i >= 0; i--) {
            if (Object.prototype.hasOwnProperty.call(this.scopeStack[i], name)) {
                return this.scopeStack[i][name];
            }
        }
        return undefined;
    }

    assignToExisting(name, value) {
        for (let i = this.scopeStack.length - 1; i >= 0; i--) {
            if (Object.prototype.hasOwnProperty.call(this.scopeStack[i], name)) {
                this.scopeStack[i][name] = value;
                return;
            }
        }
    }

    enterFunctionDeclaration(ctx) {
        if (this.skipDepth === 0) {
            const name = ctx.Identifier().getText();
            this.functions[name] = ctx;
        }
        this.skipDepth++;
    }

    exitFunctionDeclaration(ctx) {
        this.skipDepth--;
    }

    enterVariableDeclaration(ctx) {
        if (this.skipDepth > 0) return;
        const name = ctx.Identifier().getText();
        this.currentScope()[name] = undefined;
    }

    enterAssignment(ctx) {
        if (this.skipDepth > 0) return;
        const idents = ctx.Identifier();
        const target = idents[0].getText();
        let value;
        if (ctx.Number()) {
            value = ctx.Number().getText();
        } else {
            value = this.lookup(idents[1].getText());
        }
        this.assignToExisting(target, value);
    }

    enterCall(ctx) {
        if (this.skipDepth > 0) return;
        if (ctx.PrintKeyword()) {
            const name = ctx.Identifier().getText();
            const value = this.lookup(name);
            this.output.push(String(value));
        } else {
            const name = ctx.Identifier().getText();
            const funcCtx = this.functions[name];
            if (funcCtx) {
                this.scopeStack.push({});
                const body = funcCtx.statements();
                antlr4.tree.ParseTreeWalker.DEFAULT.walk(this, body);
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
        antlr4.tree.ParseTreeWalker.DEFAULT.walk(treeVisitor, this.tree);
        return output;
    }
}
