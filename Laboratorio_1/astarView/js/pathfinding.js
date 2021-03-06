//para referenciar al canvas con el html
var canvas;
var ctx;
var FPS = 50;

//ESCENARIO / TABLERO
var n = 25;
var columnas = n;
var filas = n;
var escenario;  //matriz del nivel

//Para los anchos de nuestros cuadraditos del mapa
//Los hallaremos de manera dinamica
var anchoT;
var altoT;

//Defino unos colores para la interfaz en hexa
const rojo = '#FF0000';
const amarillo = '#FFFF00';
const verde = '#0CFF00';
const morado = '#FF00F7';
const negro = '#000000';
const blanco = '#FFFFFF';
const plomo = '#B7B7B7';
const muro = negro;
const tierra = plomo;

//RUTA
var principio;
var fin;

var ListaAbierta = []; //Verde ya visitados
var ListaCerrada = []; //Rojo donde podemos pasar

var camino = [];
var terminado = false;




//CREAMOS UN ARRAY 2D
function creaArray2D(f,c){
  var obj = new Array(f);
  for(a=0; a<f; a++){
    obj[a] = new Array(c);
  }
  return obj;
}



function heuristica(a,b){
  var x = Math.abs(a.x - b.x);
  var y = Math.abs(a.y - b.y);

  var dist = x+y;

  return dist;
}


function borraDelArray(array,elemento){
  for(i=array.length-1; i>=0; i--){
    if(array[i] == elemento){
      array.splice(i,1);
    }
  }
}


function Casilla(x,y){

  //POSICIÓN
  this.x = x;
  this.y = y;

  //TIPO (obstáculo=1, vacío=0)
  this.tipo = 0;

  var aleatorio = Math.floor(Math.random()*10);
  if(aleatorio == 1)
      this.tipo = 1;

  //PESOS
  this.f = 0;  //coste total (g+h)
  this.g = 0;  //pasos dados
  this.h = 0;  //heurística (estimación de lo que queda)

  this.vecinos = [];
  this.padre = null;


  //MÉTODO QUE CALCULA SUS VECNIOS
  //x=1,y=0
  this.addVecinos = function(){
    if(this.x > 0)
      this.vecinos.push(escenario[this.y][this.x-1]);   //vecino izquierdo

    if(this.x < n-1)
      this.vecinos.push(escenario[this.y][this.x+1]);   //vecino derecho

    if(this.y > 0)
      this.vecinos.push(escenario[this.y-1][this.x]);   //vecino de arriba

    if(this.y < n-1)
      this.vecinos.push(escenario[this.y+1][this.x]); //vecino de abajo
    
    if(this.x > 0 && this.y < n-1)
      this.vecinos.push(escenario[this.y+1][this.x-1]); //vecino diagonal inferior izquierdo
    if(this.x > 0 && this.y > 0)
      this.vecinos.push(escenario[this.y-1][this.x-1]); //vecino diagonal superior izquierdo
    if(this.x < n-1 && this.y < n-1)
      this.vecinos.push(escenario[this.y+1][this.x+1]); //vecino diagonal inferior derecho
    if(this.x < n-1 && this.y > 0)
      this.vecinos.push(escenario[this.y-1][this.x+1]); //vecino diagonal superior derecho
      
    
  }



  //MÉTODO QUE DIBUJA LA CASILLA
  this.dibuja = function(){
    var color;

    if(this.tipo == 0)
      color = tierra;

    if(this.tipo == 1)
      color = muro;

    //DIBUJAMOS EL CUADRO EN EL CANVAS O DIBUJAMOS EL MAPO
    ctx.fillStyle = color;
    ctx.fillRect(this.x*anchoT,this.y*altoT,anchoT,altoT);
  }

  

  //DIBUJA ListaAbierta
  //Por donde ya paso
  this.dibujaOS = function(){
    ctx.fillStyle = rojo;
    ctx.fillRect(this.x*anchoT,this.y*altoT,anchoT,altoT);

  }

  //DIBUJA ListaCerrada
  //POr donde puede pasar
  this.dibujaCS = function(){
    ctx.fillStyle = verde;
    ctx.fillRect(this.x*anchoT,this.y*altoT,anchoT,altoT);
  }


  //DIBUJA CAMINO
  this.dibujaCamino = function(){
    ctx.fillStyle = morado; 
    ctx.fillRect(this.x*anchoT,this.y*altoT,anchoT,altoT);
  }

  //DIBUJA INICIO
  this.dibujaInicio = function(){
    ctx.fillStyle = blanco;
    ctx.fillRect(this.x*anchoT,this.y*altoT,anchoT,altoT);
  }

  //DIBUJO FINAL
  this.dibujaFinal = function(){
    ctx.fillStyle = morado; 
    ctx.fillRect(this.x*anchoT,this.y*altoT,anchoT,altoT);
  }

}

//Para inicializar el programa o canvas mejor dicho

