
///<reference path="lib/libmusicxml.ts"/>
///<reference path="download.ts"/>

const kGuidoMode 	= "1";
const kLilyMode 	= "2";
const kBrailleMode 	= "3";

declare var forge: any;
interface UrlOption  { option: string; value: string; }

//----------------------------------------------------------------------------
// this is the editor part, currently using CodeMirror
//----------------------------------------------------------------------------
class XMLConverter {

	private fFileName = "";
	private fMode: string;
	private fXmlContent = "";
	private fCode: HTMLElement;
	private fLogs: HTMLElement;
	private fXmlEngine: libmusicxml;

	constructor () {
		this.fCode = document.getElementById ("code");
		this.fLogs = document.getElementById ("logs");
		$(document).on({		// prevents document to capture dropped files
			dragover: function() 	{ return false;},
			drop: function() 		{ return false;}
		});
	}

	//------------------------------------------------------------------------
	capture(event: DragEvent) {
		event.stopImmediatePropagation();
		event.preventDefault();
	}

	dragEnter (event : DragEvent) : void { 
		this.capture (event);
		this.fCode.style.opacity = "0.3";
	}

	dragLeave (event : DragEvent) : void {
		this.capture (event);
		this.fCode.style.opacity = "1";
	}

	drop (event : DragEvent) : void {
		this.dragLeave (event);
		var data = event.dataTransfer.getData("text");
		if (data) this.convert (data, "noname");
		else {
			let filelist = event.dataTransfer.files;
			if (!filelist) return;
		
			let filecount = filelist.length;
			for (let i = 0; i < filecount; i++ )
				this.droppedFile (filelist[i]);
		}
	}

	droppedFile (file: File) {
		let reader = new FileReader();	
		reader.onload = (event) => { this.convert (reader.result.toString(), file.name); };
		reader.readAsText(file, file.name);
	}
	
	initialize (xmlengine: libmusicxml) {
		this.fXmlEngine = xmlengine;
		let div = document.getElementById ("codepane");
		div.addEventListener ("dragenter",	(e: DragEvent) => { this.dragEnter(e); }, true);
		div.addEventListener ("dragleave",	(e: DragEvent) => { this.dragLeave(e); }, true);
		div.addEventListener ("drop",		(e: DragEvent) => { this.drop(e); }, true);

		$("input[name='output'").change ( (event) => { this.changeMode(<string>$("input[name='output']:checked").val()); } );
		$("#gmnbars").change 			( (event) => { this.convert(this.fXmlContent, this.fFileName+".xml"); } );
		$("#transpose").change 			( (event) => { this.convert(this.fXmlContent, this.fFileName+".xml"); } );
		$("#guidotry").click			( (event) => { this.tryGuido(); } );
		$("#lilyhelp").click			( (event) => { this.lilyHelp(); } );
		$("#clearlog").click			( (event) => { $("#logs").html(""); } );
		$("#save").click				( (event) => { this.save(); });
		$("#font-size").change			( (event) => { this.fontSize ( <number>$("#font-size").val()); });
		this.fontSize ( <number>$("#font-size").val());

		this.changeMode(<string>$("input[name='output']:checked").val());

		$('#lilyopt').on	('blur', (event) =>  { this.convert(this.fXmlContent, this.fFileName+".xml"); });
		// let logs = document.getElementById ("logs");
		// $("#log-font").click		( () => { logs.style.fontFamily = <string>$("#log-font").val(); });
		// $("#log-size").click		( () => { logs.style.fontSize = $("#log-size").val() + "px"; });
		// logs.style.fontFamily = <string>$("#log-font").val();
		// logs.style.fontSize = $("#log-size").val() + "px";
	}

	lilyHelp () { 
		this.fXmlEngine.string2lily ("", "-help");
		$("#lognav").click();
	}

	tryGuido () {
			let gmn = $("#code").text();
		if (gmn.length) {
			if (gmn.length < 2000)
		 		window.open("https://guidoeditor.grame.fr/?code=" + btoa(gmn), '_blank');
			else {
				var md = forge.md.sha256.create();
				md.update(gmn);
				let id = md.digest().toHex();
				localStorage.setItem(id, gmn);
				window.open("https://guidoeditor.grame.fr/?s=" + id, '_blank');
			}
		}
	}

	fontSize(val: number) : void {
		let pre = document.getElementsByTagName("pre");
		let i = 0;
		let px = val + "px"; 
		while (i < pre.length) {
			pre[i++].style.fontSize = px;
		}
	}

	save () : void {
		if (this.fXmlContent.length) {
			let content = this.fCode.innerText;
			if (content.length)
				download (this.fFileName + this.outSuffix(),  this.fCode.innerText); 
		}
	}

	outSuffix () : string		{ 
		switch (this.fMode) {
			case kGuidoMode: return ".gmn";
			case kLilyMode : return ".ly";
			case kBrailleMode: return ".brl";
		}
		return "";
	}

	getTranspose () : number {
		let trsp = <string>$("#transpose").val();
		if (trsp.length) return parseInt(trsp);
		return 0;
	}

	cversion () : string		{ 
		switch (this.fMode) {
			case kGuidoMode: return this.fXmlEngine.musicxml2guidoVersionStr();
			case kLilyMode : return this.fXmlEngine.musicxml2lilypondVersionStr();
			case kBrailleMode: return this.fXmlEngine.musicxml2brailleVersionStr();
		}
		return "";
	}

	changeGuidoTryStatus() : void {
		let disable = (this.fXmlContent.length == 0) || ($("#code").text().length == 0);
		$("#guidotry").prop('disabled', disable);
	}

	changeOptionsStatus (mode: string): void {
		$("#guidoOptions").hide();
		$("#lilyOptions").hide();
		$("#brailleOptions").hide();
		switch (mode) {
			case kGuidoMode: 	$("#guidoOptions").show(); break;
			case kLilyMode : 	$("#lilyOptions").show(); break;
			case kBrailleMode:	$("#brailleOptions").show(); break;
		}
	}

	changeMode (mode: string) : void		{ 
		this.fMode = mode;
		$("#convVersion").text (this.cversion());
		this.changeGuidoTryStatus();
		this.changeOptionsStatus(mode);
		if (this.fXmlContent.length) {
			this.convert (this.fXmlContent, this.fFileName + ".xml");
		}
	}

	convert ( script: string, path: string): void {
		if (!script.length) return;
		$("#file-name").text(path);
		this.fXmlContent = script;
		this.fFileName = path.substring(0, path.lastIndexOf('.'));
		let code = "";
		let transpose = this.getTranspose();
		if (transpose) script = this.fXmlEngine.xmlStringTranspose(script, transpose);
		try {
			switch (this.fMode) {
				case kGuidoMode:
					code = this.fXmlEngine.string2guido(script, $("#gmnbars").is(":checked"));
					this.changeGuidoTryStatus();
					break;
				case kLilyMode:
					code = this.fXmlEngine.string2lily(script, <string>$("#lilyopt").val());
					break;
				case kBrailleMode:
					code = "not yet available";
					break;
			}
			if (code.length)
				$("#code").text(code);
			else
				$("#lognav").click();
		}
		catch {
			console.error ("Exception raised while converting " + path);
			$("#lognav").click();
		}
	} 
}
