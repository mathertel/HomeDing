const char *bootpageContent=
"<!DOCTYPE html><html lang='en'><head><title>Upload</title></head>"
"<body><div id='zone' style='width:9em;height:6em;padding:1em;background-color:#ddd'>Drop here</div>"
"<script>function t(e){e.stopPropagation();e.preventDefault()}function e(e){t(e);var a=e.dataTransfer.files;var n=new FormData;var r='/'+(location.hash?location.hash.substr(1)+'/':'');for(var i=0;i<a.length;i++){n.append('file',a[i],r+a[i].name)}var o=new XMLHttpRequest;o.addEventListener('readystatechange',function(e){if(o.readyState==4&&o.status>=200&&o.status<300){window.alert('done.')}});o.open('POST','/');o.send(n)}var a=document.getElementById('zone');a.addEventListener('dragenter',t,false);a.addEventListener('dragover',t,false);a.addEventListener('drop',e,false);</script></body>";