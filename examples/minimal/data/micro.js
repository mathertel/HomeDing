"use strict";var MicroState,__decorate=this&&this.__decorate||function(t,e,s,i){var n,o=arguments.length,a=o<3?e:null===i?i=Object.getOwnPropertyDescriptor(e,s):i;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)a=Reflect.decorate(t,e,s,i);else for(var c=t.length-1;c>=0;c--)(n=t[c])&&(a=(o<3?n(a):o>3?n(e,s,a):n(e,s))||a);return o>3&&a&&Object.defineProperty(e,s,a),a};!function(t){t[t.PREP=1]="PREP",t[t.INIT=2]="INIT",t[t.LOADED=3]="LOADED"}(MicroState||(MicroState={}));class MicroRegistry{_tco=null;_registry={};_state=MicroState.PREP;_unloadedList=[];List=[];constructor(){this._state=MicroState.INIT,window.addEventListener("DOMContentLoaded",this.init.bind(this))}loadFile(t){return fetch(t).then((t=>t.text())).then((t=>{const e=document.createRange().createContextualFragment(t);this._tco||(this._tco=document.getElementById("u-templates")),this._tco||(this._tco=createHTMLElement(document.body,"div",{id:"u-templates"})),this._tco&&this._tco.appendChild(e)}))}attach(t){if(this._state===MicroState.LOADED){const e=t.getAttribute("u-is");if(e){const s=this._registry[e];s&&this.loadBehavior(t,s)}}else this._unloadedList.push(t)}_setPlaceholders(t,e){function s(t){return Object.getOwnPropertyNames(e).forEach((s=>t=t.replace(new RegExp("\\$\\{"+s+"\\}","g"),e[s]))),t}if(e)if(t.nodeType===Node.TEXT_NODE)t.textContent&&(t.textContent=s(t.textContent));else if(t.nodeType===Node.ELEMENT_NODE){const i=t.attributes;for(let e=0;e<i.length;e++){const n=i[e].value;n.indexOf("${")>=0&&(t[i[e].name]&&void 0!==t[i[e].name].baseVal?t[i[e].name].baseVal=s(n):t.setAttribute(i[e].name,s(n)))}t.childNodes.forEach((t=>{this._setPlaceholders(t,e)}))}}isVisible(t){let e=!1;if(t.offsetWidth>0&&t.offsetHeight>0){const s=t.getBoundingClientRect();e=s.top<=window.innerHeight&&s.bottom>=0}return e}loadDataImage(t){t.dataset.src&&this.isVisible(t)&&(t.src=t.dataset.src)}insertTemplate(t,e,s){let i=null;if(t&&e&&this._tco){const n=this._tco.querySelector('[u-control="'+e.toLowerCase()+'"]');n&&(i=n.cloneNode(!0)),i&&(i.params=s,this._setPlaceholders(i,s),t.appendChild(i),t.querySelectorAll("[u-is]").forEach((t=>micro.attach(t))),this._setPlaceholders(i,s),t.querySelectorAll("[data-src]:not([src])").forEach((t=>this.loadDataImage(t))))}return i}getMethods(t){const e=new Set;do{Object.getOwnPropertyNames(t).filter((e=>"function"==typeof t[e])).forEach((t=>e.add(t)))}while(t=Object.getPrototypeOf(t));return e}loadBehavior(t,e){const s=e,i=t;if(t)if(e)if(i._attachedBehavior===e);else{for(const e of t.attributes)t[e.name]||(t[e.name]=e.value);for(const e of this.getMethods(s))"on_touchstart"===e?t.addEventListener(e.substring(3),s[e].bind(t),{passive:!0}):"on_"===e.substring(0,3)?t.addEventListener(e.substring(3),s[e].bind(t),!1):"on"===e.substring(0,2)?t.addEventListener(e.substring(2),s[e].bind(t),!1):(null==s[e]||s[e].constructor!==Function)&&t[e]||(t[e]=s[e]);i._attachedBehavior=e,t.parentElement!==this._tco&&(i.connectedCallback(),this.List.push(t))}else;else;}define(t,e){this._registry[t]=e}init(){this._state=MicroState.LOADED,this._tco||(this._tco=document.getElementById("u-templates")),document.querySelectorAll("[u-is]").forEach((t=>micro.attach(t))),this._unloadedList.forEach((t=>{const e=t.getAttribute("u-is");if(e){const s=this._registry[e];s&&this.loadBehavior(t,s),this.List.push(t)}})),this._unloadedList=[]}}const micro=new MicroRegistry,obs=new MutationObserver((function(t,e){for(const e of t)e.addedNodes.forEach((t=>{const e=t;e.getAttribute&&e.getAttribute("u-is")&&micro.attach(t)}))}));obs.observe(document,{childList:!0,subtree:!0}),document.addEventListener("DOMContentLoaded",(function(){function t(){document.querySelectorAll("[data-src]:not([src])").forEach((t=>micro.loadDataImage(t)))}window.addEventListener("scroll",t),window.setTimeout(t,40)}));class MicroControlClass{connectedCallback(){}_clearWhitespace(){let t=this.firstChild;for(;t;){const e=t.nextSibling;3===t.nodeType&&t.parentNode?.removeChild(t),t=e}}}function MicroControl(t){return function(e){return micro.define(t,new e),e}}var GenericWidgetClass_1;let GenericWidgetClass=class extends MicroControlClass{static{GenericWidgetClass_1=this}microid;data;actions;subId;static idc=42;uid(t){return t.id||(t.id="o"+GenericWidgetClass_1.idc++),t.id}connectedCallback(){super.connectedCallback(),this.querySelectorAll("label:not([for])+input").forEach((t=>{t.previousElementSibling.htmlFor=this.uid(t)})),this.querySelectorAll("label:not([for])+div input").forEach((t=>{const e=t.parentElement?.previousElementSibling;e.htmlFor=this.uid(t)})),this.microid||(this.microid=""),this.data={id:this.microid},this.actions=[],this.subId=hub.subscribe(this.microid+"?*",this.newData.bind(this)),hub.replay(this.subId)}newData(t,e,s){this.data[e]=s;const i=this.querySelector("h1,h3,img");i&&setAttr(i,"title",JSON.stringify(this.data,null,1).replace("{\n","").replace("\n}","")),"active"===e&&this.classList.toggle("active",toBool(s)),["span","div"].forEach((t=>{this.querySelectorAll(`${t}[u-active='${e}']`).forEach((function(t){const e=toBool(s);setAttr(t,"value",e?"1":"0"),setAttr(t,"title",e?"active":"not active"),t.classList.toggle("active",e)}))})),this.querySelectorAll(`*[u-display='${e}']`).forEach((t=>{t.style.display=s?"":"none"})),this.querySelectorAll(`*[u-text='${e}']`).forEach((t=>{t.textContent!==s&&(t.textContent=s)})),["input","output","select"].forEach((t=>{this.querySelectorAll(`${t}[u-value='${e}']`).forEach((t=>{"radio"===t.type?t.checked=t.value===s:t.value!==s&&(t.value=s||"")}))})),this.querySelectorAll(`span[u-color='${e}']`).forEach((function(t){let e=s?s.replace(/^x/,"#"):"#888";e=e.replace(/^#\S{2}(\S{6})$/,"#$1"),t.style.backgroundColor=e}))}dispatchNext(){if(this.actions){const t=this.actions.shift();if(t){const e=t.split("="),s=e[0]+"="+encodeURIComponent(e[1]);fetch(s).then((()=>{if(this.actions.length>0)debounce(this.dispatchNext.bind(this))();else try{window.updateState()}catch{}}))}}}dispatchAction(t,e){t&&e&&(t.includes("/")?(t=t.replace("${v}",encodeURI(e))).split(",").forEach((t=>{t.startsWith("/")||(t="/"+t),this.actions.push("/api/state"+t)})):this.actions.push(`/api/state${this.microid}?${t}=${encodeURI(e)}`),debounce(this.dispatchNext.bind(this))())}showSys(){return toBool(getHashParams({sys:!1}).sys)}on_change(t){const e=t.target,s=e.getAttribute("u-units");this.dispatchAction(e.getAttribute("u-value"),e.value+(s||""))}on_click(t){const e=[];let s=t.target;for(;s&&(e.push(s),s!==this);)s=s.parentElement;e.every((t=>{let e=!1;if(t.getAttribute("u-action"))this.dispatchAction(t.getAttribute("u-action"),t.getAttribute("value")||"1");else if(t.classList.contains("setconfig")){const t=this.microid.split("/");DialogClass.openModalForm("configElement",{...this.data,type:t[1],id:t[2]})}else t.classList.contains("setactive")?this.dispatchAction(toBool(this.data.active)?"stop":"start","1"):t.classList.contains("fullscreen")?this.requestFullscreen():e=!0;return e}))}};GenericWidgetClass=GenericWidgetClass_1=__decorate([MicroControl("generic")],GenericWidgetClass);let BL0937WidgetClass=class extends GenericWidgetClass{connectedCallback(){super.connectedCallback(),this.data={id:this.microid},hub.replay(this.subId)}newData(t,e,s){super.newData(t,e,s),"mode"===e&&["current","voltage"].forEach((t=>{this.querySelector(`[u-text="${t}"]`).parentElement.style.display=t===s?"":"none"}))}};BL0937WidgetClass=__decorate([MicroControl("bl0937")],BL0937WidgetClass);let ButtonWidgetClass=class extends GenericWidgetClass{_timer;_start;_duration;_objButton;connectedCallback(){super.connectedCallback();const t=document.querySelector(".panel");let e=t.querySelector(".btnPanel");e||(e=createHTMLElement(t,"div",{class:"card btnPanel"},t.firstElementChild)),e&&e.appendChild(this),this._objButton=this.querySelector("button")}on_click(t){super.on_click(t),t.target===this._objButton&&(this._duration>800?this.dispatchAction("action=press","1"):(this._timer&&window.clearTimeout(this._timer),this._timer=window.setTimeout((()=>{this.dispatchAction("action=click","1")}),250)))}on_dblclick(t){t.target===this._objButton&&(this._timer&&window.clearTimeout(this._timer),this.dispatchAction("action=doubleclick","1"))}on_pointerdown(t){t.target===this._objButton&&(this._start=(new Date).valueOf())}on_pointerup(t){t.target===this._objButton&&(this._duration=(new Date).valueOf()-this._start)}};ButtonWidgetClass=__decorate([MicroControl("button")],ButtonWidgetClass);let ColorWidgetClass=class extends GenericWidgetClass{_value;_color;_white;_brightness;_duration;connectedCallback(){super.connectedCallback(),this._value="00000000",this._color="x000000",this._white=void 0}newData(t,e,s){if(super.newData(t,e,s),"value"===e){const t=this.normColor(s);t.match(/[0-9a-z]{8}/)?this._color="#"+t.substring(2):this._color=t,this._white=parseInt(t.substring(0,2),16),t!==this._value&&(this._value=t,this.querySelectorAll("*[name=value]").forEach((t=>{t.value=s})),this.querySelectorAll("*[name=color]").forEach((t=>{t.value=this._color})),this.querySelectorAll("*[name=white]").forEach((t=>{t.value=String(this._white)})))}else if("brightness"===e)this._brightness=parseInt(s,10),this.querySelectorAll("*[name=brightness]").forEach((t=>{t.value=String(this._brightness)}));else if("duration"===e)this._duration=parseInt(s,10),this.querySelectorAll("*[name=duration]").forEach((t=>{t.value=String(this._duration)}));else if("config"===e&&"wrgb"===s.toLowerCase()){let t=this.querySelector("input[name=white]");t&&(t=t.parentElement),t&&t.previousElementSibling&&(t.style.display="",t.previousElementSibling.style.display="")}}on_input(t){const e=t.target.name,s=t.target.value;if("brightness"===e)this._brightness=parseInt(s,10),this.dispatchAction(e,s);else if("white"===e){this._white=parseInt(s,10);const t="x"+this.x16(this._white)+this._color.substring(1);this.dispatchAction("value",t)}else if("color"===e){this._color=s;let t=this._color.substring(1);this._white&&(t=this.x16(this._white)+t),this.dispatchAction("value","x"+t)}else"duration"===e&&(this._duration=parseInt(s,10),this.dispatchAction(e,s+"ms"))}normColor(t){const e={black:"000000",red:"ff0000",green:"00ff00",blue:"0000ff",white:"ffffff"};return t&&0!==t.length?("x"!==(t=e[t=t.toLowerCase()]??t).substring(0,1)&&"#"!==t.substring(0,1)||(t=t.substring(1)),6===t.length&&(t="00"+t)):t="00000000",t.toLowerCase()}x16(t){let e=t.toString(16);return 1===e.length&&(e="0"+e),e}};ColorWidgetClass=__decorate([MicroControl("color")],ColorWidgetClass);let DSTimeWidgetClass=class extends GenericWidgetClass{_nowObj;connectedCallback(){super.connectedCallback(),this._nowObj=this.querySelector(".setnow"),window.setInterval(function(){setTextContent(this._nowObj,this.isoDate())}.bind(this),200)}on_click(t){const e=t.target;e&&e.classList.contains("setnow")?this.dispatchAction("time",this.isoDate()):super.on_click(t)}isoDate(){function t(t){return(t<10?"0":"")+t}const e=new Date;return e.getFullYear()+"-"+t(e.getMonth()+1)+"-"+t(e.getDate())+" "+t(e.getHours())+":"+t(e.getMinutes())+":"+t(e.getSeconds())}};DSTimeWidgetClass=__decorate([MicroControl("dstime")],DSTimeWidgetClass);let IncludeWidgetClass=class extends MicroControlClass{ref;connectedCallback(){const t=document.querySelector("#u-templates "+this.ref);if(t){const e=t.cloneNode(!0),s=this.parentElement;s?.replaceChild(e,this)}}};IncludeWidgetClass=__decorate([MicroControl("include")],IncludeWidgetClass);let InputWidgetClass=class extends MicroControlClass{_input;_type;_value;connectedCallback(){const t=this.querySelector("input");this._input=this,"INPUT"!==this.tagName&&t&&(this._input=t),super.connectedCallback();let e=this._input.getAttribute("type")||"text";"range"===e&&this._input.classList.contains("switch")&&(e="switch",this._input.min="0",this._input.max="1"),this._type=e,this._value=this._input.value,this._clearWhitespace()}_check(){let t=this._value;const e=this._type;"checkbox"===e?t=this._input.checked?"1":"0":"range"!==e&&"switch"!==e||(t=this._input.value),t!==this._value&&(this._value=t,this._input.dispatchEvent(new Event("change",{bubbles:!0})))}on_change(){this._check()}on_click(t){let e=t.target;for(this._value=this._input.value;e;){if("range"===this._type||"switch"===this._type){const t=e.classList;if(t.contains("up")){const t=Number(this._input.value)+Number(this._input.step||1);this._input.value=String(t);break}if(t.contains("down")){const t=Number(this._input.value)-Number(this._input.step||1);this._input.value=String(t);break}}if("switch"===this._type&&(e===this._input||e===this)){this._input.value=String(1-Number(this._input.value));break}if(e===this)break;e=e.parentElement}this._input.focus(),this._check()}};function jsonParse(t,e){!function t(s,i,n){let o=i?s+"/"+i:s;if(o=o.replace("/[","["),Array.isArray(n))for(let e=0;e<n.length;e++)t(o,"["+e+"]",n[e]);else"object"==typeof n?(e(o,"",""),Object.getOwnPropertyNames(n).forEach((e=>t(o,e,n[e])))):e(s,i,String(n))}("","",t)}function jsonFind(t,e){"/"===e[0]&&(e=e.substring(1));const s=e.split("/");for(;t&&s.length>0;){const e=s[0].toLowerCase(),i=Object.keys(t).find((t=>t.toLowerCase()===e));t=i?t[i]:void 0,s.shift()}return t}function jsonLocate(t,e){"/"===e[0]&&(e=e.substring(1));const s=e.split("/");for(;t&&s.length>0;){const e=s[0],i=Object.keys(t).find((t=>t.toLowerCase()===e.toLowerCase()));t=i?t[i]:t[e]={},s.shift()}return t}InputWidgetClass=__decorate([MicroControl("input")],InputWidgetClass);let PWMOutWidgetClass=class extends GenericWidgetClass{_range;_last;connectedCallback(){super.connectedCallback(),hub.subscribe(this.microid+"?*",this.newValue.bind(this)),this._range=255,this._last=""}newValue(t,e,s){if(s){if("range"===e)this._range=Number(s);else if("value"===e&&this._last!==s){const t=this.querySelector(".ux-levelbar"),e=t.offsetHeight;let i=e*Number(s)/this._range;i>e-1&&(i=e-1),i<1&&(i=1),t.style.borderBottomWidth=i+"px",this._last=s}}else;}};var SceneWidgetClass_1;PWMOutWidgetClass=__decorate([MicroControl("pwmout")],PWMOutWidgetClass);let SceneWidgetClass=class extends GenericWidgetClass{static{SceneWidgetClass_1=this}static _sceneCard;_buttonObj;connectedCallback(){super.connectedCallback(),SceneWidgetClass_1._sceneCard?this.style.display="none":SceneWidgetClass_1._sceneCard=this;const t=SceneWidgetClass_1._sceneCard.querySelector("div.block:last-child");this._buttonObj=createHTMLElement(t,"button",{microid:this.microid}),this._buttonObj.textContent="-"}on_click(t){let e=t.target.getAttribute("microid");e&&(e.startsWith("/")&&(e=e.substring(1)),this.dispatchAction(e+"?start=1","1"))}startScene(){}newData(t,e,s){super.newData(t,e,s),"title"===e&&(this._buttonObj.textContent=s)}};SceneWidgetClass=SceneWidgetClass_1=__decorate([MicroControl("scene")],SceneWidgetClass);let SelectWidgetClass=class extends GenericWidgetClass{_objSelect;connectedCallback(){super.connectedCallback(),this._objSelect=this.querySelector("select"),this.subId=hub.subscribe(this.microid+"/options[*]?*",this.newData.bind(this)),hub.replay(this.subId)}newData(t,e,s){super.newData(t,e,s);const i=t.match(/\/options\[(\d+)\]/);if(i){const t=this._objSelect.options;let n;const o=Number(i[1]);o<t.length?n=t[o]:(n=document.createElement("option"),t.add(n)),"key"===e?n.text=s:"value"===e&&(n.value=s)}else"key"===e&&(this._objSelect.value=s)}on_change(t){super.on_change(t),this.dispatchAction(this.microid+"?index=${v}",String(this._objSelect.selectedIndex))}};function toBool(t){if(!t)return!1;switch(t.toLowerCase().trim()){case"true":case"yes":return!0;case"false":case"no":case"0":case null:return!1;default:return Boolean(t)}}function toSeconds(t){let e=0;return e=(t=t.toLowerCase()).endsWith("h")?60*parseInt(t,10)*60:t.endsWith("m")?60*parseInt(t,10):t.endsWith("s")?parseInt(t,10):t.includes(":")?(Date.parse("1.1.1970 "+t)-Date.parse("1.1.1970"))/1e3:Number(t),e}function setTextContent(t,e){t&&t.textContent!==e&&(t.textContent=e)}function setAttr(t,e,s){t&&t.getAttribute(e)!==s&&t.setAttribute(e,s)}function changeConfig(t,e){let s,i,n;n="/env.json",s=JSON.parse(hub.read("env")),i=jsonFind(s,t),i||(n="/config.json",s=JSON.parse(hub.read("config")),i=jsonLocate(s,t));for(const t in e){const s=Object.keys(i).find((e=>e.toLowerCase()===t.toLowerCase()));e[t]?i[s||t]=e[t]:delete i[t]}const o=new FormData;o.append(n,new Blob([JSON.stringify(s)],{type:"text/html"}),n),fetch("/",{method:"POST",body:o}).then((function(){window.alert("saved.")}))}function debounce(t,e=20){let s;return function(){s&&clearTimeout(s),s=window.setTimeout((()=>{s=0,t()}),e)}}function getHashParams(t){const e={...t};return window.location.hash.substring(1).split("&").forEach((function(t){const s=t.split("=");e[s[0]]=s[1]})),e}function createHTMLElement(t,e,s,i=null){const n=document.createElement(e);if(s)for(const t in s)n.setAttribute(t,s[t]);return i?t.insertBefore(n,i):t.appendChild(n),n}SelectWidgetClass=__decorate([MicroControl("select")],SelectWidgetClass);let TimerWidgetClass=class extends GenericWidgetClass{wt=0;pt=0;ct=0;time=0;connectedCallback(){super.connectedCallback(),this.wt=0,this.pt=0,this.ct=0,this.time=0}newData(t,e,s){if(super.newData(t,e,s),"waittime"===e?this.wt=toSeconds(s):"pulsetime"===e?this.pt=toSeconds(s):"cycletime"===e?this.ct=toSeconds(s):"time"===e&&(this.time=toSeconds(s)),this.ct<this.wt+this.pt&&(this.ct=this.wt+this.pt),this.ct>0){const t=this.querySelector(".u-bar"),e=t.clientWidth/this.ct,s=t.querySelector(".pulse");s.style.left=Math.floor(this.wt*e)+"px",s.style.width=Math.floor(this.pt*e)+"px";t.querySelector(".current").style.width=Math.floor(this.time*e)+"px"}}};TimerWidgetClass=__decorate([MicroControl("timer")],TimerWidgetClass);let ValueWidget=class extends GenericWidgetClass{_input;connectedCallback(){super.connectedCallback(),this._input=this.querySelector("input")}newData(t,e,s){super.newData(t,e,s),this._input&&("min"===e?this._input.min=s:"max"===e?this._input.max=s:"step"===e&&(this._input.step=s))}};ValueWidget=__decorate([MicroControl("value")],ValueWidget);class MicroHub{_registrations={};_registrationsId=0;_store={};read(t){return this._findStoreObject(this.pPath(t))[this.pKey(t)]}write(t,e){this._findStoreObject(this.pPath(t))[this.pKey(t)]=e}subscribe(t,e,s=!1){const i=this._registrationsId,n="^"+t.toLocaleLowerCase().replace(/(\[|\]|\/|\?)/g,"\\$1").replace(/\*\*/g,"\\S{0,}").replace(/\*/g,"[^/?]*")+"$",o={id:i,match:RegExp(n),callback:e};return this._registrations[i]=o,this._registrationsId++,s&&jsonParse(this._store,function(t,e,s){let i=t+(e?"?"+e:"");i&&(i=i.toLocaleLowerCase(),i.match(o.match)&&o.callback(t,(e||"").toLowerCase(),s||""))}.bind(this)),i}unsubscribe(t){delete this._registrations[t]}replay(t){const e=this._registrations[t];e&&jsonParse(this._store,function(t,s,i){let n=t+(s?"?"+s:"");n&&(n=n.toLocaleLowerCase(),n.match(e.match)&&e.callback(t,(s||"").toLowerCase(),i||""))}.bind(this))}publishObj(t){jsonParse(t,function(t,e,s){this.publishValue(t,e?e.toLowerCase():"",s||"")}.bind(this))}publishValue(t,e,s){let i=t+(e?"?"+e:"");if(i){if(e){this._findStoreObject(t)[e]=s}i=i.toLocaleLowerCase(),Object.values(this._registrations).forEach((n=>{i.match(n.match)&&n.callback(t,e,s)}))}}onunload(){Object.getOwnPropertyNames(this._registrations).forEach((t=>delete this._registrations[t]))}_findStoreObject(t){let e=this._store;"/"===t[0]&&(t=t.substring(1));const s=t.split("/");for(;s.length>0&&e[s[0]];)e=e[s[0]],s.shift();for(;s.length>0&&s[0];)e=e[s[0]]={},s.shift();return e}pPath(t){"/"===t[0]&&(t=t.substring(1));const e=t.split("/");return e.slice(0,e.length-1).join("/")}pKey(t){const e=t.split("/");return e[e.length-1]}}const hub=new MicroHub;window.addEventListener("unload",hub.onunload.bind(hub),!1);