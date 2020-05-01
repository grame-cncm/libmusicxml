
//----------------------------------------------------------------------------
// a download function
//----------------------------------------------------------------------------
function download (filename : string, text: string) : void {
	downloadMedia (filename, text, "text/plain;charset=utf-8,");
	// var element = document.createElement('a');
	// element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
	// element.setAttribute('download', filename);

	// element.style.display = 'none';
	// document.body.appendChild(element);
	// element.click();
	// document.body.removeChild(element);
}

function downloadMedia (filename : string, data: string, type: string) : void {
	var element = document.createElement('a');
	element.setAttribute('href', 'data:'+type + encodeURIComponent(data));
	element.setAttribute('download', filename);

	element.style.display = 'none';
	document.body.appendChild(element);
	element.click();
	document.body.removeChild(element);
}
