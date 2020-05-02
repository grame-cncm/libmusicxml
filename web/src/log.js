

function mylog(text, level) {
	let dest = document.getElementById("logs");
	if (level==1) 		text = '<span class="warning">'+text+'</span>';
	else if (level==2) 	text = '<span class="error">'+text+'</span>';
	dest.innerHTML += text + "\n";
}

var oldcons= console;
var console =(function(oldCons){
    return {
        log: function(text){
			oldCons.log(text);
			mylog(text, 0);
        },
        warn: function (text) {
            oldCons.warn(text);
			mylog(text, 1);
        },
        error: function (text) {
            oldCons.error(text);
			mylog(text, 2);
        }
    };
}(window.console));
