var activeFileName,filesObj=document.getElementById("files"),contentObj=document.getElementById("content"),progressObj=document.getElementById("progress"),activeFileObj=document.getElementById("activeFile"),checkerObj=document.getElementById("checker");function handleLoadFile(e){var t=e.target.innerText;activeFileName=t.split(" ")[0],activeFileObj.innerText=activeFileName,fetch(activeFileName).then(function(e){return e.text()}).then(function(e){contentObj.innerText=e})}function DelayPromise(n){return new Promise(function(e,t){setTimeout(e,n)})}function handleDeleteFile(e){e.stopPropagation(),e.preventDefault();var t=(t=e.target.parentElement.firstElementChild.innerText).split(" ")[0];window.confirm("Delete "+t+" ?")&&(fetch(t,{method:"DELETE"}),window.setTimeout(handleReloadFS,100))}function addFileEntry(e,t){var n=document.createElement("div");n.className="row nogutter",e.appendChild(n),n.addEventListener("click",handleLoadFile);var a=document.createElement("div");a.className="col stretch file-entry",a.innerText=a.title=t.name+" ("+t.size+")",n.appendChild(a);var o=document.createElement("div");o.className="col file-delete",o.innerText="[X]",o.addEventListener("click",handleDeleteFile),n.appendChild(o)}function listFiles(e){filesObj.innerHTML="",e.forEach(function(e){addFileEntry(filesObj,e)})}function handleReloadFS(){fetch("/$list").then(function(e){return e.json()}).then(function(e){e.sort(function(e,t){var n=e.name.toLowerCase(),a=t.name.toLowerCase();return n<a?-1:a<n?1:0}),listFiles(e)})}function dragHelper(e){e.stopPropagation(),e.preventDefault()}function startUpload(e,t,n){progressObj.classList.remove("fadeout");var a=new FormData,o=new Blob([n],{type:t});a.append(e,o,e);var r=new XMLHttpRequest;r.open("POST","/"),r.upload&&r.upload.addEventListener("progress",function(e){progressObj.max=e.total,progressObj.value=e.loaded}),r.addEventListener("readystatechange",function(e){4==r.readyState&&200<=r.status&&r.status<300&&(window.setTimeout(handleReloadFS,200),progressObj.classList.add("fadeout"))}),r.send(a)}function handleSave(){progressObj.value=0,progressObj.max=1,void 0!==(activeFileName=window.prompt("Upload File:",activeFileName))&&startUpload(activeFileName,"text/html",contentObj.innerText)}function handleFmt(){var e,t;activeFileObj.innerText.toLowerCase().endsWith(".json")&&(e=null,t=(t=(t=contentObj.innerText).replace(/\}\s*\{/g,"},{")).replace(/,\s*([\}\]])/g,"$1"),(e=JSON.parse(t))&&"object"==typeof e&&(contentObj.innerText=JSON.stringify(e,null,2)))}function drop(e){progressObj.classList.remove("fadeout"),progressObj.value=0,progressObj.max=1,e.stopPropagation(),e.preventDefault();for(var t=e.dataTransfer.files,n=new FormData,a="/"+(location.hash?location.hash.substr(1)+"/":""),o=0;o<t.length;o++)n.append("file",t[o],a+t[o].name);var r=new XMLHttpRequest;r.upload.addEventListener("progress",function(e){progressObj.max=e.total,progressObj.value=e.loaded}),r.addEventListener("readystatechange",function(e){4==r.readyState&&200<=r.status&&r.status<300&&(window.setTimeout(handleReloadFS,100),progressObj.classList.add("fadeout"))}),r.open("POST","/"),r.send(n)}var box=document.getElementById("dropzone");function jsonCheck(){if(activeFileObj.innerText.toLowerCase().endsWith(".json")){var e=null,t=contentObj.innerText;0<=t.indexOf(" ")&&(contentObj.innerText=t=t.replace(/\u00A0/g," "));try{(e=JSON.parse(t))&&"object"==typeof e||(e=null)}catch(e){checkerObj.title=e.message}null!==e?(checkerObj.textContent="ok",checkerObj.className="valid",checkerObj.title=""):(checkerObj.textContent="no",checkerObj.className="invalid"),window.status=t.length+"--"+(null!==e)}else"--"!=checkerObj.textContent&&(checkerObj.textContent="--",checkerObj.className="")}box.addEventListener("dragenter",dragHelper,!1),box.addEventListener("dragover",dragHelper,!1),box.addEventListener("drop",drop,!1),window.addEventListener("load",function(){window.setTimeout(handleReloadFS,400)}),window.setInterval(jsonCheck,500);