import{o as l,c,a as u,t as g,b as v,d as h,u as y,r as d,e as f,F as $,f as M,g as A,h as b,i as k}from"./vendor.41b16fa2.js";const w=function(){const n=document.createElement("link").relList;if(n&&n.supports&&n.supports("modulepreload"))return;for(const e of document.querySelectorAll('link[rel="modulepreload"]'))s(e);new MutationObserver(e=>{for(const r of e)if(r.type==="childList")for(const o of r.addedNodes)o.tagName==="LINK"&&o.rel==="modulepreload"&&s(o)}).observe(document,{childList:!0,subtree:!0});function a(e){const r={};return e.integrity&&(r.integrity=e.integrity),e.referrerpolicy&&(r.referrerPolicy=e.referrerpolicy),e.crossorigin==="use-credentials"?r.credentials="include":e.crossorigin==="anonymous"?r.credentials="omit":r.credentials="same-origin",r}function s(e){if(e.ep)return;e.ep=!0;const r=a(e);fetch(e.href,r)}};w();var C={name:"alert",data(){return{msg:!1}},props:{propsMsg:!1},mounted(){window.a=this.$store,this.$store.state.alertMsg=this.msg}};var p=(t,n)=>{for(const[a,s]of n)t[a]=s;return t};const L={key:0,class:"alert flex flex-item-center"},P=["textContent"];function E(t,n){return typeof t.propsMsg=="string"||typeof t.$store.state.alertMsg=="string"?(l(),c("div",L,[u("div",{class:"msg-div",textContent:g(t.propsMsg||t.$store.state.alertMsg)},null,8,P),u("div",{class:"close_btn-div",onClick:n[0]||(n[0]=a=>{t.$store.state.alertMsg=!1,t.propsMsg=!1})}," X ")])):v("",!0)}var N=p(C,[["render",E],["__scopeId","data-v-be10e6c2"]]);const O=h({name:"APP",data(){return{$store:y()}},components:{Alert:N}});function S(t,n,a,s,e,r){const o=d("Alert"),i=d("router-view");return l(),c($,null,[f(o),f(i,{class:"wrapper"})],64)}var x=p(O,[["render",S]]);const V="modulepreload",m={},B="/",F=function(n,a){return!a||a.length===0?n():Promise.all(a.map(s=>{if(s=`${B}${s}`,s in m)return;m[s]=!0;const e=s.endsWith(".css"),r=e?'[rel="stylesheet"]':"";if(document.querySelector(`link[href="${s}"]${r}`))return;const o=document.createElement("link");if(o.rel=e?"stylesheet":V,e||(o.as="script",o.crossOrigin=""),o.href=s,document.head.appendChild(o),e)return new Promise((i,_)=>{o.addEventListener("load",i),o.addEventListener("error",_)})})).then(()=>n())},H=[{name:"Home",path:"/",component:()=>F(()=>import("./Home.43737a43.js"),["assets/Home.43737a43.js","assets/vendor.41b16fa2.js"])}],I=M({history:A(),routes:H});var R=b({state:{alertMsg:!1},mutations:{alertMsg(t){t.alertMsg=!1}},actions:{},modules:{},getters:{}});k(x).use(I).use(R).mount("#app");export{p as _};
