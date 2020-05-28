//------------------------------------------------------------------------
// scan the current location to detect parameters
function scanUrl()	{
    let result = new Array();
    let arg = window.location.search.substring(1);
    let n = arg.indexOf("=");
    while (n > 0) {
        let option  = arg.substr(0,n);
        let remain = arg.substr(n+1);
        let next = remain.indexOf("?");
        if (next > 0) {
            let value = remain.substr(0,next);
            result.push ( {option: option, value: value} );
            arg = remain.substr(next + 1);
            n = arg.indexOf("=");
        }
        else {
            result.push ( {option: option, value: remain} );
            break;
        }
    }
    return result;
	}

//------------------------------------------------------------------------
// scan the current location to detect parameters
function scanOptions()	{
console.log ("lxml code scan options: " + window.location);
    let options = this.scanUrl();
    for (let i=0; i<options.length; i++) {
        let option = options[i].option;
        let value = options[i].value;
        switch (option) {
            case "s":
console.log ("lxml code receive s request id: " + value);
            let gmn = localStorage.getItem(value);
            if (gmn) {
console.log ("lxml code put id: " + value);
                var div = document.getElementById("code");
                div.innerHTML =  gmn;
                // document.body.innerText = gmn;
            }
            break;
        }
    }
}

scanOptions();