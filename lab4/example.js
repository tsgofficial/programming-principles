import Milestone1 from "./Milestone1.js";
import Milestone2 from "./Milestone2.js";
import parseCode from "./util.js";

const code = [
  "function a {",
  "  var x;",
  "  x = 55;",
  "  b();",
  "}",
  "function b {",
  "  print(x);",
  "}",
  "var x;",
  "x = 1;",
  "a();",
].join("");

console.log(
  "Dynamic scoping (Milestone1):",
  new Milestone1(parseCode(code)).run(),
);
console.log(
  "Static  scoping (Milestone2):",
  new Milestone2(parseCode(code)).run(),
);
