var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g;
    return g = { next: verb(0), "throw": verb(1), "return": verb(2) }, typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (_) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
///<reference path="libmusicxml.d.ts"/>
//----------------------------------------------------------------------------
// the libMusicXML interface
//----------------------------------------------------------------------------
var libmusicxml = /** @class */ (function () {
    function libmusicxml() {
    }
    libmusicxml.prototype.initialise = function () {
        return __awaiter(this, void 0, void 0, function () {
            var module;
            var _this = this;
            return __generator(this, function (_a) {
                module = MusicXMLModule();
                return [2 /*return*/, new Promise(function (success, failure) {
                        module['onRuntimeInitialized'] = function () {
                            _this.fLibrary = new module.libMusicXMLAdapter();
                            // this.moduleInit (module);
                            success(_this);
                        };
                    })];
            });
        });
    };
    //------------------------------------------------------------------------
    // async initialization
    // moduleInit ( module ) {
    // 	this.fLibrary = new module.libMusicXMLAdapter();
    // }
    //------------------------------------------------------------------------
    // libMusicXML interface
    libmusicxml.prototype.libVersion = function () { return this.fLibrary.libVersion(); };
    libmusicxml.prototype.libVersionStr = function () { return this.fLibrary.libVersionStr(); };
    libmusicxml.prototype.musicxml2guidoVersion = function () { return this.fLibrary.musicxml2guidoVersion(); };
    libmusicxml.prototype.musicxml2guidoVersionStr = function () { return this.fLibrary.musicxml2guidoVersionStr(); };
    libmusicxml.prototype.musicxml2lilypondVersion = function () { return this.fLibrary.musicxml2lilypondVersion(); };
    libmusicxml.prototype.musicxml2lilypondVersionStr = function () { return this.fLibrary.musicxml2lilypondVersionStr(); };
    libmusicxml.prototype.musicxml2brailleVersion = function () { return this.fLibrary.musicxml2brailleVersion(); };
    libmusicxml.prototype.musicxml2brailleVersionStr = function () { return this.fLibrary.musicxml2brailleVersionStr(); };
    libmusicxml.prototype.string2guido = function (xml, genBars) { return this.fLibrary.string2guido(xml, genBars); };
    libmusicxml.prototype.xmlStringTranspose = function (xml, interval) { return this.fLibrary.xmlStringTranspose(xml, interval); };
    libmusicxml.prototype.string2lily = function (xml, options) { return this.fLibrary.string2lily(xml, options); };
    libmusicxml.prototype.string2braille = function (xml, options) { return this.fLibrary.string2braille(xml, options); };
    return libmusicxml;
}());
//----------------------------------------------------------------------------
// a download function
//----------------------------------------------------------------------------
function download(filename, text) {
    downloadMedia(filename, text, "text/plain;charset=utf-8,");
    // var element = document.createElement('a');
    // element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
    // element.setAttribute('download', filename);
    // element.style.display = 'none';
    // document.body.appendChild(element);
    // element.click();
    // document.body.removeChild(element);
}
function downloadMedia(filename, data, type) {
    var element = document.createElement('a');
    element.setAttribute('href', 'data:' + type + encodeURIComponent(data));
    element.setAttribute('download', filename);
    element.style.display = 'none';
    document.body.appendChild(element);
    element.click();
    document.body.removeChild(element);
}
///<reference path="lib/libmusicxml.ts"/>
///<reference path="download.ts"/>
var kGuidoMode = "1";
var kLilyMode = "2";
var kBrailleMode = "3";
//----------------------------------------------------------------------------
// this is the editor part, currently using CodeMirror
//----------------------------------------------------------------------------
var XMLConverter = /** @class */ (function () {
    function XMLConverter() {
        this.fFileName = "";
        this.fXmlContent = "";
        this.fCode = document.getElementById("code");
        this.fLogs = document.getElementById("logs");
        $(document).on({
            dragover: function () { return false; },
            drop: function () { return false; }
        });
    }
    //------------------------------------------------------------------------
    XMLConverter.prototype.capture = function (event) {
        event.stopImmediatePropagation();
        event.preventDefault();
    };
    XMLConverter.prototype.dragEnter = function (event) {
        this.capture(event);
        this.fCode.style.opacity = "0.3";
    };
    XMLConverter.prototype.dragLeave = function (event) {
        this.capture(event);
        this.fCode.style.opacity = "1";
    };
    XMLConverter.prototype.drop = function (event) {
        this.dragLeave(event);
        var data = event.dataTransfer.getData("text");
        if (data)
            this.convert(data, "noname");
        else {
            var filelist = event.dataTransfer.files;
            if (!filelist)
                return;
            var filecount = filelist.length;
            for (var i = 0; i < filecount; i++)
                this.droppedFile(filelist[i]);
        }
    };
    XMLConverter.prototype.droppedFile = function (file) {
        var _this = this;
        var reader = new FileReader();
        reader.onload = function (event) { _this.convert(reader.result.toString(), file.name); };
        reader.readAsText(file, file.name);
    };
    XMLConverter.prototype.initialize = function (xmlengine) {
        var _this = this;
        this.fXmlEngine = xmlengine;
        var div = document.getElementById("codepane");
        div.addEventListener("dragenter", function (e) { _this.dragEnter(e); }, true);
        div.addEventListener("dragleave", function (e) { _this.dragLeave(e); }, true);
        div.addEventListener("drop", function (e) { _this.drop(e); }, true);
        $("input[name='output'").change(function (event) { _this.changeMode($("input[name='output']:checked").val()); });
        $("#gmnbars").change(function (event) { _this.convert(_this.fXmlContent, _this.fFileName + ".xml"); });
        $("#transpose").change(function (event) { _this.convert(_this.fXmlContent, _this.fFileName + ".xml"); });
        $("#guidotry").click(function (event) { _this.tryGuido(); });
        $("#lilyhelp").click(function (event) { _this.lilyHelp(); });
        $("#clearlog").click(function (event) { $("#logs").html(""); });
        $("#save").click(function (event) { _this.save(); });
        $("#font-size").change(function (event) { _this.fontSize($("#font-size").val()); });
        this.fontSize($("#font-size").val());
        this.changeMode($("input[name='output']:checked").val());
        $('#lilyopt').on('blur', function (event) { _this.convert(_this.fXmlContent, _this.fFileName + ".xml"); });
        // let logs = document.getElementById ("logs");
        // $("#log-font").click		( () => { logs.style.fontFamily = <string>$("#log-font").val(); });
        // $("#log-size").click		( () => { logs.style.fontSize = $("#log-size").val() + "px"; });
        // logs.style.fontFamily = <string>$("#log-font").val();
        // logs.style.fontSize = $("#log-size").val() + "px";
    };
    XMLConverter.prototype.lilyHelp = function () {
        this.fXmlEngine.string2lily("", "-help");
        $("#lognav").click();
    };
    XMLConverter.prototype.tryGuido = function () {
        var gmn = $("#code").text();
        if (gmn.length) {
            if (gmn.length < 2000)
                window.open("https://guidoeditor.grame.fr/?code=" + btoa(gmn), '_blank');
            else {
                var md = forge.md.sha256.create();
                md.update(gmn);
                var id = md.digest().toHex();
                localStorage.setItem(id, gmn);
                window.open("https://guidoeditor.grame.fr/?s=" + id, '_blank');
            }
        }
    };
    XMLConverter.prototype.fontSize = function (val) {
        var pre = document.getElementsByTagName("pre");
        var i = 0;
        var px = val + "px";
        while (i < pre.length) {
            pre[i++].style.fontSize = px;
        }
    };
    XMLConverter.prototype.save = function () {
        if (this.fXmlContent.length) {
            var content = this.fCode.innerText;
            if (content.length)
                download(this.fFileName + this.outSuffix(), this.fCode.innerText);
        }
    };
    XMLConverter.prototype.outSuffix = function () {
        switch (this.fMode) {
            case kGuidoMode: return ".gmn";
            case kLilyMode: return ".ly";
            case kBrailleMode: return ".brl";
        }
        return "";
    };
    XMLConverter.prototype.getTranspose = function () {
        var trsp = $("#transpose").val();
        if (trsp.length)
            return parseInt(trsp);
        return 0;
    };
    XMLConverter.prototype.cversion = function () {
        switch (this.fMode) {
            case kGuidoMode: return this.fXmlEngine.musicxml2guidoVersionStr();
            case kLilyMode: return this.fXmlEngine.musicxml2lilypondVersionStr();
            case kBrailleMode: return this.fXmlEngine.musicxml2brailleVersionStr();
        }
        return "";
    };
    XMLConverter.prototype.changeGuidoTryStatus = function () {
        var disable = (this.fXmlContent.length == 0) || ($("#code").text().length == 0);
        $("#guidotry").prop('disabled', disable);
    };
    XMLConverter.prototype.changeOptionsStatus = function (mode) {
        $("#guidoOptions").hide();
        $("#lilyOptions").hide();
        $("#brailleOptions").hide();
        switch (mode) {
            case kGuidoMode:
                $("#guidoOptions").show();
                break;
            case kLilyMode:
                $("#lilyOptions").show();
                break;
            case kBrailleMode:
                $("#brailleOptions").show();
                break;
        }
    };
    XMLConverter.prototype.changeMode = function (mode) {
        this.fMode = mode;
        $("#convVersion").text(this.cversion());
        this.changeGuidoTryStatus();
        this.changeOptionsStatus(mode);
        if (this.fXmlContent.length) {
            this.convert(this.fXmlContent, this.fFileName + ".xml");
        }
    };
    XMLConverter.prototype.convert = function (script, path) {
        if (!script.length)
            return;
        $("#file-name").text(path);
        this.fXmlContent = script;
        this.fFileName = path.substring(0, path.lastIndexOf('.'));
        var code = "";
        var transpose = this.getTranspose();
        if (transpose)
            script = this.fXmlEngine.xmlStringTranspose(script, transpose);
        try {
            switch (this.fMode) {
                case kGuidoMode:
                    code = this.fXmlEngine.string2guido(script, $("#gmnbars").is(":checked"));
                    this.changeGuidoTryStatus();
                    break;
                case kLilyMode:
                    code = this.fXmlEngine.string2lily(script, $("#lilyopt").val());
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
        catch (_a) {
            console.error("Exception raised while converting " + path);
            $("#lognav").click();
        }
    };
    return XMLConverter;
}());
function mylog(text, level) {
    var dest = document.getElementById("logs");
    if (level == 1)
        text = '<span class="warning">' + text + '</span>';
    else if (level == 2)
        text = '<span class="error">' + text + '</span>';
    dest.innerHTML += text + "\n";
}
var oldcons = console;
var console = (function (oldCons) {
    return {
        log: function (text) {
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
///<reference path="lib/libmusicxml.ts"/>
///<reference path="converter.ts"/>
//------------------------------------------------------------------------
function xmlversion(lxml) {
    console.log("LibMusicXML version " + lxml.libVersionStr());
    $("#lxmlversion").html(lxml.libVersionStr());
    console.log("MusicXML to GMN converter version " + lxml.musicxml2guidoVersionStr());
    console.log("MusicXML to Lilypond converter version " + lxml.musicxml2lilypondVersionStr());
    // 	console.log( "MusicXML to Braille converter version " + lxml.musicxml2brailleVersionStr());
    $("#version").html(lxml.libVersionStr());
    $("#guidoversion").html(lxml.musicxml2guidoVersionStr());
    $("#lilyversion").html(lxml.musicxml2lilypondVersionStr());
    // $("#brailleversion").html (lxml.musicxml2brailleVersionStr());
}
var lxml = new libmusicxml();
var converter = new XMLConverter();
lxml.initialise().then(function () {
    xmlversion(lxml);
    converter.initialize(lxml);
});
document.domain = "grame.fr";
