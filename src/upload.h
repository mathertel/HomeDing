// file: upload.h
// contains the minimal html for uploading files

// from setup.min.htm: configure the network
static const char setupContent[] PROGMEM =
R"==(<!DOCTYPE html><html lang='en'><head><title>WiFi Setup</title></head><body><h1>WiFi Setup</h1><h2>Connect to network</h2><table><tr><td>Network:</td><td><select id='conNetworks'style='width:18em'></select></td></tr><tr><td>Passphrase:</td><td><input id='conPass'style='width:18em'></td></tr></table><button id='connectPass'>Connect</button><script>var scanDone=2,checkTimer=0,selObj=document.getElementById("conNetworks");function load(e,n){var t=new XMLHttpRequest;t.open("GET",e,!0),t.addEventListener("readystatechange",function(e){4==t.readyState&&200<=t.status&&t.status<300&&n(t.responseText)}),t.send()}function startScan(){2==scanDone&&(selObj.innerHTML="<option selected disabled>scanning...</option>",scanDone=0,checkTimer||(checkTimer=window.setInterval(checkScan,200)))}function checkScan(){load("/$scan",function(e){0<e.length&&(scanDone=2,scanResult(JSON.parse(e)))})}function scanResult(e){window.clearInterval(checkTimer),checkTimer=0,selObj.innerHTML="";var n=document.createElement("option");n.value=0,n.text="select...",n.disabled=!0,selObj.options.add(n),e.forEach(function(e){var n=document.createElement("option");n.value=n.text=e.id,selObj.options.add(n)})}document.getElementById("connectPass").addEventListener("click",function(){load("/$connect?n="+selObj.value+"&p="+document.getElementById("conPass").value,function(){})}),startScan()</script></body></html>)==";

// from upload.min.htm
static const char uploadContent[] PROGMEM =
R"==(<!doctype html><html lang='en'><head><title>File Upload</title></head><body><h1>File Upload</h1><div id='zone'style='width:9em;height:6em;padding:1em;background-color:#ddd'>Drop here</div><a href='#ft'>FT-Upload</a> - <a href='#i'>I-Upload</a><script>function dragHelper(e){e.stopPropagation(),e.preventDefault()}function dropped(e){dragHelper(e);for(var n=e.dataTransfer.files,t=new FormData,a="/"+(location.hash?location.hash.substr(1)+"/":""),r=0;r<n.length;r++)t.append("file",n[r],a+n[r].name);var d=new XMLHttpRequest;d.addEventListener("readystatechange",function(e){4==d.readyState&&200<=d.status&&d.status<300&&window.alert("done.")}),d.open("POST","/"),d.send(t)}var zoneObj=document.getElementById("zone");zoneObj.addEventListener("dragenter",dragHelper,!1),zoneObj.addEventListener("dragover",dragHelper,!1),zoneObj.addEventListener("drop",dropped,!1)</script></body></html>)==";

// from boot.min.htm
static const char bootContent [] PROGMEM =
R"==(<!DOCTYPE html><html lang='en'><head><title>Web Update</title></head><body><h1>Web Update</h1><table><tr><td><progress value='0'max='1'></progress></td></tr><tr><td id='info'>.</td></tr><tr><td><button>start</button></td></tr></table><script>var repo="https://homeding.github.io/",eStart=document.querySelector("button"),eBar=document.querySelector("progress"),eInfo=document.querySelector("#info"),work={status:"0",list:null,files:0,done:0},timer=0;function log(e){eInfo.innerText=e}function doStart(){work.status="w";var o=new XMLHttpRequest;o.open("GET",repo+"list.txt",!0),o.responseType="text",o.onload=function(e){var t=o.responseText.replace(/\r?\n/g,";");t=t.replace(/;$/,""),work.list=t.split(";"),work.files=work.list.length,work.status="r"},o.send(null)}function doFile(){work.status="w";var n=work.list.shift();work.done++,eBar.max=work.files,eBar.value=work.done;var s=new XMLHttpRequest;s.open("GET",repo+n,!0),s.responseType="arraybuffer",s.onerror=function(e){log(n+": error: failed."),work.status="r"},s.onload=function(e){if(200!=s.status)log(n+": error: "+s.status);else{var t=s.response;if(t){var o=new FormData;o.append("file",new Blob([t]),"/"+n);var r=new XMLHttpRequest;r.addEventListener("readystatechange",function(e){4==r.readyState&&200<=r.status&&r.status<300&&log(n)}),r.open("POST","/"),r.send(o)}}work.status=0==work.list.length?"e":"r"},s.send(null),0==work.list.length&&(work.status="e")}function step(){"0"==work.status?doStart():"r"==work.status?doFile():"e"==work.status&&(timer&&(window.clearInterval(timer),log("done"),window.location.href="/"),timer=0)}location.hash?repo+=location.hash.substr(1)+"/":repo+="v01/",log("loading web files from "+repo),eStart.addEventListener("click",function(){timer=window.setInterval(step,330)})</script></body></html>)==";