function inicializa(){
  //referenciamos el objeto
  canvas = document.getElementById('canvas');
  //para hacer el mapa en 2d
  ctx = canvas.getContext('2d');

  //CALCULAMOS EL TAMAÑO DE LOS CUADRADITOS DEL MAPA (Proporcionalmente)
  //aPLICAMOS PARSEINT PARA EVITAR DECIMALES
  //EN OTRAS PALABRAS HACEMOS UN CASTEO

  anchoT = parseInt(canvas.width/columnas);
  altoT = parseInt(canvas.height/filas);

  //CREAMOS LA MATRIZ PARA EL ESCENARIO
  escenario = creaArray2D(filas,columnas);

  //AÑADIMOS LOS OBJETOS CASILLAS
  for(i=0;i<filas;i++){
    for(j=0;j<columnas;j++){
        escenario[i][j] = new Casilla(j,i);
        console.log(escenario[i][j]);
    }
  }

  //AÑADIMOS LOS VECINOS
  for(i=0;i<filas;i++){
    for(j=0;j<columnas;j++){
        escenario[i][j].addVecinos();
    }
  }

  //CREAMOS ORIGEN Y DESTINO DE LA RUTA
  principio = escenario[0][0]; //esquina superior izquierda
  fin = escenario[n-1][n-1]; //esquina inferior derecha

  //INICIALIZAMOS ListaAbierta
  ListaAbierta.push(principio);

  //EMPEZAMOS A EJECUTAR EL BUCLE PRINCIPAL
  //PARA HACER UN BUCLE Y LE DAMOS LA VELOCIDAD 
  setInterval(function(){principal();},1000/FPS);
}



function dibujaEscenario(){
  for(i=0;i<filas;i++){
    for(j=0;j<columnas;j++){
        escenario[i][j].dibuja();
    }
  }

  //DIBUJA ListaAbierta
  for(i=0; i<ListaAbierta.length; i++){
    ListaAbierta[i].dibujaOS();
  }


  //DIBUJA ListaCerrada
  for(i=0; i<ListaCerrada.length; i++){
    ListaCerrada[i].dibujaCS();
  }

  for(i=0; i<camino.length; i++){
    camino[i].dibujaCamino();
  }

  escenario[0][0].dibujaInicio();
  //escenario[n-1][n-1].dibujaFinal();

}


function borraCanvas(){
  canvas.width = canvas.width;
  canvas.height = canvas.height;
}






function Estrellita(){

  //SEGUIMOS HASTA ENCONTRAR SOLUCIÓN
  if(terminado!=true){

    //SEGUIMOS SI HAY AlGO EN ListaAbierta
    if(ListaAbierta.length>0){
      var ganador = 0;  //índie o posición dentro del array ListaAbierta del ganador

      //evaluamos que ListaAbierta tiene un menor coste / esfuerzo
      for(i=0; i<ListaAbierta.length; i++){
        if(ListaAbierta[i].f < ListaAbierta[ganador].f){
          ganador = i;
        }
      }

      //Analizamos la casilla ganadora
      var actual = ListaAbierta[ganador];

      //SI HEMOS LLEGADO AL FINAL BUSCAMOS EL CAMINO DE VUELTA
      if(actual === fin){

        var temporal = actual;
        camino.push(temporal);

        while(temporal.padre!=null){
          temporal = temporal.padre;
          camino.push(temporal);
        }
        
        //console.log(ListaAbierta.length);
        escenario[n-1][n-1].dibujaFinal();
        window.alert('Camino Encontrado, Costo -> '+ListaAbierta.length);
        terminado = true;
      }

      //SI NO HEMOS LLEGADO AL FINAL, SEGUIMOS
      else{
        borraDelArray(ListaAbierta,actual);
        ListaCerrada.push(actual);

        var vecinos = actual.vecinos;

        //RECORRO LOS VECINOS DE MI GANADOR
        for(i=0; i<vecinos.length; i++){
          var vecino = vecinos[i];

          //SI EL VECINO NO ESTÁ EN ListaCerrada Y NO ES UNA PARED, HACEMOS LOS CÁLCULOS
          if(!ListaCerrada.includes(vecino) && vecino.tipo!=1){
            var tempG = actual.g + 1;

            //si el vecino está en ListaAbierta y su peso es mayor
            if(ListaAbierta.includes(vecino)){
              if(tempG < vecino.g){
                vecino.g = tempG;     //camino más corto
              }
            }
            else{
              vecino.g = tempG;
              ListaAbierta.push(vecino);
            }

            //ACTUALIZAMOS VALORES
            vecino.h = heuristica(vecino,fin);
            vecino.f = vecino.g + vecino.h;

            //GUARDAMOS EL PADRE (DE DÓNDE VENIMOS)
            vecino.padre = actual;
          }
        }
      }
    }
    else{
      window.alert('No existe camino');
      terminado = true;   //el algoritmo ha terminado
    }
  }

}

function principal(){
  borraCanvas();
  Estrellita();
  dibujaEscenario();
}
