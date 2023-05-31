let btn1 = document.querySelector('#btn1');
let imgSOS = document.querySelector('#call');
let btn2 = document.querySelector('#btn2');

let btn3 = document.querySelector('#btn3');
let imgLight = document.querySelector('#light');
let btn4 = document.querySelector('#btn4');

let btn5 = document.querySelector('#btn5');
let imgSound = document.querySelector('#sound');
let btn6 = document.querySelector('#btn6');


function sosDisplay (){
    btn1.addEventListener('click', ()=>{
        imgSOS.src = 'img/fan.gif'; 
        firebase.database().ref("thietbi1").set({fan: 1})
        
    })
    btn2.addEventListener('click', ()=>{
        imgSOS.src = 'img/fan.png'; 
        firebase.database().ref("thietbi1").set({fan: 0})
        
    })
}
function lightDisplay (){
    btn3.addEventListener('click', ()=>{
        imgLight.src = 'img/light.gif'; 
        firebase.database().ref("thietbi2").set({light: 1})
        
    })
    btn4.addEventListener('click', ()=>{
        imgLight.src = 'img/light.png'; 
        firebase.database().ref("thietbi2").set({light: 0})
        
    })
}
function soundDisplay (){
    btn5.addEventListener('click', ()=>{
        imgSound.src = 'img/sound.gif'; 
        firebase.database().ref("thietbi3").set({sound: 1})
        
    })
    btn6.addEventListener('click', ()=>{
        imgSound.src = 'img/sound.png';
        firebase.database().ref("thietbi3").set({sound: 0}) 
        
    })
}



