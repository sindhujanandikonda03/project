document.title = "I have been rewritten!";

window.onload = function () {
  document.getElementById("main-heading").innerHTML = "CS510 Network Security";
  var h2Nodes = document.getElementsByTagName('h2');
  h2Nodes[0].innerHTML = "Portland State University"
  var pNodes = document.getElementsByTagName('p');
  pNodes[0].innerHTML = "Yu Yang";
  pNodes[1].innerHTML = "yyang";

};
